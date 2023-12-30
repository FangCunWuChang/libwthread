#pragma once

#include "TCBList.hpp"

#include <mutex>
#include <vector>

class WThreadUtils;
class WThread;

class WThreadManager final {
public:
	WThreadManager(int sysThreadNum = 5);
	~WThreadManager();

	bool isFinished() const;

private:
	friend class WThread;
	using Func = WThreadTCB::ThreadFunc;
	const WThreadTCB* add(const Func& func, size_t stackSize = 1024 * 1024);
	bool isRunning(const WThreadTCB* tcb) const;
	void signalExit(const WThreadTCB* tcb);

private:
	friend class WThreadUtils;
	enum class YieldType { Yield, Wait, Exit };
	WThreadTCB* findCurrentRunningTCB() const;
	void yield(WThreadTCB* current, YieldType type);

private:
	class SysWrapper final {
		friend class WThreadManager;

		std::unique_ptr<std::thread> thread = nullptr;
		std::unique_ptr<WThreadTCBNode> currentNode = nullptr;
		WThreadContext context = {};
		bool running = false;
	};

	mutable std::mutex mutex;
	std::vector<SysWrapper> sysThreadList;
	WThreadTCBList tcbWaitList;
	WThreadTCBList tcbExitList;
	std::atomic_int runningThreadsCount = 0;

	static void sysThreadRunner(WThreadManager&, SysWrapper&);
};
