;
;	Rewrite of DSA1 v3.02_de functions of seg010 (EMS)
;	Functions rewritten: 8/8 (complete)
;
;	Borlandified and identical
;	Assembler:	TASM 3.1
;	Call:		TASM.EXE /os /z SEG010.asm SEG010.OBJ
;

;	TODO:	- Two calls to F_LXULSH
;		- Adresses of variables

SEG010_TEXT	segment byte public 'CODE'
SEG010_TEXT	ends

DGROUP	group _DATA
	assume cs:SEG010_TEXT, ds:DGROUP

_DATA	segment word public 'DATA'
_DATA	ends

SEG010_TEXT	segment byte public 'CODE'

	assume cs:SEG010_TEXT

EMS_installed PROC NEAR
	push bp
	mov bp,sp
	push ds
	push es
	push si
	push di
	mov ax, 03567h
	int 21h
	mov di, 0ah
	mov si, OFFSET _g_emm_sig
	mov cx, 9
_comp:	cmpsb
	loope _comp
	xor ax,ax
	or cx,cx
	jnz _done
	mov ax, 1
_done:
	pop di
	pop si
	pop es
	pop ds
	mov sp,bp
	pop bp
	ret
EMS_installed ENDP

EMS_get_frame_ptr PROC NEAR
	push bp
	mov bp,sp
	push ds
	push es
	push si
	push di
	mov ax, 4100h
	int 67h
	or ah,ah
	jz _done1
	xor bx,bx
_done1:	mov dx,bx
	xor ax,ax
	pop di
	pop si
	pop es
	pop ds
	mov sp,bp
	pop bp
	ret
EMS_get_frame_ptr ENDP

_EMS_get_num_pages_unalloced PROC FAR
	push bp
	mov bp,sp
	push ds
	push es
	push si
	push di
	mov ax, 4200h
	int 67h
	or ah,ah
	jz _done2
	xor bx,bx
_done2:	mov ax,bx
	pop di
	pop si
	pop es
	pop ds
	mov sp,bp
	pop bp
	retf
_EMS_get_num_pages_unalloced ENDP

_EMS_alloc_pages PROC FAR
	push bp
	mov bp,sp
	push ds
	push es
	push si
	push di
	mov ax, 4300h
	mov bx,[bp+6h]
	int 67h
	or ah,ah
	jz _done3
	xor dx,dx
_done3:
	mov ax,dx
	pop di
	pop si
	pop es
	pop ds
	mov sp,bp
	pop bp
	retf
_EMS_alloc_pages ENDP

_EMS_free_pages PROC FAR
	push bp
	mov bp,sp
	push ds
	push es
	push si
	push di
	mov ax, 4500h
	mov dx,[bp+6h]
	int 67h
	mov al,ah
	pop di
	pop si
	pop es
	pop ds
	mov sp,bp
	pop bp
	retf
_EMS_free_pages ENDP

_EMS_map_memory PROC FAR
	push bp
	mov bp,sp
	push ds
	push es
	push si
	push di
	mov ax, 4400h
	mov al,[bp+0ah]
	mov bx,[bp+08h]
	mov dx,[bp+06h]
	int 67h
	mov al,ah
	pop di
	pop si
	pop es
	pop ds
	mov sp,bp
	pop bp
	retf
_EMS_map_memory ENDP

_EMS_norm_ptr PROC FAR
	push bp
	mov bp,sp
	push ds
	push es
	push si
	push di
	xor dx,dx
	mov ax, [bp+8h]
	mov cx, 4

	call far ptr F_LXLSH@

	mov [bp+8], word ptr 0h
	add ax,[bp+6]
	adc dx,[bp+8]
	mov cx, 0ch

	call far ptr F_LXLSH@

	mov ax,[bp+6]
	and ax, 0fh
	pop di
	pop si
	pop es
	pop ds
	mov sp,bp
	pop bp
	retf
_EMS_norm_ptr ENDP

_EMS_init PROC FAR
	push bp
	mov bp,sp
	push ds
	push es
	push si
	push di
	call EMS_installed
	or ax,ax
	jz _done4
	call EMS_get_frame_ptr

	mov word ptr [_g_ems_frame_ptr + 2], dx

	mov word ptr [_g_ems_frame_ptr], word ptr 0h

	mov ax, 1
_done4:
	pop di
	pop si
	pop es
	pop ds
	mov sp,bp
	pop bp
	retf
_EMS_init ENDP

SEG010_TEXT ends

	public _EMS_get_num_pages_unalloced
	public _EMS_alloc_pages
	public _EMS_free_pages
	public _EMS_map_memory
	public _EMS_norm_ptr
	public _EMS_init
	extrn _g_emm_sig:byte; 0x4ba2
	extrn _g_ems_frame_ptr:dword; off (0x4baa), seg (0x4bac)
	extrn F_LXLSH@:far

	end
