;[BITS 32]
;
; An initial example of the terrible ota file system :)
;
dw 0x0FAB       ; The signature
dw 2            ; Number of objects in the root directory

;; File 1- our executable program
dw 0b0001000000000000                   ; The type: TODO: This needs to be changed
dw FILE1_EXE_END - FILE1_EXE_START      ; The size
dw 10                                   ; File name length
db "file1.exe",0                        ; The file name
FILE1_EXE_START:
    int 0x30
    ;mov eax, 5
    ret
FILE1_EXE_END:

;;
;; File 2- our text file
;;
dw 0b0001000000000000                   ; The type: TODO: This needs to be changed
dw HELLO_TXT_END - HELLO_TXT_START
dw 10
db "hello.txt",0
HELLO_TXT_START:
    db "Hello!",0xA
    db "How are you today?",0xA
    db 0
HELLO_TXT_END:
