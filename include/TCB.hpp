#pragma once

extern "C" {
#include "ContextDef.h"
}

#include <functional>
#include <atomic>

class WThreadManager;

class WThreadTCB final {
public:
	using ThreadFunc = std::function<int(void)>;
	enum class Status {
		New, Ready, Running, Exit
	};

public:
	WThreadTCB(WThreadManager& manager, const ThreadFunc& func, size_t stackSize);
	~WThreadTCB();

	WThreadTCB(const WThreadTCB&) = delete;
	WThreadTCB& operator=(const WThreadTCB&) = delete;

	Status getStatus() const;
	uint64_t getID() const;
	WThreadManager& getManager() const;

	void sendShouldExit();
	bool checkShouldExit() const;

	void setExitCode(int code);
	int getExitCode() const;

	void statusIn();
	void statusOut(bool exit = false);
	void jumpIn(WThreadContext& oldContext);
	void jumpOut(WThreadContext& newContext, bool exit = false);

private:
	WThreadManager& manager;
	const ThreadFunc func;

	WThreadContext context;
	WThreadStack stack;

	std::atomic<Status> status = Status::New;
	std::atomic_bool shouldExit = false;
	std::atomic_int exitCode = -1;

	static void WTHREAD_ENTRY_CALL threadEntry(void* t);
};
