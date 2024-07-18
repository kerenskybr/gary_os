#include "kernel.h"
#include <stdint.h>
#include <stddef.h>
#include "idt/idt.h"
#include "io/io.h"
#include "memory/heap/kheap.h"
#include "memory/paging/paging.h"
#include "disk/disk.h"
#include "fs/pparser.h"
#include "string/string.h"
#include "disk/streamer.h"
#include "fs/file.h"
#include "gdt/gdt.h"
#include "config.h"
#include "memory/memory.h"
#include "task/tss.h"
#include "task/task.h"
#include "task/process.h"
#include "status.h"

uint16_t* video_mem = 0; //(uint16_t*)(0xB8000);
uint16_t terminal_row = 0;
uint16_t terminal_col = 0;

uint16_t terminal_make_char(char c, char colour){
    // Shift color to the left 8 bits
    return (colour << 8) | c;
}

void terminal_putchar(int x, int y, char c, char colour){

    video_mem[(y * VGA_WIDTH) + x] = terminal_make_char(c, colour);
}

void terminal_writechar(char c, char colour){

    if (c == '\n'){

        terminal_row +=1;
        terminal_col = 0;
        return;
    }
    terminal_putchar(terminal_col, terminal_row, c, colour);
    terminal_col +=1;
    if (terminal_col >= VGA_WIDTH){

        terminal_col = 0;
        terminal_row += 1;
    }
}

void terminal_initialize(){

    video_mem = (uint16_t*)(0xB8000);
    terminal_col = 0;
    terminal_row = 0;

    for (int y=0; y < VGA_HEIGHT; y++){
        for (int x=0; x < VGA_WIDTH; x++){
            //video_mem[(y * VGA_WIDTH) + x] = terminal_make_char(' ', 0);
            terminal_putchar(x, y, ' ', 0);
        }
    }
}

// size_t strlen(const char* str){

//     size_t len = 0;
//     while(str[len]){
//         len++;
//     }
//     return len;
// }

void print(const char* str){

    size_t len = strlen(str);
    for (int i = 0; i < len; i++){
        terminal_writechar(str[i], 15);
    }
}

//extern void problem();

static struct paging_4gb_chunk* kernel_chunk = 0;

void panic(const char* msg){

    print(msg);
    while(1) {};
}

void kernel_page(){

    kernel_registers();
    paging_switch(kernel_chunk);
}

struct tss tss;
struct gdt gdt_real[GARYOS_TOTAL_GDT_SEGMENTS];
struct gdt_structured gdt_structured[GARYOS_TOTAL_GDT_SEGMENTS] = {

    {.base = 0x00, .limit = 0x00, .type = 0x00},                    //Null segments
    {.base = 0x00, .limit = 0xffffffff, .type = 0x9a},              //Kernel code sgment
    {.base = 0x00, .limit = 0xffffffff, .type = 0x92},              //kernel data
    {.base = 0x00, .limit = 0xffffffff, .type = 0xf8},              // user code
    {.base = 0x00, .limit = 0xffffffff, .type = 0xf2},              // Userdata seg
    {.base = (uint32_t)&tss, .limit = sizeof(tss), .type = 0xE9}    //TSS seg
};

void kernel_main(){
    
    terminal_initialize();
    print("Hello World!\nThis is a new line bitch");

    //panic("\nTEsting kernel panic!\n");

    memset(gdt_real, 0x00, sizeof(gdt_real));
    gdt_structured_to_gdt(gdt_real, gdt_structured, GARYOS_TOTAL_GDT_SEGMENTS);
   
    // Load the gdt
    gdt_load(gdt_real, sizeof(gdt_real));

    // Initialize heap memory
    kheap_init();

    // Initialize file system
    fs_init();

    // Search and initialize disks
    disk_search_and_init();

    //Initialize the interrup
    idt_init();

    // Setup tss
    memset(&tss, 0x00, sizeof(tss));
    tss.esp0 = 0x600000;
    tss.ss0 = KERNEL_DATA_SELECTOR;

    // Load tss
    tss_load(0x28);

    // Setup paging
    kernel_chunk = paging_new_4gb(PAGING_IS_WRITEABLE | PAGING_IS_PRESENT | PAGING_ACCESS_FROM_ALL);

    //Switch to kernel paging chunk
    paging_switch(kernel_chunk);

    enable_paging();
    
    struct process* process = 0;
    int res = process_load("0:/blank.bin", &process);
    if (res != GARYOS_ALL_OK){

        panic("\nFailed to load blank.bin dummy\n");
    }

    task_run_first_ever_task();

    while(1){}

}