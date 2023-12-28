#include "Context.h"

#include <memory.h>
#include <malloc.h>

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
	stack->sp = malloc(size);
	stack->size = size;
	return size;
}

void wThreadFreeStack(WThreadStack* stack) {
	if (stack->sp != NULL) {
		free(stack->sp);
		stack->sp = NULL;
		stack->size = 0;
	}
}
