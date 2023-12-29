#include "Utils.hpp"

#include "TCB.hpp"
#include "Manager.hpp"

WThreadTCB* WThreadUtils::getCurrentTCB(WThreadManager& manager) {
	return manager.findCurrentRunningTCB();
}

void WThreadUtils::exit(WThreadTCB* tcb, int code) {
	/** Set Thread Exit Code */
	tcb->setExitCode(code);

	/** Exit Thread */
	tcb->getManager().yield(tcb, true);
}

void WThreadUtils::yield(WThreadTCB* tcb) {
	/** Should Exit */
	if (tcb->checkShouldExit()) {
		exit(tcb, -2);
		/** Never Returns */
	}

	/** Yield Thread */
	tcb->getManager().yield(tcb, false);
}
