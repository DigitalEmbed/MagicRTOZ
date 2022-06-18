#pragma once

#ifdef __cplusplus
    extern "C" {
#endif

#include <stdlib.h>
#include <stdint.h>
#include "./process.h"
#include "./slist.h"

#define STIMER_NEW(name, callback, arguments) \
{\
    PROCESS_BUILD(name),\
    ._arguments = (const void*) (arguments),\
    ._callback = (const void (*)(void*)) (callback),\
    ._time_waiting_ms = 0,\
    ._time_counter_ms = 0,\
    ._run_mode = STIMER_RUN_MODE_ONCE,\
}

#define STIMER_INIT(callback, argument, argument_type) \
void callback(void* _self)\
{\
    argument_type* argument = (argument_type*) ((stimer_t*) _self)->_arguments;\
    (void)(argument);\

#define STIMER_END \
}

#define STIMER_RUN_ONCE_INIT \
{\
    static bool _run_once = false;\
    if (_run_once == false)\
    {

#define STIMER_RUN_ONCE_END \
        _run_once = true;\
    }\
}

typedef enum stimer_run_mode_t
{
    STIMER_RUN_MODE_ONCE = 0,
    STIMER_RUN_MODE_FOREVER,
    STIMER_RUN_MODE_SIZE
}
stimer_run_mode_t;

typedef struct stimer_t
{
    PROCESS_STRUCT;
    const void* _arguments;
    const void (*_callback)(void* self);
    uint32_t _time_waiting_ms;
    uint32_t _time_counter_ms;
    stimer_run_mode_t _run_mode;
}
stimer_t;

int8_t stimer_install(stimer_t* stimer, uint8_t priority, uint32_t time_waiting_ms, stimer_run_mode_t run_mode);
void stimer_suspend(stimer_t* stimer);
void stimer_resume(stimer_t* stimer);
void stimer_setPriority(stimer_t* stimer, uint8_t priority);
void stimer_setWaitingTimer(stimer_t* stimer, uint32_t time_waiting_ms);
const char* stimer_getName(stimer_t* stimer);

#ifdef __cplusplus
    }
#endif
