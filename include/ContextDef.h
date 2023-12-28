#pragma once

#include <stdint.h>
#include <xmmintrin.h>

typedef struct {
	uint64_t onstack;	/**< +0 */
	uint64_t rdi;		/**< +8 */
	uint64_t rsi;		/**< +16 */
	uint64_t rdx;		/**< +24 */
	uint64_t rcx;		/**< +32 */
	uint64_t r8;		/**< +40 */
	uint64_t r9;		/**< +48 */
	uint64_t rax;		/**< +56 */
	uint64_t rbx;		/**< +64 */
	uint64_t rbp;		/**< +72 */
	uint64_t r10;		/**< +80 */
	uint64_t r11;		/**< +88 */
	uint64_t r12;		/**< +96 */
	uint64_t r13;		/**< +104 */
	uint64_t r14;		/**< +112 */
	uint64_t r15;		/**< +120 */
	uint64_t trapno;	/**< +128 */
	uint64_t addr;		/**< +136 */
	uint64_t flags;		/**< +144 */
	uint64_t err;		/**< +152 */
	uint64_t rip;		/**< +160 */
	uint64_t cs;		/**< +168 */
	uint64_t rflags;	/**< +176 */
	uint64_t rsp;		/**< +184 */
	uint64_t ss;		/**< +192 */

	/** xmm * 16 */
	uint64_t xmm[32];	/**< +200 ~ +240 */
} WThreadContext;

typedef struct {
	void* sp;
	size_t size;
} WThreadStack;

typedef void(WThreadEntryFunc)(void);
