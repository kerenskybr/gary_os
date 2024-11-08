#ifndef GARYOS_H
#define GARYOS_H
#include <stddef.h>

void print(const char* message);
int getkey();

void* garyos_malloc(size_t size);
void garyos_free(void* ptr);

#endif