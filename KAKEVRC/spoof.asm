PUBLIC _spoofer_stub
PUBLIC _silent_crash     
.code
     
_spoofer_stub PROC
    pop r11
    add rsp, 8 
    mov rax, [rsp + 24] 
        
    mov r10, [rax] 
    mov [rsp], r10 
        
    mov r10, [rax + 8]
    mov [rax + 8], r11 
     
    mov [rax + 16], rbx 
    lea rbx, fixup
    mov [rax], rbx 
    mov rbx, rax 
        
    jmp r10 
     
fixup:
    sub rsp, 16
    mov rcx, rbx 
    mov rbx, [rcx + 16]
    jmp QWORD PTR [rcx + 8] 
_spoofer_stub ENDP

_silent_crash PROC
	mov rax, 0
	mov rbx, 0
	mov rdx, 0
	mov rsi, 0
	mov rdi, 0
	mov rbp, 0
	mov rsp, 0
	mov r8, 0
	mov r9, 0
	mov r10, 0
	mov r11, 0
	mov r12, 0
	mov r13, 0
	mov r14, 0
	mov r15, 0
	jmp rcx
_silent_crash ENDP
  
END