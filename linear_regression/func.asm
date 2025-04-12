[BITS 64]

global add_all
global gauss
global one_diagonal
global extransw


section .text

align 8



align 8
add_all: ; rsi - ptr to equ, rdi - height, rdx - width
	mov r8, rdx
	xor r9, r9
	._checkloop:
		pxor xmm1, xmm1
		mov rax, r8
		xor rdx, rdx
		mul r9
		add rax, r9
		movss xmm0, [rsi+rax*4]
		cmpss xmm1, xmm0, 0
		movd edx, xmm1
		and edx, edx
		jnz ._zero_found
		._zero_fixed:
		inc r9
		cmp r9, rdi
	jnz ._checkloop

	xor rax, rax
ret

	._zero_found:
		xor r10, r10
		._find_nzero_loop:
			pxor xmm1, xmm1
			mov rax, r8
			xor rdx, rdx
			mul r10
			add rax, r9
			movss xmm0, [rsi+rax*4]
			cmpss xmm1, xmm0, 0
			movd edx, xmm1
			and edx, edx
			jz ._add_to_zero
			inc r10
			cmp r10, rdi
		jnz ._find_nzero_loop
	mov rax, 1
ret
		._add_to_zero: ; r9 - row with zero, r10 - row to add
			mov rcx, r8
			shr rcx, 3

			mov rax, r8
			xor rdx, rdx
			mul r9
			mov r11, rax

			mov rax, r8
			xor rdx, rdx
			mul r10

			;dec rcx
			shl rcx, 3
			add rax, rcx
			add r11, rcx
			shr rcx, 3
			;inc rcx

			._addloop:
				sub rax, 8
				sub r11, 8
				vmovups ymm0, [rsi+r11*4] ; row with zero
				vmovups ymm1, [rsi+rax*4]
				vaddps ymm0, ymm1
				vmovups [rsi+r11*4], ymm0
				dec rcx
			jnz ._addloop
		jmp ._zero_fixed


align 8
one_diagonal: ; rdi - height, rsi - equ, rdx - width
	mov r8, rdx
	xor r9, r9 
	lea r11, [r8*4]
	;xor rax, rax
	mov rax, rsi

		._loop:
				;mov rax, r8
				;xor rdx, rdx
				;mul r9
				;lea rax, [rsi+rax*4]

				mov edx, 0x3F800000
				movd xmm1, edx
				movss xmm2, [rax+r9*4]
				divss xmm1, xmm2
				vbroadcastss ymm3, xmm1

				mov rcx, r8
				;shr rcx, 3
				;dec rcx
				shl rcx, 2
				add rax, rcx
				shr rcx, 5
				;inc rcx

				._sloop:
				sub rax, 32

				vmovups ymm0, [rax]
				vmulps ymm0, ymm3, ymm0
				vmovups [rax], ymm0

				dec rcx
			jnz ._sloop

			shr rcx, 5
			inc rcx

			add rax, r11
			inc r9
			cmp r9, rdi
		jnz ._loop
ret




align 8
gauss:
	; rdx - width, rdi - unused, rsi - equ, rcx - height  [rdx*4] - width in bytes
	;push r12
	xor r9, r9

	;lea rax, [rsi+rdx*4]
	mov rax, rsi
	._loop_straight:	
		lea r10, [r9+1]
		cmp r10, rcx
		jz ._backw
		lea r11, [rax+rdx*4]
		._loop_straight_mul:
			movss xmm2, [r11+r9*4]
			pxor xmm4, xmm4
			cmpss xmm4, xmm2, 0
			movd edi, xmm4
			and edi, edi
			jnz ._loop_straight_mul_zeroed

			movss xmm1, [rax+r9*4]
			vdivss xmm1, xmm2, xmm1

			vbroadcastss ymm4, xmm1

			mov rdi, rdx
			lea rax, [rax+rdx*4]
			shr rdi, 3
			lea r11, [r11+rdx*4]

			._sm_loop:
				sub rax, 32
				sub r11, 32
				vmovups ymm0, [rax]
				vmulps ymm3, ymm0, ymm4
				vmovups ymm1, [r11]
				vsubps ymm1, ymm1, ymm3
				vmovups [r11], ymm1

				dec rdi
			jnz ._sm_loop

			._loop_straight_mul_zeroed:
			inc r10
			lea r11, [r11+rdx*4]
			cmp r10, rcx
			jnz ._loop_straight_mul
		;lea r10, [r9+1]
		lea r11, [rax+rdx*4]
		pxor xmm4, xmm4
		movss xmm0, [r11+r9*4+4]
		cmpss xmm4, xmm0, 0
		movd edi, xmm4
		and edi, edi
		jnz ._diag_next_zero_straight
		._fixed_kinda:
		inc r9
		lea rax, [rax+rdx*4]
		cmp r9, rcx
	jnz ._loop_straight

	._backw:
		lea rdi, [rdx*4]
		;sub rax, rdi
		;dec r9
		._loop_backward:
			mov r11, rax
			sub r11, rdi
			lea r10, [r9-1]
			;cmp r10, 0
			and r9, r9
			jz .end
			._loop_backward_mul:
				pxor xmm4, xmm4
				movss xmm2, [r11+r9*4]
				cmpss xmm4, xmm2, 0
				movd r8d, xmm4
				and r8d, r8d
				jnz ._loop_backward_mul_zeroed
				movss xmm1, [rax+r9*4]
				vdivss xmm1, xmm2, xmm1

				vbroadcastss ymm4, xmm1

				mov r8, rdx
				lea rax, [rax+rdx*4]
				lea r11, [r11+rdx*4]
				shr r8, 3
										
				._sm_loop_bckw:
					sub rax, 32
					sub r11, 32

					vmovups ymm0, [rax]
					vmulps ymm3, ymm0, ymm4
					vmovups ymm1, [r11]
					vsubps ymm1, ymm1, ymm3
					vmovups [r11], ymm1

					dec r8
				jnz ._sm_loop_bckw

				._loop_backward_mul_zeroed:
				sub r11, rdi
				dec r10
				cmp r10, 0
			jge ._loop_backward_mul
			sub rax, rdi
			dec r9
		jnz ._loop_backward

	.end:
	;pop r12
	xor rax, rax
ret

._diag_next_zero_straight:
	lea r10, [r9+2]
	cmp r10, rcx
	jae ._fixed_kinda
	._diag_next_zero_straight_loop:
		lea r8, [r11+rdx*4]

		pxor xmm4, xmm4
		movss xmm0, [r8+r9*4+4]
		cmpss xmm4, xmm0, 0
		movd edi, xmm4
		and edi, edi
		jz ._diag_next_zero_straight_loop_add
		inc r10
		lea r8, [r8+rdx*4]
		cmp r10, rcx
	jnz ._diag_next_zero_straight_loop
	jmp ._fixed_kinda

	._diag_next_zero_straight_loop_add:
		mov rdi, rdx
		lea r8, [r8+rdx*4]
		lea r11, [r11+rdx*4]
		shr rdi, 3

		._diag_next_zero_straight_loop_add_sm_loop:
			sub r8, 32
			sub r11, 32
			vmovups ymm0, [r11]
			vmovups ymm1, [r8]
			vaddps ymm0, ymm0, ymm1
			vmovups [r11], ymm0
			dec rdi
		jnz ._diag_next_zero_straight_loop_add_sm_loop
	jmp ._fixed_kinda




align 8
extransw: ; rsi - equ, rdi - answbuf, rcx - height, rdx - width
	mov r8, rdx
	xor r9, r9
	.answ_loop:
		mov rax, r8
		xor rdx, rdx
		mul r9
		add rax, rcx

		mov eax, dword [rsi+rax*4]
		stosd

		inc r9
		cmp r9, rcx
	jnz .answ_loop
ret

