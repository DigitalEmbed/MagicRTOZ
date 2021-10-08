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
    PROCESS_BUILD(name),\
    ._arguments = (const void*) (arguments),\
    ._callback = (const void (*)(void*)) (callback),\
    ._line = 0,\
    ._time_waiting_ms = 0,\
}

#define TASK_INIT(callback, argument, argument_type)\
void callback(void* _self)\
{\
    argument_type* argument = (argument_type*) ((task_t*) _self)->_arguments;\
    (void)(argument);\
    switch(((task_t*) _self)->_line)\
    {\
        default:\
        {\
            return;\
        }\
        case 0:

#define TASK_END\
    }\
    ((task_t*) _self)->_time_waiting_ms = (PROCESS_MINIMUM_TIME_WAITING_MS);\
    ((task_t*) _self)->_line = 0;\
}

#define TASK_YIELD() task_delay(0)

#define TASK_DELAY(time_ms)\
    ((task_t*) _self)->_time_waiting_ms = ((time_ms) > (PROCESS_MINIMUM_TIME_WAITING_MS) ? (time_ms) : (PROCESS_MINIMUM_TIME_WAITING_MS)) ; ((task_t*) _self)->_line = __LINE__ ; return ; case __LINE__:

typedef struct task_t
{
    PROCESS_STRUCT;
    const void* _arguments;
    const void (*_callback)(void* self);
    uint16_t _line;
    uint32_t _time_waiting_ms;
}
task_t;

int8_t task_install(task_t* task, const uint8_t priority);
void task_suspend(task_t* task);
void task_resume(task_t* task);
void task_priority_set(task_t* task, uint8_t priority);
const char* task_getName(task_t* task);

#ifdef __cplusplus
    }
#endif
