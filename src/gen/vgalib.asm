; Segment type:	Pure code
;
; There are 32 assembler-written, low-level VGA-routines.
; Only 14 of them are used in the game, and get meaningful names
; The other 18 are called unused_func01 .. unused_func18


SEG005_TEXT	segment byte public 'CODE'
SEG005_TEXT	ends

DGROUP	group _DATA
	assume cs:SEG005_TEXT,ds:DGROUP

_DATA		segment word public 'DATA'
_DATA		ends

SEG005_TEXT	segment byte public 'CODE'

	assume	cs:SEG005_TEXT

_save_display_stat proc far

pointer		= dword ptr 6

		push	bp
		mov	bp, sp
		push	ds
		push	es
		push	si
		push	di
		push	bp

		les	di, [bp+pointer]
		mov	ah, 0Fh
		int	10h
		mov	dx, ax
		xor	ax, ax
		mov	al, bh
		stosw
		mov	al, dl
		stosw
		mov	al, dh
		stosw
		push	es
		mov	ax, 1130h
		mov	bh, 2
		int	10h
		xor	dh, dh
		inc	dx
		pop	es
		mov	es:[di], dx

		pop	bp
		pop	di
		pop	si
		pop	es
		pop	ds
		mov	sp, bp
		pop	bp
		retf

_save_display_stat endp

_set_palette	proc far

p_palette	= dword	ptr  6
first_color	= byte ptr  0Ah
colors		= word ptr  0Ch

		push	bp
		mov	bp, sp
		push	ds
		push	es
		push	si
		push	di

		mov	dx, 3C8h
		mov	al, [bp+first_color]	;# of the first color
		out	dx, al
		lds	si, [bp+p_palette]	;ds:si == pointer to color data
		mov	dx, 3C9h
		mov	cx, [bp+colors]		;# of colors

set_palette_loop1:
		lodsb
		out	dx, al
		lodsb
		out	dx, al
		lodsb
		out	dx, al
		loop set_palette_loop1

		pop	di
		pop	si
		pop	es
		pop	ds
		mov	sp, bp
		pop	bp
		retf
_set_palette	endp


_pic_copy	proc far

dst		= dword	ptr  6
x		= word ptr  0Ah
y		= word ptr  0Ch
dummy1		= dword ptr 0Eh
v1		= word ptr  12h
v2		= word ptr  14h
dummy2		= dword ptr 16h
w		= word ptr  1Ah
h		= word ptr  1Ch
src		= dword	ptr  1Eh
mode		= word ptr  22h

		push	bp
		mov	bp, sp
		sub	sp, 8h
		push	es
		push	di
		push	ds
		push	si

		les	di, [bp+dst]
		mov	ax, [bp+y]
		mov	cx, 320
		mul	cx
		add	ax, [bp+x]
		add	di, ax
		lds	si, [bp+src]

		cmp	[bp+mode], 1
		jnz	case2
		jmp	mode_1
case2:
		cmp	[bp+mode], 2
		jnz	case3
		jmp	mode_2
case3:
		cmp	[bp+mode], 3
		jnz	default
		jmp	mode_3
mode_1:
		mov	dx, [bp+h]
		mov	bx, 320
		sub	bx, [bp+w]
m1_h_loop:
		mov	cx, [bp+w]
m1_w_loop:
		mov	al, es:[di]
		cmp	al, 200
		jnb	m1_w_skip
		mov	al, [si]
		mov	es:[di], al
m1_w_skip:
		inc	si
		inc	di
		loop	m1_w_loop
		add	di, bx
		dec	dx
		jg	m1_h_loop
		jmp	l_leave

mode_2:
		mov	dx, [bp+h]
		mov	bx, 320
		sub	bx, [bp+w]
m2_h_loop:
		mov	cx, [bp+w]
m2_w_loop:
		mov	al, [si]
		cmp	al, 0
		jz	m2_w_skip
		mov	es:[di], al
m2_w_skip:
		inc	si
		inc	di
		loop	m2_w_loop
		add	di, bx
		dec	dx
		jg	m2_h_loop
		jmp	l_leave

mode_3:
		mov	ax, [bp+v2]
		mov	cx, 320
		mul	cx
		add	ax, [bp+v1]
		add	si, ax
		mov	dx, [bp+h]
		mov	bx, 320
		sub	bx, [bp+w]
		mov	ax, [bp+w]
m3_loop:
		mov	cx, ax
		rep	movsb
		add	di, bx
		add	si, bx
		dec	dx
		jg	m3_loop
		jmp	l_leave

default:
		mov	dx, [bp+h]
		mov	bx, 320
		sub	bx, [bp+w]
		mov	ax, [bp+w]
def_loop:
		mov	cx, ax
		rep	movsb
		add	di, bx
		dec	dx
		jg	def_loop

l_leave:
		pop	si
		pop	ds
		pop	di
		pop	es
		mov	sp, bp
		pop	bp
		retf
_pic_copy	endp


_fill_rect	proc far

pptr		= dword ptr  6
color		= byte ptr  0Ah
p_width		= word ptr  0Ch
height		= word ptr  0Eh

		push	bp
		mov	bp, sp
		push	ds
		push	es
		push	si
		push	di

		mov	ax, word ptr [bp+pptr]
		mov	es, ax
		mov	di, word ptr [bp+pptr+2]
		mov	dx, [bp+height]
		mov	bx, 320
		sub	bx, [bp+p_width]
		mov	cx, [bp+p_width]
		mov	al, [bp+color]

save_loop:
		push	cx
		rep	stosb
		pop	cx
		add	di, bx
		dec	dx
		jg	save_loop

		pop	di
		pop	si
		pop	es
		pop	ds
		mov	sp, bp
		pop	bp
		retf
_fill_rect	endp

_swap_u16	proc far

val		= word	ptr  6

		push	bp
		mov	bp, sp

		push	bx
		mov	bx, [bp+val]
		mov	ax, 0
		mov	al, bh
		mov	ah, bl
		pop	bx

		mov	sp, bp
		pop	bp
		retf

_swap_u16	endp


_copy_to_screen		proc far

v2		= word ptr -4
v1		= word ptr -2
src		= dword	ptr 6
dst		= dword	ptr 0Ah
p_width		= word ptr  0Eh
height		= word ptr  10h
mode		= word ptr  12h

		push	bp
		mov	bp, sp
		sub	sp, 4

		push	es
		push	di
		push	ds
		push	si

		les	di, [bp+dst]
		lds	si, [bp+src]
		mov	dx, [bp+height]
		mov	ax, 320
		sub	ax, [bp+p_width]
		mov	[bp+v1], ax
		mov	[bp+v2], ax

		mov	ax, [bp+mode]
		and	ax, 07fh
		cmp	ax, 0
		jnz	cts_if_1
		mov	[bp+v2], 0
		jmp	cts_if_exit
cts_if_1:
		cmp	ax, 2
		jnz	cts_if_exit
		mov	[bp+v1], 0
cts_if_exit:

		mov	ax, [bp+mode]
		and	ax, 80h
		jz	cts_loop1_condition
cts_loop1_body:
		mov	cx, [bp+p_width]
cts_loop2_body:
		mov	al, [si]
		cmp	al, 0
		jz	cts_loop2_condition
		mov	es:[di], al

cts_loop2_condition:
		inc	di
		inc	si
		loop	cts_loop2_body
		add	di, [bp+v1]
		add	si, [bp+v2]
		dec	dx
		jg	cts_loop1_body
		jmp	cts_leave

cts_loop1_condition:
		mov	cx, [bp+p_width]
		rep	movsb
		add	di, [bp+v1]
		add	si, [bp+v2]
		dec	dx
		jg	cts_loop1_condition
cts_leave:
		pop	si
		pop	ds
		pop	di
		pop	es

		mov	sp, bp
		pop	bp
		retf
_copy_to_screen	endp

_normalize_ptr	proc far

pnt		= dword ptr 6

		push bp
		mov bp, sp
		push ds
		push es
		push si
		push di

		xor	dx, dx
		mov	ax, word ptr [bp+pnt+2]
		mov	cx, 4
		call	far ptr F_LXLSH@
		mov 	word ptr [bp+pnt+2], 0
		add	ax, word ptr [bp+pnt]
		adc	dx, word ptr [bp+pnt+2]
		mov	cx, 0ch
		call	far ptr F_LXLSH@
		mov	ax, word ptr [bp+pnt]
		and	ax, 0fh

		pop	di
		pop	si
		pop	es
		pop	ds
		mov	sp, bp
		pop	bp
		retf

_normalize_ptr	endp

SEG005_TEXT	ends

	extrn F_LXLSH@:far
	public _save_display_stat
	public _set_palette
	public _pic_copy
	public _fill_rect
	public _swap_u16
	public _copy_to_screen
	public _normalize_ptr

	end
