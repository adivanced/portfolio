[BITS 64]

global m_min
global m_max
global m_scale_min_max
global m_sigmoid
global m_sub_arr
global m_sum
global m_dot
global m_dot_w_add
global m_transp_matr_from_col
global m_dot_w_mul
global m_sub_arr_mul_ss

section .text



; rdi - pointer to float array, rsi - n
m_min:
	mov rax, rsi
	and rax, 0x7
	shr rsi, 3

	vmovups ymm0, [rdi]
	lea rdi, [rdi+rax*4]

	._loop:
		vmovups ymm1, [rdi]
		vminps ymm0, ymm1
		add rdi, 32
		dec rsi
	jnz ._loop

	vextractf128 xmm1, ymm0, 1
	vminps xmm0, xmm1
	vmovhlps xmm1, xmm0
	vminps xmm0, xmm1
	vpextrd eax, xmm0, 1
	vmovd xmm1, eax
	vminss xmm0, xmm1
ret


m_max:
	mov rax, rsi
	and rax, 0x7
	shr rsi, 3

	vmovups ymm0, [rdi]
	lea rdi, [rdi+rax*4]

	._loop:
		vmovups ymm1, [rdi]
		vmaxps ymm0, ymm1
		add rdi, 32
		dec rsi
	jnz ._loop

	vextractf128 xmm1, ymm0, 1
	vmaxps xmm0, xmm1
	vmovhlps xmm1, xmm0
	vmaxps xmm0, xmm1
	vpextrd eax, xmm0, 1
	vmovd xmm1, eax
	vmaxss xmm0, xmm1
ret


; rdi - array, rsi - n
m_scale_min_max: 
	push rdi 
	push rsi
	call m_min
	vmovd eax, xmm0
	push rax
	mov rdi, qword [rsp+16]
	mov rsi, qword [rsp+8]
	call m_max
	pop rax
	vmovd xmm1, eax ; xmm1 - min, xmm0 - max
	pop rsi
	pop rdi
	vsubss xmm0, xmm1
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
		vmovss xmm2, [rdi]
		vsubss xmm2, xmm1
		vdivss xmm2, xmm0
		vmovss [rdi], xmm2
		add rdi, 4
		dec rax
	jnz ._loop2

	.skip:
ret

; rdi - dest, rsi - matr, rdx - row, rcx - n_columns, r8 - n_rows, xmm0 - add
m_dot_w_add:
	._loop1:
		mov rax, rcx
		mov r10, rcx
		shr r10, 3
		and rax, 0x7

		mov r11, rdx

		vpxor ymm3, ymm3

		test r10, r10
		jz .skip1

		._loop2:
			vmovups ymm1, [rsi]
			vmovups ymm2, [r11]
			vmulps ymm1, ymm2
			vaddps ymm3, ymm1
			add r11, 32
			add rsi, 32
			dec r10
		jnz ._loop2

		vextractf128 xmm1, ymm3, 1
		vaddps xmm3, xmm1
		vhaddps xmm3, xmm3
		vhaddps xmm3, xmm3


		.skip1:
		test rax, rax
		jz .skip2

		._loop3:
			vmovss xmm1, [rsi]
			vmovss xmm2, [r11]
			vmulss xmm1, xmm2
			vaddss xmm3, xmm1
			add r11, 4
			add rsi, 4
			dec rax
		jnz ._loop3

		.skip2:
		vaddss xmm3, xmm0
		vmovss [rdi], xmm3
		add rdi, 4
		dec r8
	jnz ._loop1
ret




;rdi - float*, rsi - n
m_sigmoid:

	mov eax, 0x4B38AA3B ; 12102203.0f 
	vmovd xmm0, eax

	mov eax, 0xff800000 ; mask
	vmovd xmm1, eax

	mov eax, 0x34000000 ; 1.1920929e-7f
	vmovd xmm2, eax

	mov eax, 0x3EACA418 ; 0.3371894346f
	vmovd xmm3, eax

	mov eax, 0x3F285ADA ; 0.657636276f
	vmovd xmm4, eax

	mov eax, 0x3F803884 ; 1.00172476f
	vmovd xmm5, eax

	mov eax, 0x3F800000 ; 1.0f
	vmovd xmm6, eax

	mov eax, 0xBF800000 ; -1.0f
	vmovd xmm7, eax


	mov rax, rsi
	shr rsi, 3
	and rax, 0x7

	test rsi, rsi
	jz .skip1

	vbroadcastss ymm0, xmm0
	vbroadcastss ymm1, xmm1
	vbroadcastss ymm2, xmm2
	vbroadcastss ymm3, xmm3
	vbroadcastss ymm4, xmm4
	vbroadcastss ymm5, xmm5
	vbroadcastss ymm6, xmm6
	vbroadcastss ymm7, xmm7
	vpxor ymm11, ymm11

	._loop1:
		vmovups ymm8, [rdi]

		vmulps ymm8, ymm7

		vmulps ymm8, ymm0
		vcvtps2dq ymm8, ymm8
		vpand ymm9, ymm8, ymm1
		vpsubd ymm8, ymm9
		vcvtdq2ps ymm8, ymm8 ; exp
		vmulps ymm8, ymm2
		vmovups ymm10, ymm8
		vmulps ymm8, ymm3
		vaddps ymm8, ymm4
		vmulps ymm8, ymm10
		vaddps ymm8, ymm5
		vpaddd ymm8, ymm9

		vmaxps ymm8, ymm11
		vaddps ymm8, ymm6

		vdivps ymm8, ymm6, ymm8

		vmovups [rdi], ymm8
		add rdi, 32
		dec rsi
	jnz ._loop1

	.skip1:
	test rax, rax
	jz .skip2

	._loop2:
		vmovss xmm8, [rdi]

		vmulss xmm8, xmm7

		vmulss xmm8, xmm0
		vcvtps2dq xmm8, xmm8
		vpand xmm9, xmm8, xmm1 
		vpsubd xmm8, xmm9
		vcvtdq2ps xmm8, xmm8
		vmulss xmm8, xmm2
		vmovss xmm10, xmm8
		vmulss xmm8, xmm3
		vaddss xmm8, xmm4
		vmulss xmm8, xmm10
		vaddss xmm8, xmm5
		vpaddd xmm8, xmm9

		vmaxss xmm8, xmm11
		vaddss xmm8, xmm6
		vdivss xmm8, xmm6, xmm8

		vmovss [rdi], xmm8
		add rdi, 4
		dec rax
	jnz ._loop2
	.skip2:
ret


; rdi - dest, rsi - source, rdx - n; sub [rdi], [rsi] (arrays)
m_sub_arr:
	mov rax, rdx
	and rax, 0x7
	shr rdx, 3

	test rdx, rdx
	jz .skip1
	._loop1:
		vmovups ymm0, [rdi]
		vmovups ymm1, [rsi]
		vsubps ymm0, ymm1
		vmovups [rdi], ymm0
		add rdi, 32		
		add rsi, 32
		dec rdx
	jnz ._loop1

	.skip1:
	test rax, rax
	jz .skip2

	._loop2:
		vmovss xmm0, [rdi]
		vmovss xmm1, [rsi]
		vsubss xmm0, xmm1
		vmovss [rdi], xmm0
		add rdi, 4
		add rsi, 4
		dec rax
	jnz ._loop2

	.skip2:
ret

; rdi - arr, rsi - n; find array sum
m_sum:
	mov rax, rsi
	and rax, 0x7
	shr rsi, 3

	vpxor ymm0, ymm0

	test rsi, rsi
	jz .skip1
	._loop1:
		vmovups ymm1, [rdi]
		vaddps ymm0, ymm1
		add rdi, 32
		dec rsi
	jnz ._loop1

	vextractf128 xmm1, ymm0, 1
	vaddps xmm0, xmm1
	vhaddps xmm0, xmm0
	vhaddps xmm0, xmm0

	.skip1:

	test rax, rax
	jz .skip2
	._loop2:
		vmovss xmm1, [rdi]
		vaddss xmm0, xmm1
		add rdi, 4
		dec rax
	jnz ._loop2

	.skip2:
ret

; rdi - dest(column), rsi - matr, rdx - column, rcx - n_col, r8 - n_row
m_dot:
	._loop1:
		mov rax, rcx
		mov r10, rcx
		shr r10, 3
		and rax, 0x7

		mov r11, rdx

		vpxor ymm0, ymm0

		test r10, r10
		jz .skip1

		._loop2:
			vmovups ymm1, [rsi]
			vmovups ymm2, [r11]
			vmulps ymm1, ymm2
			vaddps ymm0, ymm1
			add r11, 32
			add rsi, 32
			dec r10
		jnz ._loop2

		vextractf128 xmm1, ymm0, 1
		vaddps xmm0, xmm1
		vhaddps xmm0, xmm0
		vhaddps xmm0, xmm0


		.skip1:
		test rax, rax
		jz .skip2

		._loop3:
			vmovss xmm1, [rsi]
			vmovss xmm2, [r11]
			vmulss xmm1, xmm2
			vaddss xmm0, xmm1
			add r11, 4
			add rsi, 4
			dec rax
		jnz ._loop3

		.skip2:
		vmovss [rdi], xmm0
		add rdi, 4
		dec r8
	jnz ._loop1
ret


; rdi - dest, rsi - src (struct column*), rdx - n_cols_src, rcx - n_rows_src
m_transp_matr_from_col:
	cld
	._loop1:
		add rsi, 8
		lodsq
		mov r8, rcx
		mov r9, rcx
		shr r8, 3
		and r9, 0x7

		test r8, r8
		jz .skip1

		._loop2:
			vmovups ymm0, [rax]
			vmovups [rdi], ymm0
			add rdi, 32
			add rax, 32
			dec r8
		jnz ._loop2

		.skip1:

		test r9, r9
		jz .skip2

		._loop3:
			vmovss xmm0, [rax]
			vmovss [rdi], xmm0
			add rdi, 4
			add rax, 4
			dec r9
		jnz ._loop3

		.skip2:
		dec rdx
	jnz ._loop1
ret


m_dot_w_mul:
	._loop1:
		mov rax, rcx
		mov r10, rcx
		shr r10, 3
		and rax, 0x7

		mov r11, rdx

		vpxor ymm3, ymm3

		test r10, r10
		jz .skip1

		._loop2:
			vmovups ymm1, [rsi]
			vmovups ymm2, [r11]
			vmulps ymm1, ymm2
			vaddps ymm3, ymm1
			add r11, 32
			add rsi, 32
			dec r10
		jnz ._loop2

		vextractf128 xmm1, ymm3, 1
		vaddps xmm3, xmm1
		vhaddps xmm3, xmm3
		vhaddps xmm3, xmm3

		.skip1:
		test rax, rax
		jz .skip2

		._loop3:
			vmovss xmm1, [rsi]
			vmovss xmm2, [r11]
			vmulss xmm1, xmm2
			vaddss xmm3, xmm1
			add r11, 4
			add rsi, 4
			dec rax
		jnz ._loop3

		.skip2:
		vmulss xmm3, xmm0
		vmovss [rdi], xmm3
		add rdi, 4
		dec r8
	jnz ._loop1
ret

; dest - src*multp
; rdi - dest, rsi - src, rdx - n, xmm0 - multp
m_sub_arr_mul_ss:
	mov rax, rdx
	shr rdx, 3
	and rax, 0x7

	test rdx, rdx
	jz .skip1

	vbroadcastss ymm0, xmm0

	._loop1:
		vmovups ymm1, [rsi]
		vmulps ymm1, ymm0
		vmovups ymm2, [rdi]
		vsubps ymm2, ymm1
		vmovups [rdi], ymm2
		add rdi, 32
		add rsi, 32
		dec rdx
	jnz ._loop1

	.skip1:
	test rax, rax
	jz .skip2

	._loop2:
		vmovss xmm1, [rsi]
		vmulss xmm0, xmm1
		vmovss xmm2, [rdi]
		vsubss xmm2, xmm1
		vmovss [rdi], xmm2
		add rdi, 4
		add rsi, 4
		dec rax
	jnz ._loop2

	.skip2:
ret





