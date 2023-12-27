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

    mov		rsp, qword ptr [rcx + 184]
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

    mov		rdi, qword ptr [rsp]            ;rip := ret addr
    mov		qword ptr [rcx + 160], rdi
    lea		rdi, qword ptr [rsp + 8]		;rsp
    mov		qword ptr [rcx + 184], rdi
    
    mov		rdi, qword ptr [rcx + 8]		;restore rdi
    mov		rax, 0                          ;ret value := 0
    ret
wThreadGetContext ENDP

END