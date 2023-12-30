#include <WThread.hpp>

#include <iostream>
#include <array>
#include <memory>
#include <atomic>
#include <string>
#include <mutex>

std::atomic_int testValue = 0;

static void output(std::string s, int i, uint64_t tid, uint64_t sysid) {
	/** Lock */
	static std::mutex outputLock;
	std::lock_guard<std::mutex> locker(outputLock);

	/** Output */
	std::cout << s << " i: " << i << " TID: " << tid << " SYSID: " << sysid << std::endl;
}

static uint64_t getSysThreadID() {
	std::thread::id id = std::this_thread::get_id();
	return std::hash<std::thread::id>()(id);
}

int main(int argc, char* argv[]) {
	/** Thread Hub With 8 System Threads */
	WThreadHub hub(8);

	/** Create Threads */
	std::array<std::unique_ptr<WThread>, 100> threads;
	for (int i = 0; i < threads.size(); i++) {
		auto func = [i, &hub] {
			int m = 0;

			output("Start   ", i, WThisThread::getID(hub), getSysThreadID());
			testValue += (i * (++m));

			WThisThread::wait(hub, (uint64_t)10 * i);

			output("Wake 0  ", i, WThisThread::getID(hub), getSysThreadID());
			testValue += (i * (++m));

			WThisThread::yeild(hub);

			output("Resume 0", i, WThisThread::getID(hub), getSysThreadID());
			testValue += (i * (++m));

			WThisThread::wait(hub, (uint64_t)20 * i);

			output("Wake 1  ", i, WThisThread::getID(hub), getSysThreadID());
			testValue += (i * (++m));

			WThisThread::exit(hub, i * (++m));

			output("Exit    ", i, WThisThread::getID(hub), getSysThreadID());
			testValue += (i * (++m));

			return 0;
			};
		threads[i] = std::make_unique<WThread>(hub, func, 1024 * 1024 * 10);
	}

	/** Start Threads */
	for (int i = 0; i < threads.size(); i++) {
		threads[i]->start();
	}

	/** Wait For Finished */
	while (!hub.isFinised()) {};

	/** Get Return Value */
	int retValue = 0;
	for (int i = 0; i < threads.size(); i++) {
		retValue += threads[i]->getExitCode();
	}

	return !((testValue == 49500) && (retValue == 24750));
}
