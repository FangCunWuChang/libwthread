#pragma once

#include "TCBList.hpp"

#include <mutex>
#include <vector>

class WThreadUtils;

class WThreadManager final {
public:
	WThreadManager(int sysThreadNum = 5);
	~WThreadManager();

	bool isFinished() const;

private:
	friend class WThreadUtils;
	WThreadTCB* findCurrentRunningTCB() const;
	void yield(WThreadTCB* current, bool exit);

private:
	class SysWrapper final {
		friend class WThreadManager;

		std::thread thread;
		std::unique_ptr<WThreadTCBNode> currentNode = nullptr;
		WThreadContext context;
	};

	mutable std::mutex mutex;
	std::vector<SysWrapper> sysThreadList;
	WThreadTCBList tcbWaitList;
	WThreadTCBList tcbExitList;

	static void sysThreadRunner(SysWrapper&);
};
