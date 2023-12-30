#include "TCBList.hpp"

WThreadTCBList::~WThreadTCBList() {
	this->clear();
}

void WThreadTCBList::addLast(WThreadManager& manager,
	const WThreadTCB::ThreadFunc& func, size_t stackSize) {
	this->addLastInternal(new WThreadTCBNode{ new WThreadTCB{manager, func, stackSize} });
};
void WThreadTCBList::addLast(std::unique_ptr<WThreadTCBNode> node) {
	this->addLastInternal(node.release());
};

std::unique_ptr<WThreadTCBNode> WThreadTCBList::getFirst() {
	if (!(this->root)) { return nullptr; }

	auto node = this->root;
	this->root = node->next;
	node->next = nullptr;

	if (!(this->root)) { this->last = nullptr; }

	return std::unique_ptr<WThreadTCBNode>(node);
};

std::unique_ptr<WThreadTCBNode> WThreadTCBList::getId(uint64_t id) {
	/** Check First */
	if (this->root && (this->root->tcb->getID() == id)) {
		return this->getFirst();
	}

	/** Check Other */
	for (auto p = this->root; p; p = p->next) {
		if (p->next && (p->next->tcb->getID() == id)) {
			auto node = p->next;
			p->next = node->next;
			node->next = nullptr;

			if (!(p->next)) {
				this->last = p;
			}

			return std::unique_ptr<WThreadTCBNode>(node);
		}
	}

	/** Not Found */
	return nullptr;
}

bool WThreadTCBList::empty() const {
	return !(this->root);
}

bool WThreadTCBList::contains(const WThreadTCB* tcb) const {
	for (auto p = this->root; p; p = p->next) {
		if (p->tcb.get() == tcb) {
			return tcb;
		}
	}
	return false;
}

void WThreadTCBList::clear() {
	while (this->root) {
		auto ptr = this->getFirst();
	}
}

void WThreadTCBList::addLastInternal(WThreadTCBNode* node) {
	if (this->last) {
		this->last->next = node;
	}

	this->last = node;
	node->next = nullptr;

	if (!(this->root)) {
		this->root = node;
	}
};
