#include "kernel.h"
#include <stdint.h>
#include <stddef.h>
#include "idt/idt.h"
#include "io/io.h"
#include "memory/heap/kheap.h"
#include "memory/paging/paging.h"

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

size_t strlen(const char* str){

    size_t len = 0;
    while(str[len]){
        len++;
    }
    return len;
}

void print(const char* str){

    size_t len = strlen(str);
    for (int i = 0; i < len; i++){
        terminal_writechar(str[i], 15);
    }
}

//extern void problem();

static struct paging_4gb_chunk* kernel_chunk = 0;
void kernel_main(){
    
    terminal_initialize();
    print("Hello World!\nThis is a new line bitch");

    // Initialize heap memory
    kheap_init();

    //Initialize the interrup
    idt_init();

    // Setup paging
    kernel_chunk = paging_new_4gb(PAGING_IS_WRITEABLE | PAGING_IS_PRESENT | PAGING_ACCESS_FROM_ALL);

    //Switch to kernel paging chunk
    paging_switch(paging_4gb_chunk_get_directory(kernel_chunk));

    char* ptr = kzalloc(4096); 
    paging_set(paging_4gb_chunk_get_directory(kernel_chunk), (void*)0x1000, (uint32_t)ptr | PAGING_ACCESS_FROM_ALL | PAGING_IS_PRESENT | PAGING_IS_WRITEABLE);


    enable_paging();

    char* ptr2 = (char*) 0x1000;
    ptr2[0] = 'A';
    ptr2[1] = 'B';
    print(ptr2);

    print(ptr);

    


    // Enable interrupts
    enable_interrupts();


    // Testing memory heap
    // void* ptr = kmalloc(50);
    // void* ptr2 = kmalloc(5000);
    // void* ptr3 = kmalloc(5000);
    
    // kfree(ptr);

    // void* ptr4 = kmalloc(50);

    // if (ptr || ptr2 || ptr3 || ptr4){

    // }

    //problem();

    // 
    // outb(0x60, 0xff)
    //terminal_writechar('A', 15);

    //video_mem[0] = terminal_make_char('B', 15);

    // video_mem[0] = 0x0341; // 0x41=first byte, 0x03=second (char/color)
    // char* video_mem = (char*)(0xB8000); // Pointer
    // video_mem[0] = 'A';
    // video_mem[1] = 3;
    // video_mem[2] = 'C';
    // video_mem[3] = 3;
}