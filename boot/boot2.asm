;
; Stage 2 of the bootloader
; This really doesn't do much besides jump to C.
;

[bits 32]

extern kernel_main
global _start
_start:
    call kernel_main
	hlt
