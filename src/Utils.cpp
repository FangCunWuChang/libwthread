#include "Utils.hpp"

#include "TCB.hpp"
#include "Manager.hpp"

#include <chrono>

WThreadTCB* WThreadUtils::getCurrentTCB(WThreadManager& manager) {
	return manager.findCurrentRunningTCB();
}

uint64_t WThreadUtils::getCurrentTimeMSec() {
	return std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::system_clock::now().time_since_epoch()).count();
}

void WThreadUtils::exit(WThreadTCB* tcb, int code) {
	/** Set Thread Exit Code */
	tcb->setExitCode(code);

	/** Clear Exit Flag */
	tcb->clearShouldExit();

	/** Exit Thread */
	tcb->getManager().yield(tcb, WThreadManager::YieldType::Exit);
}

void WThreadUtils::yield(WThreadTCB* tcb) {
	/** Should Exit */
	if (tcb->checkShouldExit()) {
		WThreadUtils::exit(tcb, -2);
		/** Never Returns */
	}

	/** Yield Thread */
	tcb->getManager().yield(tcb, WThreadManager::YieldType::Yield);
}

void WThreadUtils::wait(WThreadTCB* tcb, uint64_t msecs) {
	/** Should Exit */
	if (tcb->checkShouldExit()) {
		WThreadUtils::exit(tcb, -2);
		/** Never Returns */
	}

	/** Set Thread Wake Time */
	uint64_t current = WThreadUtils::getCurrentTimeMSec();
	tcb->setWakeTime(current + msecs);

	/** Wait Thread */
	tcb->getManager().yield(tcb, WThreadManager::YieldType::Wait);
}
