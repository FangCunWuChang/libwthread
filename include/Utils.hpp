#pragma once

#include <cstdint>

class WThisThread;
class WThreadTCB;
class WThreadManager;

class WThreadUtils final {
	WThreadUtils() = delete;

	friend class WThreadTCB;
	friend class WThisThread;

	static WThreadTCB* getCurrentTCB(WThreadManager& manager);

	static void exit(WThreadTCB* tcb, int code);
	static void yield(WThreadTCB* tcb);
	static void wait(WThreadTCB* tcb, uint64_t msecs);

public:
	static uint64_t getCurrentTimeMSec();
};
