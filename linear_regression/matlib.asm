[BITS 64]


global m_min
global m_max
global m_mean
global m_var
global m_std
global m_scale_min_max
global m_col_bckw_copy
global m_fill_dword
global m_mul_sum
global m_sst
global m_rss

section .text



; rdi - pointer to float array, rsi - n
m_min:
	mov rax, rsi
	and rax, 0x7
	shr rsi, 3

	vmovups ymm0, [rdi]
	add rdi, rax

	._loop:
		vmovups ymm1, [rdi]
		vminps ymm0, ymm1
		add rdi, 32
		dec rsi
	jnz ._loop

	sub rsp, 32
	vmovups [rsp], ymm0

	mov rax, 32
	movss xmm0, dword [rsp]
	._lastloop:
		movss xmm1, dword [rsp+rax-4]
		minss xmm0, xmm1
		sub rax, 8
	jnz ._lastloop
	add rsp, 32
ret


m_max:
	mov rax, rsi
	and rax, 0x7
	shr rsi, 3

	vmovups ymm0, [rdi]
	add rdi, rax

	._loop:
		vmovups ymm1, [rdi]
		vmaxps ymm0, ymm1
		add rdi, 32
		dec rsi
	jnz ._loop

	sub rsp, 32
	vmovups [rsp], ymm0

	mov rax, 32
	movss xmm0, dword [rsp]
	._lastloop:
		movss xmm1, dword [rsp+rax-4]
		maxss xmm0, xmm1
		sub rax, 8
	jnz ._lastloop
	add rsp, 32
ret

; rdi - array address, rsi - n
m_mean:
	mov rax, rsi
	finit 
	fld dword [rdi]
	add rdi, 4
	dec rsi
	._loop:
		fld dword [rdi]
		faddp st1, st0
		add rdi, 4
		dec rsi
	jnz ._loop
	sub rsp, 8
	mov qword [rsp], rax
	fidiv dword [rsp]
	add rsp, 8
	;dw 0x3adb
	;fstp tword [rdx]
ret

; void m_var(float*, uint64_t, float, float*, long double*)
m_var: ; rdi - array, rsi - n, rdx - ptr to mean, rcx - pointer to new array, r8 
	mov r9, rsi
	mov r8, rcx
	mov rax, rsi
	shr rsi, 3
	and rax, 0x7

	;mov edx, dword [rdx]
	sub rsp, 4
	fld tword [rdx]
	fstp dword [rsp]
	mov edx, dword [rsp]
	add rsp, 4

	movd xmm0, edx
	vbroadcastss ymm0, xmm0
	._loop:
		vmovups ymm1, [rdi]
		vsubps ymm1, ymm0
		vmulps ymm1, ymm1
		vmovups [rcx], ymm1
		add rdi, 32
		add rcx, 32
		dec rsi
	jnz ._loop

	test rax, rax
	jz .skip

	._sloop:
		movss xmm1, [rdi]
		subss xmm1, xmm0
		mulss xmm1, xmm1
		movss [rcx], xmm1
		add rdi, 4
		add rcx, 4
		dec rax
	jnz ._sloop

	.skip:
	mov rdi, r8
	mov rsi, r9
jmp m_mean

; rdi - long double* variance
m_std:
	fld tword [rdi]
	fsqrt
ret

; rdi - array, rsi - n
m_scale_min_max: 
	push rdi 
	push rsi
	call m_min
	movd eax, xmm0
	push rax
	mov rdi, qword [rsp+16]
	mov rsi, qword [rsp+8]
	call m_max
	pop rax
	movd xmm1, eax ; xmm1 - min, xmm0 - max
	pop rsi
	pop rdi
	subss xmm0, xmm1
	vbroadcastss ymm0, xmm0 ; ymm0 = (max - min)(ps)
	vbroadcastss ymm1, xmm1 ; ymm1 = min(ps)

	mov rax, rsi
	and rax, 0x7
	shr rsi, 3

	._loop:
		vmovups ymm2, [rdi]
		vsubps ymm2, ymm1
		vdivps ymm2, ymm0
		vmovups [rdi], ymm2
		add rdi, 32
		dec rsi
	jnz ._loop

	test rax, rax
	jz .skip

	._loop2:
		movss xmm2, [rdi]
		subss xmm2, xmm1
		divss xmm2, xmm0
		add rdi, 4
		dec rax
	jnz ._loop2

	.skip:
ret

; rdi - dest end, rsi - src end, rdx - unused, rcx - amount
m_col_bckw_copy:
	std
	shl rcx, 1
	rep movsq
ret

; rdi - dest, rsi - value, rdx - unused, rcx - amount, 
m_fill_dword:
	mov rax, rsi
	cld
	rep stosd
ret


; rdi - array1, rsi - arry2, rdx - n
m_mul_sum:
	mov rax, rdx
	and rax, 0x7
	shr rdx, 3

	vpxor ymm0, ymm0
	._loop:
		vmovups ymm1, [rdi]
		vmovups ymm2, [rsi]
		vmulps ymm1, ymm2 
		vaddps ymm0, ymm1
		add rdi, 32
		add rsi, 32
		dec rdx
	jnz ._loop

	test rax, rax
	jz .skip

	._loop2:
		movss xmm1, [rdi]
		movss xmm2, [rsi]
		mulss xmm1, xmm2
		addss xmm0, xmm1
		add rdi, 4
		add rsi, 4
		dec rax
	jnz ._loop2

	.skip:
	vpxor ymm1, ymm1
	vhaddps ymm0, ymm1
	vhaddps ymm0, ymm1

	vextractf128 xmm1, ymm0, 1	
	addss xmm0, xmm1
ret

; rdi - Y_test, rsi - answer, rdx - length
m_rss:	
	mov rax, rdx
	shr rdx, 3
	and rax, 0x7

	vpxor ymm0, ymm0
	._loop:
		vmovups ymm1, [rdi]
		vmovups ymm2, [rsi]
		vsubps ymm1, ymm2
		vmulps ymm1, ymm1
		vaddps ymm0, ymm1
		add rdi, 32
		add rsi, 32
		dec rdx
	jnz ._loop

	test rax, rax
	jz .skip

	._loop2:
		movss xmm1, [rdi]
		movss xmm2, [rsi]
		subss xmm1, xmm2
		mulss xmm1, xmm1
		addss xmm0, xmm1
		add rdi, 4
		add rsi, 4
		dec rax
	jnz ._loop2

	.skip:
	vpxor ymm1, ymm1
	vhaddps ymm0, ymm1
	vhaddps ymm0, ymm1

	vextractf128 xmm1, ymm0, 1	
	addss xmm0, xmm1
ret

; rdi - Y_test, rsi - length, xmm0 - mean
m_sst:
	mov rax, rsi
	shr rsi, 3
	and rax, 0x7

	vbroadcastss ymm0, xmm0
	vpxor ymm1, ymm1
	._loop:
		vmovups ymm2, [rdi]
		vsubps ymm2, ymm0
		vmulps ymm2, ymm2
		vaddps ymm1, ymm2
		add rdi, 32
		dec rsi
	jnz ._loop

	test rax, rax
	jz .skip

	._loop2:
		movss xmm2, [rdi]
		subss xmm2, xmm0
		mulss xmm2, xmm2
		addss xmm1, xmm2
		add rdi, 4
		dec rax
	jnz ._loop2

	.skip:
	vpxor ymm0, ymm0
	vhaddps ymm1, ymm0
	vhaddps ymm1, ymm0

	vextractf128 xmm0, ymm1, 1	
	addss xmm0, xmm1
ret




