#ifndef PROCESS_H
#define PROCESS_H
#include <stdint.h>
#include "config.h"
#include "task.h"

struct process{

    uint16_t id;

    char filename[GARYOS_MAXPATH];

    struct task* task;

    // THe malloc allocations of the process
    void* allocations[GARYOS_MAX_PROGRAM_ALLOCATIONS];

    // Physical pointer to the proccess memory
    void* ptr;

    //Phisical pointer to the stack mem
    void* stack;

    // Size of the data pointer
    uint32_t size;
};

#endif