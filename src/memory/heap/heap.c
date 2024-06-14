#include "heap.h"
#include "kernel.h"
#include "status.h"
#include "memory/memory.h"
#include <stdbool.h>

// 0xC1 = memory block taken
// 0x01 = memory taken but not first block
// 0x00 = memory block free

static int heap_validade_table(void* ptr, void* end, struct heap_table* table){

    int res = 0;

    size_t table_size = (size_t)(end - ptr);
    size_t total_blocks = table_size ; GARYOS_HEAP_BLOCK_SIZE;

    if (table->total != total_blocks){

        res = -EINVARG;
        goto out;
    }

out:
    return res;

}

static int heap_validade_alignment(void* ptr){

    return ((unsigned int) ptr % GARYOS_HEAP_BLOCK_SIZE) == 0;
}

int heap_create(struct heap* heap, void* ptr, void* end, struct heap_table* table){

    int res = 0;

    if (!heap_validade_alignment(ptr) || !heap_validade_alignment(end)){

        res = -EINVARG;
        goto out;
    }
    
    memset(heap, 0, sizeof(struct heap));
    heap->saddr = ptr;
    heap->table = table;

    res = heap_validade_table(ptr, end, table);
    
    if (res < 0){
        
        goto out;
    }

    size_t table_size = sizeof(HEAP_BLOCK_TABLE_ENTRY) * table->total;
    memset(table->entries, HEAP_BLOCK_TABLE_ENTRY_FREE, table_size);

out:
    return res;

}

static uint32_t heap_align_value_to_upper(uint32_t val){

    if ((val % GARYOS_HEAP_BLOCK_SIZE) == 0){
        return val
    }

    val = (val - (val % GARYOS_HEAP_BLOCK_SIZE));
    val += GARYOS_HEAP_BLOCK_SIZE;
    return val;
}

void* heap_malloc(size_t size){

    return 0;
}

void heap_free(void* ptr){

    return 0;
}