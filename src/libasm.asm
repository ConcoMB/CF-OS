GLOBAL  _read_msw,_lidt
GLOBAL  _int_08_hand
GLOBAL  _int_80_hand
GLOBAL  _int_09_hand
GLOBAL  _int_0E_hand
GLOBAL  _scheduler
GLOBAL  _IO_in
GLOBAL	_IO_out
GLOBAL  _IO_out_w
GLOBAL  _IO_in_w
GLOBAL  __write
GLOBAL  __read
GLOBAL  __readNB
GLOBAL  __hour
GLOBAL  __min
GLOBAL  __setcolor
GLOBAL  _mascaraPIC1,_mascaraPIC2,_Cli,_Sti
GLOBAL  _debug
GLOBAL __malloc
GLOBAL __calloc
GLOBAL __free
GLOBAL __heap_count
GLOBAL  __set_scancode
GLOBAL __kill
GLOBAL __top
GLOBAL _lcr3
GLOBAL _epag
GLOBAL _fill_page1
GLOBAL __stack_count
GLOBAL _sys_stack_count
GLOBAL _sys_yield
GLOBAL __sleep
GLOBAL __createChild
GLOBAL  _port_in, _port_out, _portw_in, _portw_out

EXTERN  getIP
EXTERN  printIdleStack
EXTERN  int_08
EXTERN  eokl
EXTERN  int_09
EXTERN  int_80
EXTERN  page_fault
EXTERN  getStack
EXTERN  saveStack
EXTERN  getNextProcess
EXTERN  getIdleStack
EXTERN  printStack


SECTION .text


_Cli:
	cli			; limpia flag de interrupciones
	ret

_Sti:

	sti			; habilita interrupciones por flag
	ret

_mascaraPIC1:			; Escribe mascara del PIC 1
	push    ebp
        mov     ebp, esp
        mov     ax, [ss:ebp+8]  ; ax = mascara de 16 bits
        out	21h,al
        pop     ebp
        retn

_mascaraPIC2:			; Escribe mascara del PIC 2
	push    ebp
        mov     ebp, esp
        mov     ax, [ss:ebp+8]  ; ax = mascara de 16 bits
        out	0A1h,al
        pop     ebp
        retn

_read_msw:
        smsw    ax		; Obtiene la Machine Status Word
        retn


_lidt:				; Carga el IDTR
        push    ebp
        mov     ebp, esp
        push    ebx
        mov     ebx, [ss: ebp + 6] ; ds:bx = puntero a IDTR 
	rol	ebx,16		    	
	lidt    [ds: ebx]          ; carga IDTR
        pop     ebx
        pop     ebp
        retn


_epag:
		mov eax, cr0
		or eax, 80000000h
		mov cr0, eax
		ret
		
		
_int_08_hand:				; Handler de INT 8 ( Timer tick)
        
		cli
  		pushad
		mov eax, esp
		push eax
		call saveStack
		pop eax
		call getIdleStack
		mov esp, eax
		call getNextProcess
		push eax
		call getStack
		pop ebx
		mov esp,eax

		mov	al,20h			; Envio de EOI generico al PIC
		out	20h,al
		popad 
		 
	
		;mov esp,ebp
		;pop ebp
		sti
		iret

_int_80_hand:				; Handler de INT 80 ( System calls)
		cli
        push ebp
		mov ebp,esp
        pusha
        call    int_80
		mov esp,ebp
		pop ebp
        
        iret
        
_int_09_hand:				; Handler de INT 09 (Teclado)
		cli
        push ebp
		mov ebp,esp
		pusha 

        call    int_09
		mov	al,20h			; Envio de EOI generico al PIC
		out	20h,al
		
		popa
		mov esp,ebp
		pop ebp
		sti
        iret
        
_int_0E_hand:				; Handler de INT 14 (PAGE FAULT)

        mov eax, cr2
        push eax
        call page_fault
        iret
        
;SYSTEM CALLS

__write:
	mov ecx, [esp+8]
	mov edx, [esp+4]
	mov ebx, 1
	int 080h
	ret

__read:
	mov ecx, [esp+8]
	mov ebx, 2
	int 080h
	ret

__readNB:
	mov ecx, [esp+8]
	mov ebx, 20
	int 080h
	ret
	
__hour:
	mov ecx, [esp+4]
	mov ebx, 3
	int 080h
	ret
	
__min:
	mov ecx, [esp+4]
	mov ebx, 4
	int 080h
	ret

__setcolor:
	mov ecx, [esp+4]
	mov ebx, 5
	int 080h
	ret

__malloc:
	mov ebx, 6
	mov ecx, [esp+4]
	int 080h
	ret
	
__calloc:
	mov ebx, 7
	mov ecx, [esp+4]
	int 080h
	ret
	
__free:
	mov ebx, 8
	mov ecx, [esp+4]
	int 080h
	ret
	
__heap_count:
	mov ebx, 9
	int 080h
	ret
	
__set_scancode:
	mov ebx, 10
	mov ecx, [esp+4]
	int 080h
	ret

__stack_count:
	mov ebx, 11
	int 080h
	ret

__kill:
	mov ebx, 12
	mov ecx, [esp+4]
	int 080h
	ret

__top:
	mov ebx, 13
	mov ecx, [esp+4]
	int 080h
	ret

__sleep:
	mov ebx, 14
	mov ecx, [esp+4]
	int 080h
	ret

__createChild:
  mov ebx, 15
  mov eax, [esp+12]
  mov ecx, [esp+8]
  mov edx, [esp+4]
  int 080h
  ret
	
_sys_stack_count:
	mov eax, eokl
	mov ebx, esp
	sub eax,ebx
	ret

_sys_yield:
	;call _scheduler
	sti
	int 08
	ret

_lcr3:
	;mov eax, [esp+4]
	mov eax, 00200000h 
	mov cr3, eax
	ret



; Mapeo 1:1 de la primer pagina
_fill_page1:

	 mov eax, 0
	 mov ebx, 0
	 .fill_table:
		  mov ecx, ebx
		  cmp eax, 530
		  jg .notpresent
		  or ecx, 1
.notpresent:  mov [201000h+eax*4], ecx
		  add ebx, 4096
		  inc eax
		  cmp eax, 1024
		  je .end
		  jmp .fill_table
	 .end:
	 ret

	
; Debug para el BOCHS, detiene la ejecució; Para continuar colocar en el BOCHSDBG: set $eax=0

_debug:
        push    bp
        mov     bp, sp
        push	ax
vuelve:	mov     ax, 1
        cmp	ax, 0
	jne	vuelve
	pop	ax
	pop     bp
        retn






_IO_in:
	mov dx, [esp+4]
	in byte al, dx
	ret
	
_IO_out:
	mov al, [esp+8]
	mov dx, [esp+4]
	out dx,al
	ret

_IO_out_w:
	mov ax, [esp+8]
	mov dx, [esp+4]
	out word dx,ax
	ret

_IO_in_w:
	mov dx, [esp+4]
	in word ax, dx
	ret

;=================================================================
;					PORT_IN
;=================================================================
_port_in:
	push ebp
	mov ebp, esp
	push dx	

	mov eax, 0
	mov dx, [ebp+8]
	in al, dx
	
	pop dx
	leave
	ret


;=================================================================
;					PORT_OUT
;=================================================================

_port_out:
	push ebp
	mov ebp, esp
	pusha

	mov dx, [ebp+8]
	mov ax, [ebp+12]
	out dx, al
	
	popa
	leave
	ret



;=================================================================
;					PORTW_IN
;=================================================================
_portw_in:
	push ebp
	mov ebp, esp
	push dx	

	mov eax, 0
	mov dx, [ebp+8]
	in ax, dx
	
	pop dx
	leave
	ret


;=================================================================
;					PORTW_OUT
;=================================================================

_portw_out:
	push ebp
	mov ebp, esp
	pusha

	mov dx, [ebp+8]
	mov ax, [ebp+12]
	out dx, ax
	
	popa
	leave
	ret

