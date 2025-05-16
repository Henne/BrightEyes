.186
.model large
.data

src_ptr	dd 0
word_1EC4C	dd 0
dst_ptr	dd 0
src_end_ptr	dd 0
pp20_counter	dw 0
word_1EC5A	dd 0
word_1EC5E	dw 0
word_1EC60	dw 0
word_1EC62	dd 0

.code

	public _decomp_pp20


_decomp_pp20	proc far


dst_in		= dword	ptr  6
src_in		= dword	ptr  0Ah
len_in		= dword	ptr  0Eh

		push	bp
		mov	bp, sp
		push	ds
		push	es
		push	si
		push	di
		mov	ax, word ptr [bp+dst_in+2]
		mov	bx, word ptr [bp+dst_in]
		mov	word ptr dst_ptr+2,	ax
		mov	word ptr dst_ptr, bx
		mov	ax, word ptr [bp+src_in+2]
		mov	bx, word ptr [bp+src_in]
		mov	di, bx
		xor	dx, dx
		add	bx, word ptr [bp+len_in]
		adc	dx, word ptr [bp+len_in+2]
		mov	cx, 0Ch
		shl	dx, cl
		add	ax, dx
		mov	word ptr src_ptr+2,	ax
		mov	word ptr src_ptr, bx
		add	di, 4
		mov	word ptr src_end_ptr+2,	ax
		mov	word ptr src_end_ptr, di
		call	sub_1B3E3
		pop	di
		pop	si
		pop	es
		pop	ds
		mov	sp, bp
		pop	bp
		retf
_decomp_pp20	endp

sub_1B3E3	proc near
		mov	word ptr word_1EC62, 1
		mov	word ptr word_1EC62+2, 0
		mov	ax, word ptr dst_ptr
		mov	word ptr word_1EC4C, ax
		mov	ax, word ptr dst_ptr+2
		mov	word ptr word ptr word_1EC4C+2, ax
		xor	ax, ax
		sub	word ptr src_ptr, 4
		sbb	ax, 0
		mov	cx, 0Ch
		shl	ax, cl
		add	word ptr src_ptr+2,	ax
		les	di, src_ptr
		mov	bh, es:[di]
		mov	bl, es:[di+1]
		mov	ah, es:[di+2]
		mov	al, es:[di+3]
		mov	word ptr word_1EC5A, ax
		mov	word ptr word_1EC5A+2, bx
		cmp	al, 0
		jz	short loc_1B448
		call	sub_1B589
		dec	byte ptr word ptr word_1EC5A
		mov	ax, word ptr word_1EC62
		mov	dx, word ptr word_1EC62+2
		mov	cx, word ptr word_1EC5A
		call	sub_1B679
		mov	word ptr word_1EC62, ax
		mov	word ptr word_1EC62+2, dx

loc_1B448:
		mov	cl, 8
		mov	ax, word ptr word_1EC5A
		mov	dx, word ptr word_1EC5A+2
		call	sub_1B679
		mov	word ptr word_1EC5A, ax
		mov	word ptr word_1EC5A+2, dx
		mov	ax, word ptr word_1EC5A
		xor	dx, dx
		add	word ptr dst_ptr, ax
		adc	dx, word ptr word_1EC5A+2
		mov	cx, 0Ch
		shl	dx, cl
		add	word ptr dst_ptr+2,	dx

loc_1B471:
		call	sub_1B589
		jb	short loc_1B4D2
		mov	word_1EC5E, 0

loc_1B47C:
		mov	pp20_counter, 1
		call	sub_1B5F0
		mov	ax, word ptr word_1EC5A
		add	word_1EC5E, ax
		cmp	ax, 3
		jz	short loc_1B47C

loc_1B491:
		mov	pp20_counter, 7
		call	sub_1B5F0
		xor	ax, ax
		sub	word ptr dst_ptr, 1
		sbb	ax, 0
		mov	cx, 0Ch
		shl	ax, cl
		add	word ptr dst_ptr+2,	ax
		mov	ax, word ptr word_1EC5A
		les	di, dst_ptr
		mov	es:[di], al
		dec	word_1EC5E
		jge	short loc_1B491
		mov	cx, word ptr dst_ptr+2
		mov	bx, word ptr dst_ptr
		mov	dx, word ptr word ptr word_1EC4C+2
		mov	ax, word ptr word_1EC4C
		call	sub_1B697
		jb	short loc_1B4D2
		retn

loc_1B4D2:
		mov	pp20_counter, 1
		call	sub_1B5F0
		mov	pp20_counter, 0
		les	di, src_end_ptr
		add	di, word ptr word_1EC5A
		mov	al, es:[di]
		mov	byte ptr pp20_counter, al
		mov	ax, word ptr word_1EC5A
		mov	word_1EC5E, ax
		cmp	ax, 3
		jnz	short loc_1B528
		call	sub_1B589
		jb	short loc_1B505
		mov	pp20_counter, 7

loc_1B505:
		call	pp20_dec_counter
		mov	ax, word ptr word_1EC5A
		mov	word_1EC60, ax

loc_1B50E:
		mov	pp20_counter, 2
		call	sub_1B5F0
		mov	ax, word ptr word_1EC5A
		add	word_1EC5E, ax
		cmp	word ptr word_1EC5A, 7
		jz	short loc_1B50E
		jmp	short loc_1B531
;REMARK: may become an obstacle
		nop
;

loc_1B528:
		call	pp20_dec_counter
		mov	ax, word ptr word_1EC5A
		mov	word_1EC60, ax

loc_1B531:
		inc	word_1EC5E

loc_1B535:
		push	ds
		les	di, dst_ptr
		xor	ax, ax
		add	di, word_1EC60
		adc	ax, 0
		mov	cx, 0Ch
		shl	ax, cl
		mov	bx, es
		add	bx, ax
		mov	es, bx
		xor	ax, ax
		sub	word ptr dst_ptr, 1
		sbb	ax, 0
		mov	cx, 0Ch
		shl	ax, cl
		add	word ptr dst_ptr+2,	ax
		lds	si, dst_ptr
		mov	al, es:[di]
		mov	[si], al
		pop	ds
		dec	word_1EC5E
		jge	short loc_1B535
		mov	cx, word ptr dst_ptr+2
		mov	bx, word ptr dst_ptr
		mov	dx, word ptr word ptr word_1EC4C+2
		mov	ax, word ptr word_1EC4C
		call	sub_1B697
		jnb	short locret_1B588
		jmp	loc_1B471

locret_1B588:
		retn
sub_1B3E3	endp


sub_1B589	proc near
		mov	bx, word ptr word_1EC62
		mov	ax, word ptr word_1EC62+2
		mov	dx, 0
		shr	ax, 1
		rcr	bx, 1
		jnb	short loc_1B59C
		mov	dx, 1

loc_1B59C:
		cmp	ax, 0
		jnz	short loc_1B5DE
		cmp	bx, 0
		jnz	short loc_1B5DE
		xor	ax, ax
		sub	word ptr src_ptr, 4
		sbb	ax, 0
		mov	cx, 0Ch
		shl	ax, cl
		add	word ptr src_ptr+2,	ax
		les	di, src_ptr
		mov	ah, es:[di]
		mov	al, es:[di+1]
		mov	bh, es:[di+2]
		mov	bl, es:[di+3]
		mov	cx, dx
		clc
		jcxz	short loc_1B5D2
		stc

loc_1B5D2:
		mov	dx, 0
		rcr	ax, 1
		rcr	bx, 1
		jnb	short loc_1B5DE
		mov	dx, 1

loc_1B5DE:

		mov	word ptr word_1EC62, bx
		mov	word ptr word_1EC62+2, ax
		mov	cx, dx
		clc
		jcxz	short locret_1B5EB
		stc

locret_1B5EB:
		retn
sub_1B589	endp

pp20_dec_counter	proc near
		dec	pp20_counter
pp20_dec_counter	endp

sub_1B5F0	proc near

		mov	word ptr word_1EC5A, 0
		mov	word ptr word_1EC5A+2, 0

loc_1B5FC:
		mov	bx, word ptr word_1EC62
		mov	ax, word ptr word_1EC62+2
		mov	dx, 0
		shr	ax, 1
		rcr	bx, 1
		jnb	short loc_1B60F
		mov	dx, 1

loc_1B60F:
		cmp	ax, 0
		jnz	short loc_1B651
		cmp	bx, 0
		jnz	short loc_1B651
		xor	ax, ax
		sub	word ptr src_ptr, 4
		sbb	ax, 0
		mov	cx, 0Ch
		shl	ax, cl
		add	word ptr src_ptr+2,	ax
		les	di, src_ptr
		mov	ah, es:[di]
		mov	al, es:[di+1]
		mov	bh, es:[di+2]
		mov	bl, es:[di+3]
		mov	cx, dx
		clc
		jcxz	short loc_1B645
		stc

loc_1B645:
		mov	dx, 0
		rcr	ax, 1
		rcr	bx, 1
		jnb	short loc_1B651
		mov	dx, 1

loc_1B651:

		mov	word ptr word_1EC62, bx
		mov	word ptr word_1EC62+2, ax
		mov	bx, word ptr word_1EC5A
		mov	ax, word ptr word_1EC5A+2
		mov	cx, dx
		clc
		jcxz	short loc_1B665
		stc

loc_1B665:
		rcl	bx, 1
		rcl	ax, 1
		mov	word ptr word_1EC5A, bx
		mov	word ptr word_1EC5A+2, ax
		dec	pp20_counter
		jl	short locret_1B678
		jmp	short loc_1B5FC

locret_1B678:
		retn
sub_1B5F0	endp


sub_1B679	proc near

		cmp	cl, 10h
		jnb	short loc_1B68E
		mov	bx, dx
		shr	ax, cl
		shr	dx, cl
		neg	cl
		add	cl, 10h
		shl	bx, cl
		or	ax, bx
		retn

loc_1B68E:
		sub	cl, 10h
		xchg	ax, dx
		xor	dx, dx
		shr	ax, cl
		retn
sub_1B679	endp

sub_1B697	proc near

		push	cx
		mov	ch, al
		mov	cl, 4
		shr	ax, cl
		add	dx, ax
		mov	al, ch
		mov	ah, bl
		shr	bx, cl
		pop	cx
		add	cx, bx
		mov	bl, ah
		and	ax, 0Fh
		and	bx, 0Fh
		cmp	dx, cx
		jnz	short locret_1B6B7
		cmp	ax, bx

locret_1B6B7:
		retn
sub_1B697	endp

		end
