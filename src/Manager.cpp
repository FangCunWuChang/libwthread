#include "Manager.hpp"

WThreadManager::WThreadManager(int sysThreadNum) {
	/** Lock */
	std::lock_guard<std::mutex> locker(this->mutex);

	/** Init System Threads */
	this->sysThreadList.resize(sysThreadNum);
	for (int i = 0; i < sysThreadNum; i++) {
		auto& wrapper = this->sysThreadList[i];
		wrapper.thread = std::thread{ WThreadManager::sysThreadRunner, std::ref(wrapper) };
	}
}

WThreadManager::~WThreadManager() {
	{
		/** Lock */
		std::lock_guard<std::mutex> locker(this->mutex);

		/** Clear Wait List */
		this->tcbWaitList.clear();

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
}

bool WThreadManager::isFinished() const {
	/** Lock */
	std::lock_guard<std::mutex> locker(this->mutex);

	/** Check Wait List */
	if (!(this->tcbWaitList.empty())) { return false; }

	/** Check Running Threads */
	for (int i = 0; i < this->sysThreadList.size(); i++) {
		auto& wrapper = this->sysThreadList[i];
		if (wrapper.currentNode.get()) {
			return false;
		}
	}

	/** Finished */
	return true;
}

WThreadTCB* WThreadManager::findCurrentRunningTCB() const {
	/** Lock */
	std::lock_guard<std::mutex> locker(this->mutex);

	/** Get System Thread ID */
	auto sysID = std::this_thread::get_id();

	/** Find For Running Threads */
	for (int i = 0; i < this->sysThreadList.size(); i++) {
		auto& wrapper = this->sysThreadList[i];
		if (wrapper.thread.get_id() == sysID) {
			if (wrapper.currentNode) {
				return wrapper.currentNode->tcb.get();
			}
		}
	}

	/** Not Found */
	return nullptr;
}

void WThreadManager::yield(WThreadTCB* current, bool exit) {
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
			if (wrapper.thread.get_id() == sysID
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
		pTCBNode->tcb->statusOut(exit);
		pContext = &(pWrapper->context);

		/** Continue In List */
		if (exit) {
			this->tcbExitList.addLast(std::move(pTCBNode));
		}
		else {
			this->tcbWaitList.addLast(std::move(pTCBNode));
		}
	}

	/** Jump Out */
	current->jumpOut(*pContext, exit);
}

void WThreadManager::sysThreadRunner(SysWrapper& wrapper) {
	/** TODO Thread Dispatch */
}
