#pragma once

#include "ContextDef.h"

extern int wThreadGetContext(WThreadContext*);
extern void wThreadSetContext(const WThreadContext*);

extern int wThreadSwapContext(WThreadContext* oldContext, const WThreadContext* newContext);
extern void wThreadInitContext(WThreadContext* context,
	WThreadEntryFunc* entry, const WThreadStack* stack);

extern int wThreadCreateStack(WThreadStack* stack, size_t size);
extern void wThreadFreeStack(WThreadStack* stack);
