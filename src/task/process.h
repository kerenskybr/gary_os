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

    struct keyboard_buffer{

        char buffer[GARYOS_KEYBOARD_BUFFER_SIZE];
        int tail;
        int head;
    } keyboard;
};

int process_switch(struct process* process);
int process_load_switch(const char* filename, struct process** process);
int process_load_for_slot(const char* filename, struct process** process, int process_slot);
int process_load(const char* filename, struct process** process);
struct process* process_current();
struct process* process_get(int process_id);

#endif