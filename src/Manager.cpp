#include "Manager.hpp"

#include "Utils.hpp"

WThreadManager::WThreadManager(int sysThreadNum) {
	/** Lock */
	std::lock_guard<std::mutex> locker(this->mutex);

	/** Init System Threads */
	this->sysThreadList.resize(sysThreadNum);
}

WThreadManager::~WThreadManager() {
	{
		/** Lock */
		std::lock_guard<std::mutex> locker(this->mutex);

		/** Clear Wait List */
		while (!(this->tcbWaitList.empty())) {
			auto tcbNode = this->tcbWaitList.getFirst();
			tcbNode->tcb->statusOut(true);
			this->tcbExitList.addLast(std::move(tcbNode));
		}

		/** Send Exit Message To Running Threads */
		for (int i = 0; i < this->sysThreadList.size(); i++) {
			auto& wrapper = this->sysThreadList[i];
			if (auto tcbNode = wrapper.currentNode.get()) {
				tcbNode->tcb->sendShouldExit();
			}
		}
	}

	/** Wait For Finished */
	while (!this->isFinished()) {}

	/** Detach All System Threads */
	for (int i = 0; i < this->sysThreadList.size(); i++) {
		auto& wrapper = this->sysThreadList[i];
		if (wrapper.thread) {
			wrapper.thread->detach();
		}
	}
}

const WThreadTCB* WThreadManager::add(const Func& func, size_t stackSize) {
	/** Lock */
	std::lock_guard<std::mutex> locker(this->mutex);

	/** Add TCB To Wait List */
	auto pTCB = new WThreadTCB{ *this, func, stackSize };
	auto tcbNode = std::make_unique<WThreadTCBNode>(pTCB);
	this->tcbWaitList.addLast(std::move(tcbNode));

	/** Create A Sys Thread If Need */
	for (int i = 0; i < this->sysThreadList.size(); i++) {
		auto& wrapper = this->sysThreadList[i];
		if (!wrapper.running) {
			if (wrapper.thread) {
				if (wrapper.thread->joinable()) {
					continue;
				}
			}
			wrapper.thread = std::make_unique<std::thread>(
				WThreadManager::sysThreadRunner, std::ref(*this), std::ref(wrapper));
			wrapper.running = true;
		}
	}

	/** Return Value */
	return pTCB;
}

bool WThreadManager::isFinished() const {
	/** Lock */
	std::lock_guard<std::mutex> locker(this->mutex);

	/** Check Wait List */
	if (!(this->tcbWaitList.empty())) { return false; }

	/** Check Running Threads */
	for (int i = 0; i < this->sysThreadList.size(); i++) {
		auto& wrapper = this->sysThreadList[i];
		if ((wrapper.thread && !(wrapper.thread->joinable())) || wrapper.running) {
			return false;
		}
	}

	/** Finished */
	return (this->runningThreadsCount == 0);
}

bool WThreadManager::isRunning(const WThreadTCB* tcb) const {
	/** Lock */
	std::lock_guard<std::mutex> locker(this->mutex);

	/** Check Wait List */
	if (this->tcbWaitList.contains(tcb)) { return true; }

	/** Check Running Threads */
	for (int i = 0; i < this->sysThreadList.size(); i++) {
		auto& wrapper = this->sysThreadList[i];
		if (wrapper.currentNode.get()
			&& wrapper.currentNode->tcb.get() == tcb) {
			return true;
		}
	}

	/** Not Found */
	return false;
}

void WThreadManager::signalExit(const WThreadTCB* tcb) {
	if (!this->isRunning(tcb)) { return; }

	/** Set Flag */
	const_cast<WThreadTCB*>(tcb)->sendShouldExit();
}

WThreadTCB* WThreadManager::findCurrentRunningTCB() const {
	/** Lock */
	std::lock_guard<std::mutex> locker(this->mutex);

	/** Get System Thread ID */
	auto sysID = std::this_thread::get_id();

	/** Find For Running Threads */
	for (int i = 0; i < this->sysThreadList.size(); i++) {
		auto& wrapper = this->sysThreadList[i];
		if (wrapper.thread
			&& wrapper.thread->get_id() == sysID) {
			if (wrapper.currentNode) {
				return wrapper.currentNode->tcb.get();
			}
		}
	}

	/** Not Found */
	return nullptr;
}

void WThreadManager::yield(WThreadTCB* current, YieldType type) {
	WThreadContext* pContext = nullptr;

	/** Lock Area */
	{
		/** Lock */
		std::lock_guard<std::mutex> locker(this->mutex);

		/** Get System Thread ID */
		auto sysID = std::this_thread::get_id();

		/** Find For Running Threads */
		SysWrapper* pWrapper = nullptr;
		for (int i = 0; i < this->sysThreadList.size(); i++) {
			auto& wrapper = this->sysThreadList[i];
			if (wrapper.thread
				&& wrapper.thread->get_id() == sysID
				&& wrapper.currentNode) {
				if (wrapper.currentNode->tcb.get() == current) {
					pWrapper = &wrapper;
					break;
				}
			}
		}
		if (!pWrapper) { return; }

		/** Release Wrapper */
		auto pTCBNode = 
			std::unique_ptr<WThreadTCBNode>(pWrapper->currentNode.release());
		switch (type) {
		case YieldType::Yield:
		case YieldType::Exit:
			pTCBNode->tcb->statusOut(type == YieldType::Exit);
			break;
		case YieldType::Wait:
			pTCBNode->tcb->statusWait();
			break;
		}
		pContext = &(pWrapper->context);

		/** Continue In List */
		switch (type) {
		case YieldType::Yield:
		case YieldType::Wait:
			this->tcbWaitList.addLast(std::move(pTCBNode));
			break;
		case YieldType::Exit:
			this->tcbExitList.addLast(std::move(pTCBNode));
			break;
		}
	}

	/** Jump Out */
	current->jumpOut(*pContext);
}

void WThreadManager::sysThreadRunner(WThreadManager& manager, SysWrapper& wrapper) {
	/** Increase Running Count */
	{
		std::lock_guard<std::mutex> locker(manager.mutex);
		wrapper.running = true;
		manager.runningThreadsCount++;
	}

	/** Thread Schedule */
	while (true) {
		/** Lock Area */
		{
			/** Lock */
			std::lock_guard<std::mutex> locker(manager.mutex);

			/** Wait List Empty */
			if (manager.tcbWaitList.empty()) {
				break;
			}

			/** Get First TCB In List */
			auto tcbNode = manager.tcbWaitList.getFirst();

			/** Thread Waiting */
			if (tcbNode->tcb->getStatus() == WThreadTCB::Status::Wait) {
				/** Get Current Time */
				uint64_t current = WThreadUtils::getCurrentTimeMSec();
				
				/** Not Wake Time */
				if (current < tcbNode->tcb->getWakeTime()) {
					manager.tcbWaitList.addLast(std::move(tcbNode));
					continue;
				}

				/** Wake */
				tcbNode->tcb->setWakeTime(0);
			}

			/** Move TCB To Sys Thread */
			tcbNode->tcb->statusIn();
			wrapper.currentNode = std::move(tcbNode);

			/** Create Sys Threads If Need */
			/*for (int i = 0; i < manager.sysThreadList.size(); i++) {
				auto& wr = manager.sysThreadList[i];
				if (!wr.running) {
					if (wr.thread) {
						wr.thread->join();
						wr.thread = nullptr;
					}
					wr.thread = std::make_unique<std::thread>(
						WThreadManager::sysThreadRunner, std::ref(manager), std::ref(wr));
					wr.running = true;
				}
			}*/
		}

		/** Jump In */
		wrapper.currentNode->tcb->jumpIn(wrapper.context);
	}

	/** Decrease Running Count */
	{
		std::lock_guard<std::mutex> locker(manager.mutex);
		manager.runningThreadsCount--;
		wrapper.running = false;
	}
}
