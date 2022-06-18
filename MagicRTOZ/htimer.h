#pragma once

#ifdef __cplusplus
    extern "C" {
#endif

#include <stdlib.h>
#include <stdint.h>
#include "./process.h"
#include "./slist.h"

typedef enum htimer_run_mode_t
{
    HTIMER_RUN_MODE_ONCE = 0,
    HTIMER_RUN_MODE_FOREVER,
    HTIMER_RUN_MODE_SIZE
}
htimer_run_mode_t;

struct _htimer_t;

typedef struct htimer_t
{
    PROCESS_STRUCT;
    const void* _arguments;
    const void (*_callback)(void* self);
    uint32_t _time_waiting_ms;
    uint32_t _time_counter_ms;
    htimer_run_mode_t _run_mode;
    struct _htimer_t* _deprived;
}
htimer_t;

int8_t htimer_install(htimer_t* htimer, uint8_t priority, uint32_t time_waiting_ms, htimer_run_mode_t run_mode);
void htimer_suspend(htimer_t* htimer);
void htimer_resume(htimer_t* htimer);
void htimer_setPriority(htimer_t* htimer, uint8_t priority);
void htimer_setWaitingTimer(htimer_t* htimer, uint32_t time_waiting_ms);
const char* htimer_getName(htimer_t* htimer);

#ifdef __cplusplus
    }
#endif