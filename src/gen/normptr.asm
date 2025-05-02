; Segment type:	Pure code


SEG005_TEXT	segment byte public 'CODE'
SEG005_TEXT	ends

DGROUP	group _DATA
	assume cs:SEG005_TEXT,ds:DGROUP

_DATA		segment word public 'DATA'
_DATA		ends

SEG005_TEXT	segment byte public 'CODE'

	assume	cs:SEG005_TEXT

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
	public _normalize_ptr

	end
