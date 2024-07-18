#include "idt.h"
#include "config.h"
#include "kernel.h"
#include "memory/memory.h"
#include "task/task.h"
#include "io/io.h"

struct idt_desc idt_descriptors[GARYOS_TOTAL_INTERRUPTS];
struct idtr_desc idtr_descriptor;

static ISR80H_COMMAND isr80h_commands[GARYOS_MAX_ISR80H_COMMANDS];

extern void idt_load(struct idtr_desc* ptr);
extern void init21h();
extern void no_interrupt();
extern void isr80h_wrapper();

void init21h_handler(){
    
    print("\nKey pressed\n");
    outb(0x20, 0x20);
}

void no_interrupt_handler(){

    outb(0x20, 0x20);
}

void idt_zero(){

    print("\nDivide by zero error\n");
}

void idt_set(int interrupt_no, void* address){

    struct idt_desc* desc = &idt_descriptors[interrupt_no]; // "&" gets the memory addr of the variable
    desc->offset_1 = (uint32_t) address & 0x0000ffff;
    desc->selector = KERNEL_CODE_SELECTOR;  // CODE_SEG
    desc->zero = 0x00;
    desc->type_attr = 0xEE;
    desc->offset_2 = (uint32_t) address >> 16;
}

void idt_init(){

    memset(idt_descriptors, 0, sizeof(idt_descriptors));
    idtr_descriptor.limit = sizeof(idt_descriptors) -1;
    idtr_descriptor.base = (uint32_t) idt_descriptors;

    for (int i=0; i < GARYOS_TOTAL_INTERRUPTS; i++){

        idt_set(i, no_interrupt);
    }

    idt_set(0, idt_zero);
    idt_set(0x21, init21h);
    idt_set(0x80, isr80h_wrapper);

    // Load the interrupt desc table
    idt_load(&idtr_descriptor);
}

void isr80h_register_command(int command_id, ISR80H_COMMAND command){

    if (command_id <= 0 || command_id >= GARYOS_MAX_ISR80H_COMMANDS){

        panic("\nDon't you see the command is out of bounds idiot?\n");
    }

    if (isr80h_commands[command_id]){

        panic("\nAttemping to overwrite an existing command. Stop b4 i stab you.\n");
    }

    isr80h_commands[command_id] = command;
}

void* isr80h_handle_command(int command, struct interrupt_frame* frame){

    void* result = 0;
    if (command <= 0 || command >= GARYOS_MAX_ISR80H_COMMANDS){

        return 0;
    }

    ISR80H_COMMAND command_func = isr80h_commands[command];
    if (!command_func){

        return 0;
    }

    result = command_func(frame);
    return result;
}

void* isr80h_handler(int command, struct interrupt_frame* frame){

    void* res = 0;
    kernel_page();

    task_current_save_state(frame);
    res = isr80h_handle_command(command, frame);
    task_page();
    return res;
}