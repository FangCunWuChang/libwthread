#include "TCB.hpp"

extern "C" {
#include "Context.h"
}

#include "Utils.hpp"

WThreadTCB::WThreadTCB(WThreadManager& manager,
	const ThreadFunc& func, size_t stackSize)
	: manager(manager), func(func) {
	/** Create Stack */
	wThreadCreateStack(&(this->stack), stackSize);

	/** Init Context */
	wThreadInitContext(&(this->context), WThreadTCB::threadEntry, this, &(this->stack));
}

WThreadTCB::~WThreadTCB() {
	/** Clear Stack */
	wThreadFreeStack(&(this->stack));
}

WThreadTCB::Status WThreadTCB::getStatus() const {
	return this->status;
}

uint64_t WThreadTCB::getID() const {
	return (uint64_t)this;
}

WThreadManager& WThreadTCB::getManager() const {
	return this->manager;
}

void WThreadTCB::sendShouldExit() {
	this->shouldExit = true;
}

bool WThreadTCB::checkShouldExit() const {
	return this->shouldExit;
}

void WThreadTCB::setExitCode(int code) {
	this->exitCode = code;
}

int WThreadTCB::getExitCode() const {
	return this->exitCode;
}

void WThreadTCB::statusIn() {
	this->status = Status::Running;
}

void WThreadTCB::statusOut(bool exit) {
	this->status = exit ? Status::Exit : Status::Ready;
}

void WThreadTCB::jumpIn(WThreadContext& oldContext) {
	/** Clear Should Exit Flag */
	this->shouldExit = false;

	/** Swap Context To Jump In */
	wThreadSwapContext(&oldContext, &(this->context));
}

void WThreadTCB::jumpOut(WThreadContext& newContext, bool exit) {
	/** Swap Context To Jump Out */
	wThreadSwapContext(&(this->context), &newContext);
}

void WTHREAD_ENTRY_CALL WThreadTCB::threadEntry(void* t) {
	/** Get TCB */
	auto tcb = reinterpret_cast<WThreadTCB*>(t);
	if (!tcb) { return; }

	/** Invoke Func */
	int ret = tcb->func();

	/** Exit */
	WThreadUtils::exit(tcb, ret);
}
