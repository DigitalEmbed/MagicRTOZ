#pragma once

#ifdef __cplusplus
  extern "C" {
#endif

#include <stdlib.h>
#include <stdint.h>
#include "./process.h"
#include "./config.h"
#include "./slist.h"

#define TASK_NEW(name, callback, arguments) \
{\
    .$arguments = (const void*) (arguments),\
    .$callback = (const void (*)(void*)) (callback),\
    .$line = 0,\
    .$time_waiting_ms = 0,\
    .$selement = LIST_ELEMENT_NEW(NULL, 0),\
    .$process = PROCESS_NEW((name), 0, NULL),\
}

#define TASK_INIT(callback, argument, argument_type) \
void callback(void* $self)\
{\
    argument_type* argument = (argument_type*) ((task_t*) $self)->$arguments;\
    (void)(argument);\
    switch(((task_t*) $self)->$line)\
    {\
        default:\
        {\
            return;\
        }\
        case 0:

#define TASK_END\
    }\
    ((task_t*) $self)->$time_waiting_ms = (PROCESS_MINIMUM_TIME_WAITING_MS);\
    ((task_t*) $self)->$line = 0;\
}

#define TASK_YIELD() task_delay(0)

#define TASK_DELAY(time_ms)\
    ((task_t*) $self)->$time_waiting_ms = ((time_ms) > (PROCESS_MINIMUM_TIME_WAITING_MS) ? (time_ms) : (PROCESS_MINIMUM_TIME_WAITING_MS)) ; ((task_t*) $self)->$line = __LINE__ ; return ; case __LINE__:

typedef struct task_t
{
    const void* $arguments;
    const void (*$callback)(void* self);
    uint16_t $line;
    uint32_t $time_waiting_ms;
    selement_t $selement;
    process_t $process;
}
task_t;

void task_install(task_t* task, const uint8_t priority);
void task_suspend(task_t* task);
void task_resume(task_t* task);
void task_priority_set(task_t* task, uint8_t priority);
const char* task_getName(task_t* task);

#ifdef __cplusplus
  }
#endif
