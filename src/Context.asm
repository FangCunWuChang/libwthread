.CODE
wThreadSetContext PROC
    mov		rdi, qword ptr [rcx + 8]
    mov		rsi, qword ptr [rcx + 16]
    mov		rdx, qword ptr [rcx + 24]
    mov		r8, qword ptr [rcx + 40]
    mov		r9, qword ptr [rcx + 48]
    mov		rax, qword ptr [rcx + 56]
    mov		rbx, qword ptr [rcx + 64]
    mov		rbp, qword ptr [rcx + 72]
    mov		r10, qword ptr [rcx + 80]
    mov		r11, qword ptr [rcx + 88]
    mov		r12, qword ptr [rcx + 96]
    mov		r13, qword ptr [rcx + 104]
    mov		r14, qword ptr [rcx + 112]
    mov		r15, qword ptr [rcx + 120]

    push    qword ptr [rcx + 176]
    popfq                                   ;rflags

    movups  xmm0, xmmword ptr [rcx + 200 + 16 * 0]
    movups  xmm1, xmmword ptr [rcx + 200 + 16 * 1]
    movups  xmm2, xmmword ptr [rcx + 200 + 16 * 2]
    movups  xmm3, xmmword ptr [rcx + 200 + 16 * 3]
    movups  xmm4, xmmword ptr [rcx + 200 + 16 * 4]
    movups  xmm5, xmmword ptr [rcx + 200 + 16 * 5]
    movups  xmm6, xmmword ptr [rcx + 200 + 16 * 6]
    movups  xmm7, xmmword ptr [rcx + 200 + 16 * 7]
    movups  xmm8, xmmword ptr [rcx + 200 + 16 * 8]
    movups  xmm9, xmmword ptr [rcx + 200 + 16 * 9]
    movups  xmm10, xmmword ptr [rcx + 200 + 16 * 10]
    movups  xmm11, xmmword ptr [rcx + 200 + 16 * 11]
    movups  xmm12, xmmword ptr [rcx + 200 + 16 * 12]
    movups  xmm13, xmmword ptr [rcx + 200 + 16 * 13]
    movups  xmm14, xmmword ptr [rcx + 200 + 16 * 14]
    movups  xmm15, xmmword ptr [rcx + 200 + 16 * 15]

    mov		rsp, qword ptr [rcx + 184]
    add     rsp, 8
    push	qword ptr [rcx + 160]           ;ret addr := new rip

    mov		rcx, qword ptr [rcx + 32]       ;restore rcx
    mov		rax, 1                          ;ret value := 1
    ret
wThreadSetContext ENDP

wThreadGetContext PROC
    mov		qword ptr [rcx + 8], rdi
    mov		qword ptr [rcx + 16], rsi
    mov		qword ptr [rcx + 24], rdx
    mov		qword ptr [rcx + 32], rcx
    mov		qword ptr [rcx + 40], r8
    mov		qword ptr [rcx + 48], r9
    mov		qword ptr [rcx + 56], rax
    mov		qword ptr [rcx + 64], rbx
    mov		qword ptr [rcx + 72], rbp
    mov		qword ptr [rcx + 80], r10
    mov		qword ptr [rcx + 88], r11
    mov		qword ptr [rcx + 96], r12
    mov		qword ptr [rcx + 104], r13
    mov		qword ptr [rcx + 112], r14
    mov		qword ptr [rcx + 120], r15

    pushfq
    pop     qword ptr [rcx + 176]           ;rflags

    movups  xmmword ptr [rcx + 200 + 16 * 0], xmm0
    movups  xmmword ptr [rcx + 200 + 16 * 1], xmm1
    movups  xmmword ptr [rcx + 200 + 16 * 2], xmm2
    movups  xmmword ptr [rcx + 200 + 16 * 3], xmm3
    movups  xmmword ptr [rcx + 200 + 16 * 4], xmm4
    movups  xmmword ptr [rcx + 200 + 16 * 5], xmm5
    movups  xmmword ptr [rcx + 200 + 16 * 6], xmm6
    movups  xmmword ptr [rcx + 200 + 16 * 7], xmm7
    movups  xmmword ptr [rcx + 200 + 16 * 8], xmm8
    movups  xmmword ptr [rcx + 200 + 16 * 9], xmm9
    movups  xmmword ptr [rcx + 200 + 16 * 10], xmm10
    movups  xmmword ptr [rcx + 200 + 16 * 11], xmm11
    movups  xmmword ptr [rcx + 200 + 16 * 12], xmm12
    movups  xmmword ptr [rcx + 200 + 16 * 13], xmm13
    movups  xmmword ptr [rcx + 200 + 16 * 14], xmm14
    movups  xmmword ptr [rcx + 200 + 16 * 15], xmm15

    mov		rdi, qword ptr [rsp]            ;rip := ret addr
    mov		qword ptr [rcx + 160], rdi
    lea		rdi, qword ptr [rsp]		    ;rsp
    mov		qword ptr [rcx + 184], rdi
    
    mov		rdi, qword ptr [rcx + 8]		;restore rdi
    mov		rax, 0                          ;ret value := 0
    ret
wThreadGetContext ENDP
END