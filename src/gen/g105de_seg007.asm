_FARDATA	segment byte public 'FAR_DATA'

; generating the request sections
; a request section has 28/0x1c byte

_req db 16h, 0, 85h	; starts at 0x000
     dw 0000h		; status
     db 8 dup (0)	; dummy2[8]
     db 0h		; redbook
     dd 0h		; ptr
     dw 0000h, 0000h	; dummy4, dummy6
     db 6 dup (0)       ; dummy7[6]

req1 db 16h, 0, 85h	; starts at 0x01c
     dw 0000h		; status
     db 8 dup (0)	; dummy2[8]
     db 1h		; redbook
     dd 0h		; ptr
     dw 0000h, 0000h	; dummy4, dummy6
     db 6 dup (0)       ; dummy7[6]

req2 db 1ah, 0, 03h	; starts at 0x038
     dw 0000h		; status
     db 8 dup (0)	; dummy2[8]
     db 0h		; redbook
     dd 0h		; ptr
     dw 0007h, 0000h	; dummy4, dummy6
     db 6 dup (0)       ; dummy7[6]

req3 db 1ah, 0, 03h	; starts at 0x054
     dw 0000h		; status
     db 8 dup (0)	; dummy2[8]
     db 0h		; redbook
     dd 0h		; ptr
     dw 000bh, 0000h	; dummy4, dummy6
     db 6 dup (0)       ; dummy7[6]

req4 db 18h, 0, 83h	; starts at 0x070
     dw 0000h		; status
     db 8 dup (0)	; dummy2[8]
     db 0h		; redbook
     dd 0h		; ptr
     dw 0000h, 0002h	; dummy4, dummy6
     db 6 dup (0)       ; dummy7[6]

req5 db 16h, 0, 84h	; starts at 0x08c
     dw 0000h		; status
     db 8 dup (0)	; dummy2[8]
     db 1h		; redbook
     dd 0h		; ptr
     dw 0000h, 0000h	; dummy4, dummy6
     db 6 dup (0)       ; dummy7[6]

req6 db 16h, 0, 85h	; starts at 0x0a8
     dw 0000h		; status
     db 8 dup (0)	; dummy2[8]
     db 1h		; redbook
     dd 0h		; ptr
     dw 0000h, 0000h	; dummy4, dummy6
     db 6 dup (0)       ; dummy7[6]

req7 db 16h, 0, 88h	; starts at 0x0c4
     dw 0000h		; status
     db 8 dup (0)	; dummy2[8]
     db 1h		; redbook
     dd 0h		; ptr
     dw 0000h, 0000h	; dummy4, dummy6
     db 6 dup (0)       ; dummy7[6]

req8 db 16h, 0, 0ch	; starts at 0x0e0
     dw 0000h		; status
     db 8 dup (0)	; dummy2[8]
     db 0h		; redbook
     dd 0cc70094h	; ptr
     dw 0001h, 0000h	; dummy4, dummy6
     db 6 dup (0)       ; dummy7[6]

;;GEN_REQ 16h, 85h, 0h, 00h, 0h		;;STOP CDA HSG-mode
;;GEN_REQ 16h, 85h, 1h, 00h, 0h		;;STOP CDA Redbook-mode
;;GEN_REQ 1ah, 03h, 0h, 07h, 0h		;;IOCTL input
;;GEN_REQ 1ah, 03h, 0h, 0bh, 0h		;;IOCTL input
;;GEN_REQ 18h, 83h, 0h, 00h, 2h		;;SEEK (not used)
;;GEN_REQ 16h, 84h, 1h, 00h, 0h		;;PLAY CDA Redbook
;;GEN_REQ 16h, 85h, 1h, 00h, 0h		;;STOP CDA Redbook
;;GEN_REQ 16h, 88h, 1h, 00h, 0h		;;RESUME CDA Redbook
;;GEN_REQ 16h, 0ch, 0h, 01h, 0h		;;?_w


_cd_buf1 db 804 dup (0)			;another buffer starts at 0x0fc
_cd_buf2 db 20 dup (0)			;buffer for IOCTL

_FARDATA	ends

	public _req	; array of 9 requests
	public _cd_buf1
	public _cd_buf2

	end
