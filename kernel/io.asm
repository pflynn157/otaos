
extern keyboard_handler
global keyboard_handler_int
keyboard_handler_int:
    pushad
    cld
    call keyboard_handler
    popad
    iretd
    
extern software_handler
global software_handler_int
software_handler_int:
    pushad
    cld
    push ebx
    call software_handler
    pop ebx
    popad
    iretd
    
global software_int_test1
software_int_test1:
    mov ebx, 1
    push ebx
    int 0x30
    pop ebx
    ret
    
global software_int_test2
software_int_test2:
    mov ebx, 2
    push ebx
    int 0x30
    pop ebx
    ret
    
global software_int_test3
software_int_test3:
    mov ebx, 3
    push ebx
    int 0x30
    pop ebx
    ret

global read_port
read_port:
    mov edx, [esp+4]
    in al, dx
    ret
    
global write_port
write_port:
    mov edx, [esp+4]
    mov al, [esp+4+4]
    out dx, al
    ret
    
extern gdtPointer
global loadGdt
loadGdt:
    lgdt [gdtPointer]
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x08:fload
fload:
    ret
    
global loadTss
loadTss:
    mov ax, 0x2B
    ltr ax
    ret
    
