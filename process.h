#pragma once

#include <stdlib.h>
#include <stdint.h>
#include "./slist.h"

#define PROCESS_NEW(name, id, data)\
{\
    .$name = name,\
    .$id = id,\
    .$data = data,\
    .$last_runtime_ms = 0,\
    .$average_runtime_ms = 0,\
}

#define PROCESS_SLICE_NEW(schedule_callback, run_callback)\
{\
    .$schedule = schedule_callback,\
    .$run = run_callback,\
    .$id = 0,\
}

#define PROCESS_SLICE_CREATE(schedule_callback, run_callback, id_variable)\
{\
    static process_slice_t process_slice = PROCESS_SLICE_NEW(schedule_callback, run_callback);\
    static selement_t slice_selement = LIST_ELEMENT_NEW(&process_slice, 0);\
    if (id_variable == 0)\
    {\
        id_variable = $process_init(&slice_selement);\
    }\
}

typedef enum
{
    PROCESS_STATUS_RUN = 0,
    PROCESS_STATUS_STOP,
    PROCESS_STATUS_WAIT,
}
process_status_t;

typedef enum
{
    PROCESS_TYPE_TASK = 0,
    PROCESS_TYPE_TIMER,
    PROCESS_TYPE_INTERRUPT,
    PROCESS_TYPE_EVENT,
    PROCESS_TYPE_EXCEPTION,
    PROCESS_TYPE_SIZE
}
process_type_t;

typedef struct process_t
{
    const char* $name;
    uint8_t $id;
    void* $data;
    process_status_t $status;
    selement_t* $selement;
    uint16_t $last_runtime_ms;
    uint16_t $average_runtime_ms;
}
process_t;

typedef struct process_slice_t
{
    void (*$schedule)(process_t* process);
    void (*$run)(process_t* process);
    uint8_t $id;
}
process_slice_t;

uint8_t $process_init(selement_t* slice);
void $process_schedule(void);
void $process_run(void);
void $process_install(process_t* process, uint8_t priority);
void $process_resume(process_t* process);
void $process_suspend(process_t* process);
void $process_priority_set(process_t* process, uint8_t priority);
const char* $process_getName(process_t* process);
