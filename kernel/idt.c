#include <stdio.h>
#include <cpu/idt.h>
#include <cpu/io.h>

// Forward declarations of needed structures
__attribute__((aligned(0x10)))
static idt_entry_t idt[256];
static idtr_t idtr;

// The general exception handler
// For now, just hang the computer
__attribute__((noreturn))
void exception_handler(int num);
void exception_handler(int num) {
    if (num == 0x10) kprintf("Bad TSS");
    kprintf("IDT Error\n");
    asm volatile("cli; hlt");
}

// Used to setup descriptors
void idt_set_descriptor(uint8_t vector, void *isr, uint8_t flags) {
    idt_entry_t *descriptor = &idt[vector];
    
    descriptor->isr_low = (uint32_t)isr & 0xFFFF;
    descriptor->kernel_cs = 0x08;
    descriptor->attributes = flags;
    descriptor->isr_high = (uint32_t)isr >> 16;
    descriptor->reserved = 0;
}

// INIT the interrupt descriptor table
void idt_init() {
    idtr.base = (uintptr_t)&idt[0];
    idtr.limit = (uint16_t)sizeof(idt_entry_t) * IDT_MAX_DESCRIPTORS - 1;
    
    for (uint8_t vector = 0; vector < 32; vector++) {
        idt_set_descriptor(vector, isr_stub_table[vector], 0x8E);
    }
    
    asm volatile("lidt %0" : : "m"(idtr));
    asm volatile("sti");
}
