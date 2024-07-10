#ifndef GDT_H
#define GDT_H
#include <stdint.h>

// represent the gdt code con boot.asm
struct gdt{

    uint16_t segment;
    uint16_t base_first;
    uint8_t base;
    uint8_t access;
    uint8_t high_flags;
    uint8_t base_24_31_bits;
};

struct gdt_structured{

    uint32_t base;
    uint32_t limit;
    uint32_t type;
};

void gdt_load(struct gdt* gdt, int size);
void gdt_structured_to_gdt(struct gdt* gdt, struct gdt_structured* structure_gdt, int total_entires);

#endif