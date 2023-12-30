#include <WThread.hpp>

#include <iostream>
#include <array>
#include <memory>
#include <string>
#include <mutex>

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
			output("Start ", i, WThisThread::getID(hub), getSysThreadID());

			while (true) {
				if (WThisThread::shouldExit(hub)) {
					output("Exit  ", i, WThisThread::getID(hub), getSysThreadID());
					WThisThread::exit(hub, i * 2);
				}
			};

			return 0;
			};
		threads[i] = std::make_unique<WThread>(hub, func, 1024 * 1024 * 10);
	}

	/** Start Threads */
	for (int i = 0; i < threads.size(); i++) {
		threads[i]->start();
	}

	/** Signal Exit */
	for (int i = 0; i < threads.size(); i++) {
		threads[i]->signalExit();
	}

	/** Wait For Finished */
	while (!hub.isFinised()) {};

	/** Get Return Value */
	int retValue = 0;
	for (int i = 0; i < threads.size(); i++) {
		retValue += threads[i]->getExitCode();
	}

	return !(retValue == 9900);
}
