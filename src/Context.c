#include "Context.h"

#include <memory.h>
#include <malloc.h>

#define STACK_SAFETY_BLOCK_SIZE 16

int wThreadSwapContext(WThreadContext* oldContext, const WThreadContext* newContext) {
	if (wThreadGetContext(oldContext) == 0) {
		wThreadSetContext(newContext);
	}
	return 0;
}

void wThreadInitContext(WThreadContext* context,
	WThreadEntryFunc* entry, const WThreadStack* stack) {
	/** Clear Context */
	memset(context, 0, sizeof(WThreadContext));

	/** Set Stack */
	uint64_t* sp = (uint64_t)(stack->sp) + stack->size;
	sp = (void*)((uint64_t)sp - (uint64_t)sp % 16);	/**< 16 Bytes Align */
	context->rsp = (uint64_t)sp;

	/** Set Thread Entry */
	context->rip = (uint64_t)entry;
}

int wThreadCreateStack(WThreadStack* stack, size_t size) {
	char* sPtr = malloc(size + STACK_SAFETY_BLOCK_SIZE * 2);
	if (sPtr != NULL) {
		stack->sp = sPtr + STACK_SAFETY_BLOCK_SIZE;
		stack->size = size;
		return size + STACK_SAFETY_BLOCK_SIZE;
	}
	return 0;
}

void wThreadFreeStack(WThreadStack* stack) {
	if (stack->sp != NULL) {
		free((char*)(stack->sp) - STACK_SAFETY_BLOCK_SIZE);
		stack->sp = NULL;
		stack->size = 0;
	}
}
