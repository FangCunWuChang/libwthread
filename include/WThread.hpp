#pragma once

#include "Macros.hpp"

#include <functional>

class WThreadManager;
class WThreadTCB;

using WThreadFunc = WTHREAD_FUNC;
template class WTHREAD_API WTHREAD_FUNC;

class WTHREAD_API WThreadHub final {
public:
	WThreadHub(int sysThreadNum = 5);
	~WThreadHub();

	WThreadHub(const WThreadHub&) = delete;
	WThreadHub& operator=(const WThreadHub&) = delete;

	bool isFinised() const;

private:
	friend class WThread;
	friend class WThisThread;
	WThreadManager& getImpl() const;
	WThreadManager* impl = nullptr;
};

class WTHREAD_API WThread {
public:
	using Func = WThreadFunc;
	WThread(WThreadHub& hub, size_t stack = 1024 * 1024);
	WThread(WThreadHub& hub, const Func& func, size_t stack = 1024 * 1024);
	virtual ~WThread();

	void start();
	int waitForFinished() const;
	void signalExit();

	bool isRunning() const;
	bool shouldExit() const;
	int getExitCode() const;
	uint64_t getID() const;

protected:
	virtual int run() { return 0; };

private:
	WThreadHub& hub;
	Func func;
	const size_t stack;

	const WThreadTCB* handle = nullptr;
};

class WTHREAD_API WThisThread final {
	WThisThread() = delete;

public:
	static void exit(WThreadHub& hub, int code);
	static void yeild(WThreadHub& hub);
	static void wait(WThreadHub& hub, uint64_t msecs);
	static bool shouldExit(WThreadHub& hub);
	static uint64_t getID(WThreadHub& hub);
};
