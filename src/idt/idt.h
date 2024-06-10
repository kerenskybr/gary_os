#ifndef IDT_H
#define IDT_H
#include <stdint.h>

// https://wiki.osdev.org/Interrupt_Descriptor_Table
struct idt_desc{

    uint16_t offset_1;      //Offset bits 0 - 15
    uint16_t selector;      //Selector that is in our GDT
    uint8_t zero;           // Does nothing, unused set to zero
    uint8_t type_attr;      // Descriptor type and attributes
    uint16_t offset_2;      // Offset bits 16-31
} __attribute__((packed));   // Pack instructs tigheter tightly in memory

struct idtr_desc{
    
    uint16_t limit;         // Size of descriptor table -1
    uint32_t base;          // BAse addr of the start of interrup desc table
} __attribute__((packed));   

void idt_init();

#endif