#pragma once

#include "ContextDef.h"
#include <stdint.h>

extern int wThreadGetContext(WThreadContext*);
extern void wThreadSetContext(const WThreadContext*);

extern void wThreadSwapContext(WThreadContext* oldContext, const WThreadContext* newContext);
extern void wThreadInitContext(WThreadContext* context,
	WThreadEntryFunc* entry, const WThreadStack* stack);

extern size_t wThreadCreateStack(WThreadStack* stack, size_t size);
extern void wThreadFreeStack(WThreadStack* stack);
