#pragma once

extern "C" {
#include "ContextDef.h"
}

#include "Macros.hpp"

#include <functional>
#include <atomic>

class WThreadManager;

class WThreadTCB final {
public:
	using ThreadFunc = WTHREAD_FUNC;
	enum class Status {
		New, Ready, Running, Wait, Exit
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
	void clearShouldExit();
	bool checkShouldExit() const;

	void setExitCode(int code);
	int getExitCode() const;

	void setWakeTime(uint64_t time);
	uint64_t getWakeTime() const;

	void statusIn();
	void statusOut(bool exit = false);
	void statusWait();
	void jumpIn(WThreadContext& oldContext);
	void jumpOut(WThreadContext& newContext);

private:
	WThreadManager& manager;
	const ThreadFunc func;

	WThreadContext context;
	WThreadStack stack;

	std::atomic<Status> status = Status::New;
	std::atomic_bool shouldExit = false;
	std::atomic_int exitCode = -1;
	std::atomic_uint64_t wakeTime = 0;

	static void WTHREAD_ENTRY_CALL threadEntry(void* t);
};
