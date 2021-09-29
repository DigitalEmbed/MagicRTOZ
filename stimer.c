#include "./stimer.h"
#include "./process.h"
#include "./config.h"

#include <string.h>

void $stimer_schedule(process_t* process);
void $stimer_run(process_t* process);

stimer_t* $running_stimer = NULL;

void stimer_install(stimer_t* stimer, uint8_t priority, uint32_t time_waiting_ms, stimer_run_mode_t run_mode)
{
    static uint8_t process_id = 0;
    PROCESS_SLICE_CREATE(&$stimer_schedule, &$stimer_run, process_id);
    if (process_id != 0 && stimer->$selement.$slist == NULL && stimer->$run_mode < STIMER_RUN_MODE_SIZE)
    {
        stimer->$process.$id = process_id;
        stimer->$time_waiting_ms = time_waiting_ms < (PROCESS_MINIMUM_TIME_WAITING_MS) ? (PROCESS_MINIMUM_TIME_WAITING_MS) : time_waiting_ms;
        stimer->$run_mode = run_mode >= STIMER_RUN_MODE_SIZE ? STIMER_RUN_MODE_SIZE : run_mode;
        stimer->$time_counter_ms = stimer->$time_waiting_ms;
        stimer->$process.$data = (void*) stimer;
        stimer->$process.$status = PROCESS_STATUS_WAIT;
        stimer->$process.$selement = &stimer->$selement;
        stimer->$selement.$data = (const void*) &stimer->$process;
        $process_install(&stimer->$process, priority);
    }
}

void stimer_suspend(stimer_t* stimer)
{
    if (stimer != NULL && stimer->$process.$data != NULL)
    {
        $process_suspend(&stimer->$process);
    }
    else
    {
        $process_suspend(NULL);
    }
}

void stimer_resume(stimer_t* stimer)
{
    if (stimer != NULL && stimer->$process.$data != NULL)
    {
        $process_resume(&stimer->$process);
    }
    else
    {
        $process_resume(NULL);
    }
}

void stimer_priority_set(stimer_t* stimer, uint8_t priority)
{
    if (stimer != NULL)
    {
        $process_priority_set(&stimer->$process, priority);
    }
    else
    {
        $process_priority_set(NULL, priority);
    }
}

void stimer_waitingTimer_set(stimer_t* stimer, uint32_t time_waiting_ms)
{
    if (stimer == NULL)
    {
        $running_stimer->$time_waiting_ms = time_waiting_ms;
        $running_stimer->$time_counter_ms = $running_stimer->$time_counter_ms > time_waiting_ms ? time_waiting_ms : $running_stimer->$time_counter_ms;
    }
    else
    {
        stimer->$time_waiting_ms = time_waiting_ms;
        stimer->$time_counter_ms = stimer->$time_counter_ms > time_waiting_ms ? time_waiting_ms : stimer->$time_counter_ms;
    }
}

const char* stimer_getName(stimer_t* stimer)
{
    if (stimer != NULL)
    {
        return $process_getName(&stimer->$process);
    }
    return $process_getName(NULL);
}

void $stimer_schedule(process_t* process)
{
    if (process != NULL)
    {
        stimer_t* stimer_buffer = (stimer_t*) process->$data;
        if (stimer_buffer->$time_counter_ms == 0)
        {
            process->$status = PROCESS_STATUS_RUN;
        }
        else
        {
            stimer_buffer->$time_counter_ms = stimer_buffer->$time_counter_ms > (PROCESS_MINIMUM_TIME_WAITING_MS) ? stimer_buffer->$time_counter_ms - (PROCESS_MINIMUM_TIME_WAITING_MS) : 0;
        }
    }
}

void $stimer_run(process_t* process)
{
    if (process != NULL)
    {
        $running_stimer = (stimer_t*) process->$data;
        $running_stimer->$callback($running_stimer);
        if (process->$status != PROCESS_STATUS_STOP)
        {
            $running_stimer->$time_counter_ms = $running_stimer->$time_waiting_ms;
            if ($running_stimer->$run_mode == STIMER_RUN_MODE_FOREVER)
            {
                process->$status = PROCESS_STATUS_WAIT;
            }
            else
            {
                process->$status = PROCESS_STATUS_STOP;
            }
        }
        $running_stimer = NULL;
    }
}
