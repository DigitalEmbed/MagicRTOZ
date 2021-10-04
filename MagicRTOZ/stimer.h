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
    .$arguments = (const void*) (arguments),\
    .$callback = (const void (*)(void*)) (callback),\
    .$selement = SLIST_ELEMENT_NEW(NULL, 0),\
    .$process = PROCESS_NEW((name), PROCESS_TYPE_TIMER, NULL),\
}

#define STIMER_INIT(callback, argument, argument_type) \
void callback(void* $self)\
{\
    argument_type* argument = (argument_type*) ((stimer_t*) $self)->$arguments;\

#define STIMER_END \
    (void)(argument);\
}

#define STIMER_RUN_ONCE_INIT \
{\
    static bool $run_once = false;\
    if ($run_once == false)\
    {

#define STIMER_RUN_ONCE_END \
        $run_once = true;\
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
    const char* $name;
    const void* $arguments;
    const void (*$callback)(void* self);
    uint32_t $time_waiting_ms;
    uint32_t $time_counter_ms;
    stimer_run_mode_t $run_mode;
    selement_t $selement;
    process_t $process;
}
stimer_t;

void stimer_install(stimer_t* stimer, uint8_t priority, uint32_t time_waiting_ms, stimer_run_mode_t run_mode);
void stimer_suspend(stimer_t* stimer);
void stimer_resume(stimer_t* stimer);
void stimer_priority_set(stimer_t* stimer, uint8_t priority);
void stimer_waitingTimer_set(stimer_t* stimer, uint32_t time_waiting_ms);
const char* stimer_getName(stimer_t* stimer);

#ifdef __cplusplus
    }
#endif
