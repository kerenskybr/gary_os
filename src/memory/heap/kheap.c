#include "kheap.h"
#include "heap.h"
#include "config.h"
#include "kernel.h"
struct heap kernel_heap;

struct heap_table kernel_heap_table;

// https://wiki.osdev.org/Memory_Map_(x86)

void kheap_init(){

    int total_table_entries = GARYOS_HEAP_SIZE_BYTES /GARYOS_HEAP_BLOCK_SIZE; // 25600
    kernel_heap_table.entries = (HEAP_BLOCK_TABLE_ENTRY*)(GARYOS_HEAP_TABLE_ADDRESS);
    kernel_heap_table.total = total_table_entries;

    void* end = (void*)(GARYOS_HEAP_ADDRESS + GARYOS_HEAP_SIZE_BYTES);
    int res = heap_create(&kernel_heap, (void*)(GARYOS_HEAP_ADDRESS), end, &kernel_heap_table);
    if (res < 0){
        print("\nFAILED to crate heap\n");
    }
}