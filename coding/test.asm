[bits 16]
[org 0x7c00]

start:
	mov si, some_text
	call typeIt

	jmp short start

typeIt:
	mov ah, 0x0e
	mov bx, 0x000a
	int 0x10
	.again:
		cmp al, 0x0
		je .endTyper
		jmp short typeIt

	.endTyper:
		ret

some_text db "Hello youtube.com", 0x0d, 0x0a, 0x0

times 510-($-$$) db 0
dw 0xAA55