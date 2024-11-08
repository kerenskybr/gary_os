#include "stdlib.h"
#include "garyos.h"

void* malloc(size_t size){

    return garyos_malloc(size);
}

void free(void* ptr){

    garyos_free(ptr);
}