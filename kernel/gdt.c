#include <cpu/gdt.h>
#include <string.h>

struct {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) gdtPointer;

static gdtEntry_t gdt[6];
tss_entry_t tss;

void gdtSetEntry(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
    gdt[num].lbase = (base & 0xFFFF);
    gdt[num].mbase = (base >> 16) & 0xFF;
    gdt[num].hbase = (base >> 24) & 0xFF;
    gdt[num].limit = (limit & 0xFFFF);
    gdt[num].granularity = (limit >> 16) & 0x0F;
    gdt[num].granularity |= (gran & 0xF0);
    gdt[num].access = access;
}

void gdtInstallTSS(int num, uint16_t ss0, uint32_t esp0) {
    uintptr_t base = (uintptr_t)&tss;
    uintptr_t limit = base + sizeof(tss);
    
    gdtSetEntry(num, base, limit, 0xE9, 0x00);
    memset(&tss, 0x0, sizeof(tss));
    
    tss.ss0 = ss0;
    tss.esp0 = esp0;
    tss.cs = 0x0B;
    tss.ss = 0x13;
    tss.ds = 0x13;
    tss.es = 0x13;
    tss.fs = 0x13;
    tss.gs = 0x13;
    tss.iomap_base = sizeof(tss);
}

void setupGDT() {
    gdtPointer.limit = (sizeof(gdtEntry_t) * 6) - 1;
    gdtPointer.base = &gdt;
    
    gdtSetEntry(0, 0, 0, 0, 0);
    gdtSetEntry(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);    // Code segment
    gdtSetEntry(2, 0, 0xFFFFFFFF, 0x92, 0xCF);    // Data segment
    gdtSetEntry(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);    // User code
    gdtSetEntry(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);    // User data
    gdtInstallTSS(5, 0x10, 0);                    // TSS
    loadGdt();
    loadTss();
}

