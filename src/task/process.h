#ifndef PROCESS_H
#define PROCESS_H
#include <stdint.h>
#include "config.h"
#include "task.h"

struct process{

    uint16_t id;

    char filename[GARYOS_MAX_PATH];

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

int process_load_for_slot(const char* filename, struct process** process, int process_slot);
int process_load(const char* filename, struct process** process);

#endif