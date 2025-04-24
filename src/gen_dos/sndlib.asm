SNDLIB_TEXT	segment byte public 'CODE'
SNDLIB_TEXT ends

DGROUP	group _DATA
		assume cs:SNDLIB_TEXT, ds:DGROUP
		
_DATA	segment word public 'DATA'
_DATA	ends


SNDLIB_TEXT		segment	byte public 'CODE'
				assume cs:SNDLIB_TEXT, ds:DGROUP

_sndlib_normalize_ptr	proc far

p_off		= word ptr  6
p_seg		= word ptr  8

		push	bp
		mov	bp, sp
		push	ds
		push	es
		push	si
		push	di
		mov	ax, [bp+p_off]
		mov	dx, [bp+p_seg]
		mov	cx, 4
		shr	ax, cl
		add	dx, ax
		mov	ax, [bp+p_off]
		and	ax, 0Fh
		pop	di
		pop	si
		pop	es
		pop	ds
		mov	sp, bp
		pop	bp
		retf
_sndlib_normalize_ptr	endp


sndlib_030	proc far		; CODE XREF: init_sndlib+16P
		push	bp
		mov	bp, sp
		push	ds
		push	es
		push	si
		push	di
		mov	ax, 3508h
		int	21h		; DOS -	2+ - GET INTERRUPT VECTOR
					; AL = interrupt number
					; Return: ES:BX	= value	of interrupt vector
		mov	word ptr cs:dword_A647+2, es
		mov	word ptr cs:dword_A647,	bx
		mov	ax, seg	SNDLIB_TEXT
		mov	ds, ax
		mov	dx, offset ISR08
		mov	ax, 2508h
		int	21h		; DOS -	SET INTERRUPT VECTOR
					; AL = interrupt number
					; DS:DX	= new vector to	be used	for specified interrupt
		pop	di
		pop	si
		pop	es
		pop	ds
		mov	sp, bp
		pop	bp
		retf
sndlib_030	endp


sndlib_05b	proc far

		push	bp
		mov	bp, sp
		push	ds
		push	es
		push	si
		push	di
		mov	dx, word ptr cs:dword_A647
		mov	ax, word ptr cs:dword_A647+2
		mov	ds, ax
		mov	ax, 2508h
		int	21h		; DOS -	SET INTERRUPT VECTOR
					; AL = interrupt number
					; DS:DX	= new vector to	be used	for specified interrupt
		mov	al, 36h	; '6'
		out	43h, al		; Timer	8253-5 (AT: 8254.2).
		xor	ax, ax
		out	40h, al		; Timer	8253-5 (AT: 8254.2).
		out	40h, al		; Timer	8253-5 (AT: 8254.2).
		pop	di
		pop	si
		pop	es
		pop	ds
		mov	sp, bp
		pop	bp
		retf
sndlib_05b	endp


sndlib_084	proc far

arg_0		= word ptr  6

		push	bp
		mov	bp, sp
		push	ds
		push	es
		push	si
		push	di
		mov	ax, [bp+arg_0]
		call	sub_A845
		pop	di
		pop	si
		pop	es
		pop	ds
		mov	sp, bp
		pop	bp
		retf
sndlib_084	endp


sndlib_099	proc far

		push	bp
		mov	bp, sp
		push	ds
		push	es
		push	si
		push	di
		call	sub_A88E
		pop	di
		pop	si
		pop	es
		pop	ds
		mov	sp, bp
		pop	bp
		retf
sndlib_099	endp


sndlib_0ab	proc far

arg_0		= word ptr  6

		push	bp
		mov	bp, sp
		push	ds
		push	es
		push	si
		push	di
		mov	bx, [bp+arg_0]
		mov	word_D583, bx
		pop	di
		pop	si
		pop	es
		pop	ds
		mov	sp, bp
		pop	bp
		retf
sndlib_0ab	endp


sndlib_0c1	proc far

arg_0		= word ptr  6

		push	bp
		mov	bp, sp
		push	ds
		push	es
		push	si
		push	di
		cmp	byte ptr _g_sndlib_byte, 0
		jz	short loc_A623
		mov	cx, [bp+arg_0]
		mov	ax, word_D583
		mov	es, ax
		mov	bx, es:16h
		add	bx, 0FEh ; '˛'
		mov	si, es:[bx]
		mov	bx, cx
		shl	bx, 1
		shl	bx, 1
		shl	bx, 1
		add	si, bx
		mov	al, [si+1]
		and	al, 0Fh
		mov	al, 8
		xor	ah, ah
		shl	ax, 1
		shl	ax, 1
		shl	ax, 1
		shl	ax, 1
		add	ax, 1ED3h
		mov	di, ax
		mov	ax, [di+0Ah]
		cmp	ax, 0
		jz	short loc_A5F4
		mov	al, es:[si+6]
		cmp	al, [di+0Ch]
		jb	short loc_A623

loc_A5F4:
		mov	word ptr [di], 0FFh
		mov	ax, es:[si+4]
		mov	[di+0Ah], ax
		mov	al, es:[si+6]
		mov	[di+0Ch], al
		mov	al, es:[si+3]
		mov	[di+7],	al
		xor	ah, ah
		mov	al, es:[si+2]
		add	ax, ax
		mov	[di+2],	ax
		xor	al, al
		mov	[di+4],	ax
		mov	[di], ax
		mov	byte ptr [di+0Eh], 1

loc_A623:

		pop	di
		pop	si
		pop	es
		pop	ds
		mov	sp, bp
		pop	bp
		retf
sndlib_0c1	endp


sndlib_14b	proc far
		push	bp
		mov	bp, sp
		push	ds
		push	es
		push	si
		push	di
		mov	bx, [bp+4]
		push	ds
		add	bx, bx
		add	bx, 1FF5h
		mov	ax, [bx]
		pop	ds
		pop	di
		pop	si
		pop	es
		pop	ds
		mov	sp, bp
		pop	bp
		retf
sndlib_14b	endp

dword_A647	dd 0

word_A64B	dw 0

ISR08		proc far
		push	ax
		push	bx
		push	cx
		push	dx
		push	es
		push	di
		push	si
		push	ds
		mov	ax, seg	_DATA
		mov	ds, ax
		call	sub_A980
		call	sub_A693
		mov	al, 34h	; '4'
		out	43h, al		; Timer	8253-5 (AT: 8254.2).
		mov	al, 55h	; 'U'
		out	40h, al		; Timer	8253-5 (AT: 8254.2).
		mov	al, 55h	; 'U'
		out	40h, al		; Timer	8253-5 (AT: 8254.2).
		pop	ds
		pop	si
		pop	di
		pop	es
		pop	dx
		pop	cx
		pop	bx
		dec	cs:word_A64B
		js	short loc_A680
		mov	al, 20h	; ' '
		out	20h, al		; Interrupt controller,	8259A.
		pop	ax
		iret


loc_A680:
		mov	ax, 2
		mov	cs:word_A64B, ax
		pop	ax
		push	word ptr cs:dword_A647+2
		push	word ptr cs:dword_A647
		retf
ISR08		endp ; sp = -4


sub_A693	proc near
		mov	al, byte_D41E
		test	al, 0FFh
		jz	short locret_A701
		dec	word_D418
		jns	short locret_A701
		mov	ax, word_D420
		mov	word_D418, ax
		mov	ax, word_D583
		mov	es, ax
		mov	word_D41A, 0
sub_A693	endp


loc_A6B1:
		mov	bx, word_D41A
		add	bx, offset unk_D422
		mov	di, bx
		dec	word ptr [di+4]
		jns	short loc_A6F5

loc_A6C0:
		cmp	word ptr [di], 1FEh
		jz	short loc_A6F5
		mov	bx, es:16h
		add	bx, [di]
		mov	ax, es:[bx]
		add	ax, [di+2]
		mov	si, ax
		mov	al, es:[si+3]
		cmp	al, 40h
		jb	short loc_A702
		cmp	al, 80h
		jb	short loc_A6EC
		cmp	al, 0C0h

loc_A6E3:
		jb	short loc_A740

loc_A6E5:
		cmp	al, 0F0h
		jb	short loc_A6EC
		jmp	loc_A783


loc_A6EC:

		mov	word ptr [di+4], 0

loc_A6F1:
		add	word ptr [di+2], 4

loc_A6F5:
		add	word_D41A, 8
		cmp	word_D41A, 40h ; '@'
		jnz	short loc_A6B1

locret_A701:
		retn

loc_A702:
		mov	bx, offset unk_D463
		mov	cx, es:[si+1]
		and	cx, 0Fh
		shl	cx, 1
		shl	cx, 1
		shl	cx, 1
		shl	cx, 1
		add	bx, cx
		mov	word ptr [bx], 0FFh
		mov	al, es:[si+3]
		add	al, [di+6]
		mov	[bx+7],	al
		xor	ah, ah
		mov	al, es:[si+2]
		add	ax, ax
		mov	[bx+2],	ax

loc_A72F:
		xor	al, al
		mov	[bx+4],	ax
		mov	[bx], ax
		mov	byte ptr [bx+0Eh], 1

loc_A73A:
		add	word ptr [di+2], 4
		jmp	short loc_A6C0


loc_A740:
		mov	bx, offset unk_D463
		mov	cl, es:[si+1]
		and	cx, 0Fh
		shl	cx, 1

loc_A74C:
		shl	cx, 1
		shl	cx, 1
		shl	cx, 1
		add	bx, cx
		mov	word ptr [bx], 0FFh
		mov	al, es:[si+3]
		and	al, 3Fh
		add	al, [di+6]
		mov	[bx+7],	al
		xor	ah, ah
		mov	al, es:[si+2]
		add	ax, ax
		mov	[bx+2],	ax
		xor	al, al
		mov	[bx+4],	ax
		mov	[bx], ax
		mov	byte ptr [bx+0Eh], 1
		mov	al, es:[si]
		mov	[di+4],	ax
		jmp	loc_A6F1


loc_A783:
		cmp	al, 0F0h ; ''
		jz	short loc_A7DF
		cmp	al, 0F3h ; 'Û'
		jz	short loc_A79A
		cmp	al, 0F4h ; 'Ù'
		jz	short loc_A7C1
		cmp	al, 0F5h ; 'ı'
		jz	short loc_A7A6

loc_A793:
		cmp	al, 0FDh ; '˝'
		jz	short loc_A7C9
		jmp	loc_A6EC


loc_A79A:
		mov	al, es:[si+2]
		xor	ah, ah
		mov	[di+4],	ax

loc_A7A3:
		jmp	loc_A6F1


loc_A7A6:
		mov	bx, offset unk_D463
		mov	cx, es:[si+1]
		and	cx, 0Fh
		shl	cx, 1
		shl	cx, 1
		shl	cx, 1
		shl	cx, 1
		add	bx, cx
		mov	byte ptr [bx+0Eh], 0
		jmp	loc_A73A


loc_A7C1:
		mov	word ptr [di+4], 0FFFFh
		jmp	loc_A6F5

loc_A7C9:
		mov	bx, es:[si+1]
		and	bx, 3
		add	bx, bx
		add	bx, 1FF5h
		mov	ax, es:[si+2]
		mov	[bx], ax
		jmp	loc_A73A

loc_A7DF:
		call	sub_A7E5
		jmp	loc_A6B1

sub_A7E5	proc near
		inc	word_D41C
sub_A7E5	endp


sub_A7E9	proc near

		mov	bx, word_D41C
		shl	bx, 1
		shl	bx, 1
		shl	bx, 1
		shl	bx, 1
		add	bx, 0DAh ; '⁄'
		mov	si, bx
		cmp	word ptr es:[si], 0FEEFh

loc_A800:
		jz	short loc_A82D
		mov	cx, 8
		mov	bx, 1E92h

loc_A808:
		mov	ax, es:[si]

loc_A80B:
		mov	[bx+6],	ah
		xor	ah, ah
		add	ax, ax
		mov	[bx], ax
		mov	word ptr [bx+2], 0
		mov	word ptr [bx+4], 0
		add	si, 2
		add	bx, 8
		loop	loc_A808
		mov	word_D41A, 0
		retn

loc_A82D:
		mov	ax, es:[si+2]
		cmp	ax, 100h
		jnz	short loc_A7DF
		mov	ax, es:[si+4]
		mov	cl, al
		mov	al, ah
		mov	ah, cl
		mov	word_D41C, ax
		jmp	short sub_A7E9
sub_A7E9	endp


sub_A845	proc near
		mov	byte_D41F, al
		mov	ax, word_D583
		mov	es, ax
		mov	bl, byte_D41F
		xor	bh, bh

loc_A853:
		add	bx, bx
		add	bx, 1Ah
		mov	si, bx
		mov	bx, es:[si+80h]
		mov	word_D420, bx
		mov	bx, es:[si]
		mov	word_D41C, bx

loc_A86A:
		call	sub_A7E9
		call	sub_A879
		call	sub_A88E
		mov	byte_D41E, 1
		retn
sub_A845	endp


sub_A879	proc near
		mov	bx, offset unk_D463
		mov	cx, 9

loc_A87F:
		mov	word ptr [bx], 7FFFh
		mov	word ptr [bx+0Ah], 0
		add	bx, 10h
		loop	loc_A87F
		retn
sub_A879	endp


sub_A88E	proc near

		mov	ax, 0B0h ; '∞'

loc_A891:
		push	ax
		call	sub_A8D8
		pop	ax
		add	ax, 1
		cmp	al, 0B8h ; '∏'
		jnz	short loc_A891
		mov	bx, 3F40h

loc_A8A0:
		mov	cx, 3

loc_A8A3:
		call	sub_A8CA
		add	bx, 1
		loop	loc_A8A3
		add	bx, 5
		cmp	bl, 58h	; 'X'
		jnz	short loc_A8A0
		mov	byte_D41E, 0
		mov	ax, 0FFFFh
		mov	bx, 1ED3h
		mov	cx, 9

loc_A8C1:
		mov	[bx+2],	ax
		add	bx, 10h
		loop	loc_A8C1
		retn
sub_A88E	endp


sub_A8CA	proc near
		mov	ax, bx
		call	sub_A8D8
		mov	ax, bx
		add	ax, 3
		call	sub_A8D8
		retn
sub_A8CA	endp


sub_A8D8	proc near
		mov	dx, 388h
		out	dx, al
		inc	dx
		call	sub_A8E7
		mov	al, ah
		out	dx, al
		call	sub_A907
		retn
sub_A8D8	endp


sub_A8E7	proc near		; CODE XREF: sub_A8D8+5p
		push	ax
		mov	al, 36h	; '6'
		out	43h, al		; Timer	8253-5 (AT: 8254.2).
		mov	al, 0FFh
		out	40h, al		; Timer	8253-5 (AT: 8254.2).
		mov	al, 0FFh
		out	40h, al		; Timer	8253-5 (AT: 8254.2).

loc_A8F4:				; CODE XREF: sub_A8E7+1Cj
		mov	al, 6
		out	43h, al		; Timer	8253-5 (AT: 8254.2).
		in	al, 40h		; Timer	8253-5 (AT: 8254.2).
		mov	ah, al
		in	al, 40h		; Timer	8253-5 (AT: 8254.2).
		xchg	al, ah
		cmp	ax, 0FFF0h
		ja	short loc_A8F4
		pop	ax
		retn
sub_A8E7	endp


sub_A907	proc near		; CODE XREF: sub_A8D8+Bp sub_A980+1B4p
		mov	al, 36h	; '6'
		out	43h, al		; Timer	8253-5 (AT: 8254.2).
		mov	al, 0FFh
		out	40h, al		; Timer	8253-5 (AT: 8254.2).
		out	40h, al		; Timer	8253-5 (AT: 8254.2).

loc_A911:				; CODE XREF: sub_A907+19j
		mov	al, 6
		out	43h, al		; Timer	8253-5 (AT: 8254.2).
		in	al, 40h		; Timer	8253-5 (AT: 8254.2).
		mov	ah, al
		in	al, 40h		; Timer	8253-5 (AT: 8254.2).
		xchg	al, ah
		cmp	ax, 0FFD0h
		ja	short loc_A911
		retn
sub_A907	endp

; Attributes: bp-based frame

sndlib_443	proc far		; CODE XREF: init_sndlib+1BP
		push	bp
		mov	bp, sp
		push	ds
		push	es
		push	si
		push	di
		mov	ax, 1
		call	sub_A8D8
		mov	ax, 8
		call	sub_A8D8
		mov	ax, 0BDh ; 'Ω'
		call	sub_A8D8
		pop	di
		pop	si
		pop	es
		pop	ds
		mov	sp, bp
		pop	bp
		retf
sndlib_443	endp


sub_A944	proc far
		push	bp
		mov	bp, sp
		push	ds
		push	es
		push	si
		push	di

loc_A94B:
		mov	ax, 3
		int	33h		; - MS MOUSE - RETURN POSITION AND BUTTON STATUS
					; Return: BX = button status, CX = column, DX =	row
		and	bx, 1
		jz	short loc_A94B
		pop	di
		pop	si
		pop	es
		pop	ds
		mov	sp, bp
		pop	bp
		retf
sub_A944	endp


sub_A95D	proc near
		mov	al, 0
		mov	dx, 3C8h
		out	dx, al
		inc	dx
		mov	al, bh
		and	al, 0Fh
		add	al, al
		add	al, al
		out	dx, al
		mov	al, bl
		and	al, 0F0h
		shr	al, 1
		shr	al, 1
		out	dx, al
		mov	al, bl
		and	al, 0Fh
		add	al, al
		add	al, al
		out	dx, al
		retn
sub_A95D	endp


sub_A980	proc near

		mov	ax, word_D583
		mov	es, ax
		mov	al, 8
		mov	byte_D462, al

loc_A98A:
		mov	bl, byte_D462
		xor	bh, bh
		mov	cl, 4
		shl	bx, cl
		add	bx, 1ED3h
		mov	di, bx
		mov	bx, [di+0Ah]
		cmp	bx, 0
		jz	short loc_A9A5
		dec	word ptr [di+0Ah]

loc_A9A5:
		dec	word ptr [di]
		jns	short loc_AA24
		mov	ax, [di+2]
		test	ah, 0FFh
		js	short loc_AA24

loc_A9B1:

		mov	bx, es:14h
		add	bx, [di+2]
		mov	si, es:[bx]
		add	si, [di+4]
		mov	bl, es:[si+3]
		xor	bh, bh
		mov	ax, bx
		add	bx, bx
		add	bx, ax
		add	bx, 4F1h
		jmp	bx
		jmp	loc_AAB2
		jmp	loc_AAB5
		jmp	loc_AABD
		jmp	loc_AAB1
		jmp	loc_AAAE
		jmp	loc_AADA
		jmp	loc_AAFF
		jmp	loc_AAAE
		jmp	loc_AAAE
		jmp	loc_AAAE
		jmp	loc_AAAE
		jmp	loc_AAB1
		jmp	loc_AAB1
		jmp	loc_AAB1

loc_A9FB:
		jmp	loc_AAB1
		jmp	loc_AAAE
		jmp	loc_AB08
		jmp	loc_AB28
		jmp	loc_AB3A
		jmp	loc_AB5D
		jmp	loc_AB69
		jmp	loc_AB54

loc_AA13:
		jmp	loc_AB76

loc_AA16:
		mov	ax, 4
		add	[di+4],	ax
		jmp	short loc_A9B1

loc_AA1E:
		mov	ax, 4
		add	[di+4],	ax

loc_AA24:
		dec	byte_D462
		js	short locret_AA2D
		jmp	loc_A98A

locret_AA2D:
		retn
sub_A980	endp

byte_AA2E	db 80h dup(0)

loc_AAAE:
		jmp	loc_AA16


loc_AAB1:
		nop

loc_AAB2:
		jmp	loc_AA24


loc_AAB5:
		mov	bx, es:[si]
		mov	[di], bx

loc_AABA:
		jmp	loc_AA1E

loc_AABD:
		cmp	byte ptr [di+0Eh], 0
		jz	short loc_AACE
		cmp	byte ptr [di+6], 0
		jz	short loc_AAD1
		dec	byte ptr [di+6]
		jnz	short loc_AAD7

loc_AACE:
		jmp	loc_AA16


loc_AAD1:
		mov	al, es:[si]

loc_AAD4:
		mov	[di+6],	al

loc_AAD7:
		jmp	loc_AA24

loc_AADA:

		mov	al, [di+6]
		test	al, 0FFh
		jz	short loc_AAF6
		dec	byte ptr [di+6]
		jnz	short loc_AAE9
		jmp	loc_AA16

loc_AAE9:
		mov	bx, es:[si]
		shl	bx, 1
		shl	bx, 1
		mov	[di+4],	bx
		jmp	loc_A9B1

loc_AAF6:
		mov	al, es:[si+2]
		mov	[di+6],	al
		jmp	short loc_AAE9


loc_AAFF:
		cmp	byte ptr [di+0Eh], 0
		jnz	short loc_AADA
		jmp	loc_AA16


loc_AB08:
		mov	bx, [di+8]
		or	bx, 2000h
		mov	ah, bl
		mov	al, 0A0h ; '†'
		add	al, byte_D462
		call	sub_A8D8
		mov	ah, bh
		mov	al, 0B0h ; '∞'
		add	al, byte_D462
		call	sub_A8D8
		jmp	loc_AA16


loc_AB28:
		mov	ax, [di+8]
		mov	al, 0B0h ; '∞'
		add	al, byte_D462
		call	sub_A8D8
		call	sub_A907
		jmp	loc_AA16


loc_AB3A:
		mov	bl, [di+7]
		add	bl, es:[si]
		xor	bh, bh
		shl	bx, 1
		add	bx, 1F63h
		mov	cx, [bx]
		and	cx, 1FFFh
		mov	[di+8],	cx
		jmp	loc_AA16

loc_AB54:
		mov	bx, es:[si]
		mov	[di+8],	bx
		jmp	loc_AA16

loc_AB5D:
		mov	ax, es:[si]
		add	al, [di+0Fh]
		call	sub_A8D8
		jmp	loc_AA16


loc_AB69:
		mov	ax, es:[si]
		add	al, byte_D462
		call	sub_A8D8
		jmp	loc_AA16


loc_AB76:
		mov	ax, es:[si]
		call	sub_A8D8
		jmp	loc_AA16
		
SNDLIB_TEXT	ends


_DATA		segment	word public 'DATA' use16
word_D418	dw 0
word_D41A	dw 0
word_D41C	dw 0
byte_D41E	db 0
byte_D41F	db 0
word_D420	dw 0
unk_D422	db    2
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    4
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    6
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
byte_D462	db 0

unk_D463	db    0

		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    1
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    2
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    8
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    9
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db  0Ah
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db  10h
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db  11h
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db    0
		db  12h
unk_D4F3	db  6Bh
		db    1
		db  81h	; Å
		db    1
		db  98h	; ò
		db    1
		db 0B0h	; ∞
		db    1
		db 0CAh	;  
		db    1
		db 0E5h	; Â
		db    1
		db    2
		db    2
		db  20h
		db    2
		db  41h	; A
		db    2
		db  63h	; c
		db    2
		db  87h	; á
		db    2
		db 0AEh	; Æ
		db    2
		db  6Bh	; k
		db    9
		db  81h	; Å
		db    9
		db  98h	; ò
		db    9
		db 0B0h	; ∞
		db    9
		db 0CAh	;  
		db    9
		db 0E5h	; Â
		db    9
		db    2
		db  0Ah
		db  20h
		db  0Ah
		db  41h	; A
		db  0Ah
		db  63h	; c
		db  0Ah
		db  87h	; á
		db  0Ah
		db 0AEh	; Æ
		db  0Ah
		db  6Bh	; k
		db  0Dh
		db  81h	; Å
		db  0Dh
		db  98h	; ò
		db  0Dh
		db 0B0h	; ∞
		db  0Dh
		db 0CAh	;  
		db  0Dh
		db 0E5h	; Â
		db  0Dh
		db    2
		db  0Eh
		db  20h
		db  0Eh
		db  41h	; A
		db  0Eh
		db  63h	; c
		db  0Eh
		db  87h	; á
		db  0Eh
		db 0AEh	; Æ
		db  0Eh
		db  6Bh	; k
		db  11h
		db  81h	; Å
		db  11h
		db  98h	; ò
		db  11h
		db 0B0h	; ∞
		db  11h
		db 0CAh	;  
		db  11h
		db 0E5h	; Â
		db  11h
		db    2
		db  12h
		db  20h
		db  12h
		db  41h	; A
		db  12h
		db  63h	; c
		db  12h
		db  87h	; á
		db  12h
		db 0AEh	; Æ
		db  12h
		db  6Bh	; k
		db  15h
		db  81h	; Å
		db  15h
		db  98h	; ò
		db  15h
		db 0B0h	; ∞
		db  15h
		db 0CAh	;  
		db  15h
		db 0E5h	; Â
		db  15h
		db    2
		db  16h
		db  20h
		db  16h
		db  41h	; A
		db  16h
		db  63h	; c
		db  16h
		db  87h	; á
		db  16h
		db 0AEh	; Æ
		db  16h
		db  6Bh	; k
		db  19h
		db  81h	; Å
		db  19h
		db  98h	; ò
		db  19h
		db 0B0h	; ∞
		db  19h
		db 0CAh	;  
		db  19h
		db 0E5h	; Â
		db  19h
		db    2
		db  1Ah
		db  20h
		db  1Ah
		db  41h	; A
		db  1Ah
		db  63h	; c
		db  1Ah
		db  87h	; á
		db  1Ah
		db 0AEh	; Æ
		db  1Ah
word_D583	dw 0
		db 11 dup (0)
		db  30h	; 0
		db  30h	; 0
		db  20h
		db  24h	; $

_DATA		ends

		extrn _g_sndlib_byte:byte
		public _sndlib_normalize_ptr

		end
