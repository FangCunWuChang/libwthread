#include "Context.h"

#include <memory.h>
#include <malloc.h>

/** Alloc Enough Stack Size */
#define STACK_SIZE_MIN 1024 * 10

/** Safety Block At Stack Bottom */
#define STACK_SAFETY_BLOCK_SIZE 128

void wThreadSwapContext(WThreadContext* oldContext, const WThreadContext* newContext) {
	if (wThreadGetContext(oldContext) == 0) {
		wThreadSetContext(newContext);
		/** Never Returns */
	}
}

void wThreadInitContext(WThreadContext* context,
	WThreadEntryFunc* entry, void* startArg, const WThreadStack* stack) {
	/** Clear Context */
	memset(context, 0, sizeof(WThreadContext));

	/** Set Stack */
	uint64_t* sp = (uint64_t*)((uint64_t)(stack->sp) + stack->size);
	sp = (void*)((uint64_t)sp - (uint64_t)sp % 16);	/**< 16 Bytes Align */
	context->rsp = (uint64_t)sp;

	/** Set Arg */
	context->rcx = (uint64_t)startArg;

	/** Set Thread Entry */
	context->rip = (uint64_t)entry;
}

size_t wThreadCreateStack(WThreadStack* stack, size_t size) {
	if (size < STACK_SIZE_MIN) {
		size = STACK_SIZE_MIN;
	}

	stack->sp = malloc(size + STACK_SAFETY_BLOCK_SIZE);
	if (stack->sp != NULL) {
		stack->size = size;
		return size;
	}
	return 0;
}

void wThreadFreeStack(WThreadStack* stack) {
	if (stack->sp != NULL) {
		free(stack->sp);
		stack->sp = NULL;
		stack->size = 0;
	}
}
