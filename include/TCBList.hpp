#pragma once

#include "TCB.hpp"

#include <memory>

class WThreadManager;

class WThreadTCBNode final {
public:
	WThreadTCBNode(WThreadTCB* tcb) : tcb(tcb) {};

	friend class WThreadTCBList;
	std::unique_ptr<WThreadTCB> tcb = nullptr;
	WThreadTCBNode* next = nullptr;
};

class WThreadTCBList final {
public:
	WThreadTCBList() = default;
	WThreadTCBList(const WThreadTCBList&) = delete;
	WThreadTCBList& operator=(const WThreadTCBList&) = delete;

	~WThreadTCBList();

	void addLast(WThreadManager& manager,
		const WThreadTCB::ThreadFunc& func, size_t stackSize);
	void addLast(std::unique_ptr<WThreadTCBNode> node);

	std::unique_ptr<WThreadTCBNode> getFirst();

	std::unique_ptr<WThreadTCBNode> getId(uint64_t id);

	bool empty() const;
	void clear();

private:
	WThreadTCBNode* root = nullptr;
	WThreadTCBNode* last = nullptr;

	void addLastInternal(WThreadTCBNode* node);
};
