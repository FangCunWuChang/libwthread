﻿#pragma once

#include <stdint.h>

typedef struct {
	uint64_t onstack;
	uint64_t rdi;
	uint64_t rsi;
	uint64_t rdx;
	uint64_t rcx;
	uint64_t r8;
	uint64_t r9;
	uint64_t rax;
	uint64_t rbx;
	uint64_t rbp;
	uint64_t r10;
	uint64_t r11;
	uint64_t r12;
	uint64_t r13;
	uint64_t r14;
	uint64_t r15;
	uint64_t trapno;
	uint64_t addr;
	uint64_t flags;
	uint64_t err;
	uint64_t rip;
	uint64_t cs;
	uint64_t rflags;
	uint64_t rsp;
	uint64_t ss;

	uint64_t fpcsr;
	uint64_t mxcsr;
	uint64_t xmm[32];
	uint64_t ymm[32];
	uint64_t zmm[32];
} WThreadContext;

typedef struct {
	void* sp;
	size_t size;
} WThreadStack;

typedef void(WThreadEntryFunc)(void);
