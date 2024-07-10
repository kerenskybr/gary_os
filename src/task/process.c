#include "process.h"
#include "config.h"
#include "status.h"
#include "memory/memory.h"
#include "task.h"
#include "heap/kheap.h"
#include "fs/file.h"
#include "string/string.h"
#include "kernel.h"
#include "paging/paging.h"

// The current process that is running
struct process* current_process = 0;

struct process* processes[GARYOS_MAX_PROCESSES] = {};

static void process_init(struct process* process){

    memset(process, 0, sizeof(struct process));
}

struct process* process_current(){

    return current_process;
}

int process_get(int process_id){

    if (process_id < 0 || process_id >= GARYOS_MAX_PROCESSES){

        return -EINVARG;
    }

    return processes[process_id];
}

static int process_load_binary(const char* filename, struct process* process){

    int res = 0;
    int fd = fopen(filename, "r");
    if (!fd){

        res = -EIO;
        goto out;

    }
    
    struct file_stat stat;
    res = fstat(fd, &stat);
    if (res != GARYOS_ALL_OK){

        goto out;
    }

    void* program_data_ptr = kzalloc(stat.filesize);

    if (!program_stack_ptr){

        res = -ENOMEM;
        goto out;
    }

    if (fread(program_data_ptr, stat.filesize, 1, fd) != fd){

        res = -EIO;
        goto out;
    }

    process->ptr = program_data_ptr;
    process->size = stat.filesize;

out:
    fclose(fd)
    return res;
}

static int process_load_data(const char* filename, struct process* process){

    int res = 0;
    res = process_load_binary(filename, process);
    return res;
}

struct process_map_binary(struct process* process){

    int res = 0;
    paging_map_to(process->task->page_directory->directory_entry, (void*) GARYOS_PROGRAM_VIRTUAL_ADDRESS);
    return res;
}

int process_map_memory(struct process* process){

    int res = 0;
    res = process_map_binary(process);
    return res;
}

int process_load_for_slot(const char* filename, struct process** process, int process_slot){

    int res = 0;
    struct task* task = 0;
    struct process* _process;
    void* program_stack_ptr = 0;

    if (process_get(process_slot) != 0){

        res = -EISTKN;
        goto out;
    }

    _process = kzalloc(sizeof(struct process));

    if (!_process){

        res = -ENOMEM;
        goto out;
    }

    process_init(_process);
    res = process_load_data(filename, _process);

    if (res < 0){

        goto out;
    }

    program_stack_ptr = kzalloc(GARYOS_USER_PROGRAM_STACK_SIZE);
    if (!program_stack_ptr){

        res =-ENOMEM;
        goto out;
    }

    strncpy(_process->filename, filename, sizeof(_process->filename));
    _process->stack = program_stack_ptr;
    _process->id = process_slot;

    // Create a task
    task = task_new(_process);
    if (ERROR_I(task) == 0){

        res = ERROR_I(task);
    } 

    _process->task = task;

    res = process_map_memory(process);

    if (res < 0){

        goto out;
    }

    *process = _process;

    // Add the process to the array
    processes[process_slot] = _process;

out:
    if (ISERR(res)){

        if (_process && _process->task){

            task_free(_process->task;)
        }

        // Should create a func to free process data
    }

    return res;
}