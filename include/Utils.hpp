#pragma once

class WThreadTCB;
class WThreadManager;

class WThreadUtils final {
	WThreadUtils() = delete;

	friend class WThreadTCB;

	static WThreadTCB* getCurrentTCB(WThreadManager& manager);

	static void exit(WThreadTCB* tcb, int code);
	static void yield(WThreadTCB* tcb);
};
