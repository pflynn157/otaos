[org 0x7C00]
[bits 16]

boot_start:
    mov bp, 0x8000
    mov sp, bp
    
    mov eax, 0
.load_kernel:           ; A loop to read the next segment of the kernel into memory
    inc eax
    push eax
    mov bx, 0x1000      ; Memory location we are reading to
    mov al, 20          ; Number of sectors to read
    mov ch, 0           ; Cylinder number
    mov dh, 0           ; Head number
    mov cl, 2           ; Sector number
    call read_drive
    pop eax
    cmp eax, 3
    jle .load_kernel
    call do_e820
    jmp enable_protected
    hlt
    
read_drive:
    pusha
    mov ah, 02
    mov dl, 0x80
    int 0x13
    popa
    ret
    
%include "boot/memory.asm"
    
;
; The GDT- global descriptor table
;
; This is a temporary GDT just to help us get things going and into protected mode.
; Once the kernel is up and running, we will reload the GDT
;
GDT_start:
    null_descriptor:
        dd 0
        dd 0
    code:
        dw 0xFFFF
        dw 0
        db 0
        db 10011010b
        db 11001111b
        db 0
    data:
        dw 0xFFFF
        dw 0
        db 0
        db 10010010b
        db 11001111b
        db 0
GDT_end:
GDT1:
    dw GDT_end - GDT_start - 1
    dd GDT_start
    
CODE_SEGMENT equ code - GDT_start
DATA_SEGMENT equ data - GDT_start

;
; Enables protected mode.
; Our last foray into 16-bit land...
;
[bits 16]    
enable_protected:
    cli
    lgdt [GDT1]
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    jmp CODE_SEGMENT:pm_entry       ; Do the far jump into protected mode
    hlt
    
;
; Sets the segments and calls the kernel
;
[bits 32]
pm_entry:
    mov ax, DATA_SEGMENT
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    
    ; Sets up the stack for the kernel
    mov ebp, 0x90000
    mov esp, ebp
    mov edi, 0x8000
    call 0x1000
    hlt
    
;KERNEL_ENTRY 0x1000
    
; Boot signature
times 510 - ($ - $$) db 0
dw 0xAA55

