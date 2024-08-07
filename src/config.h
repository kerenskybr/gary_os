#ifndef CONFIG_H
#define CONFIG_H

#define KERNEL_CODE_SELECTOR 0x08
#define KERNEL_DATA_SELECTOR 0x10

#define GARYOS_TOTAL_INTERRUPTS 512 //0x200
#define GARYOS_HEAP_SIZE_BYTES 104857600 // 100mb
#define GARYOS_HEAP_BLOCK_SIZE 4096 
#define GARYOS_HEAP_ADDRESS 0x01000000 // https://wiki.osdev.org/Memory_Map_(x86) 
#define GARYOS_HEAP_TABLE_ADDRESS 0x00007E00

// Disk controller stuff
#define GARYOS_SECTOR_SIZE 512

#define GARYOS_MAX_FILESYSTEMS 12
#define GARYOS_MAX_FILE_DESCRIPTORS 512
#define GARYOS_MAX_PATH 108

#define GARYOS_TOTAL_GDT_SEGMENTS 6
#define GARYOS_PROGRAM_VIRTUAL_ADDRESS 0x400000

#define GARYOS_PROGRAM_VIRTUAL_STACK_ADDRESS_START 0x3FF000
#define GARYOS_USER_PROGRAM_STACK_SIZE 1024 * 16
#define GARYOS_PROGRAM_VIRTUAL_STACK_ADDRESS_END GARYOS_PROGRAM_VIRTUAL_STACK_ADDRESS_START - GARYOS_USER_PROGRAM_STACK_SIZE

#define USER_DATA_SEGMENT 0x23
#define USER_CODE_SEGMENT 0x1b

#define GARYOS_MAX_PROGRAM_ALLOCATIONS 1024
#define GARYOS_MAX_PROCESSES 12

#define GARYOS_MAX_ISR80H_COMMANDS 1024

#define GARYOS_KEYBOARD_BUFFER_SIZE 1024

#endif