#include "stdio.h"
#include "garyos.h"

// https://www.tutorialspoint.com/c_standard_library/stdio_h.htm

int putchar(int c){

    garyos_putchar((char)c);
    return 0;
}