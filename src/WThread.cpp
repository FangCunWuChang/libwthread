#include "WThread.hpp"

#include "Manager.hpp"
#include "Utils.hpp"

WThreadHub::WThreadHub(int sysThreadNum) {
	this->impl = new WThreadManager{ sysThreadNum };
}

WThreadHub::~WThreadHub() {
	delete this->impl;
}

bool WThreadHub::isFinised() const {
	return this->getImpl().isFinished();
}

WThreadManager& WThreadHub::getImpl() const {
	return *(this->impl);
}

/** ================================================================== */

WThread::WThread(WThreadHub& hub, size_t stack)
	: hub(hub), stack(stack) {}

WThread::WThread(WThreadHub& hub, const Func& func, size_t stack)
	: hub(hub), stack(stack), func(func) {}

WThread::~WThread() {
	/** Exit Then Wait */
	if (this->isRunning()) {
		this->signalExit();
		this->waitForFinished();
	}
}

void WThread::start() {
	/** Make Func With run() */
	if (!this->func) {
		this->func = [this]()->int { return this->run(); };
	}

	/** Create Thread */
	this->handle = this->hub.getImpl()
		.add(this->func, this->stack);
}

int WThread::waitForFinished() const {
	while (this->isRunning()) {};
	return this->getExitCode();
}

void WThread::signalExit() {
	if (this->handle) {
		this->hub.getImpl().signalExit(this->handle);
	}
}

bool WThread::isRunning() const {
	if (this->handle) {
		return this->hub.getImpl().isRunning(this->handle);
	}
	return false;
}

bool WThread::shouldExit() const {
	if (this->handle) {
		return this->handle->checkShouldExit();
	}
	return false;
}

int WThread::getExitCode() const {
	if (this->handle) { return handle->getExitCode(); }
	return 0;
}

uint64_t WThread::getID() const {
	if (this->handle) { return handle->getID(); }
	return 0;
}

/** ========================================================================== */

void WThisThread::exit(WThreadHub& hub, int code) {
	if (auto tcb = WThreadUtils::getCurrentTCB(hub.getImpl())) {
		WThreadUtils::exit(tcb, code);
	}
}

void WThisThread::yeild(WThreadHub& hub) {
	if (auto tcb = WThreadUtils::getCurrentTCB(hub.getImpl())) {
		WThreadUtils::yield(tcb);
	}
}

void WThisThread::wait(WThreadHub& hub, uint64_t msecs) {
	if (auto tcb = WThreadUtils::getCurrentTCB(hub.getImpl())) {
		WThreadUtils::wait(tcb, msecs);
	}
}

bool WThisThread::shouldExit(WThreadHub& hub) {
	if (auto tcb = WThreadUtils::getCurrentTCB(hub.getImpl())) {
		return tcb->checkShouldExit();
	}
	return false;
}

uint64_t WThisThread::getID(WThreadHub& hub) {
	if (auto tcb = WThreadUtils::getCurrentTCB(hub.getImpl())) {
		return tcb->getID();
	}
	return 0;
}
