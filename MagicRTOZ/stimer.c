#include "./stimer.h"
#include "./config.h"

#include <string.h>

void _stimer_schedule(process_t* process);
void _stimer_run(process_t* process);

stimer_t* _running_stimer = NULL;

int8_t stimer_install(stimer_t* stimer, uint8_t priority, uint32_t time_waiting_ms, stimer_run_mode_t run_mode)
{
    if (run_mode < STIMER_RUN_MODE_SIZE)
    {
        static process_slice_t process_slice = PROCESS_SLICE_NEW(&_stimer_schedule, &_stimer_run);
        static selement_t slice_selement = SLIST_ELEMENT_NEW(&process_slice, 0);
        _process_init(&slice_selement);
        stimer->_time_waiting_ms = time_waiting_ms < (PROCESS_MINIMUM_TIME_WAITING_MS) ? (PROCESS_MINIMUM_TIME_WAITING_MS) : time_waiting_ms;
        stimer->_time_counter_ms = stimer->_time_waiting_ms;
        stimer->_run_mode = run_mode;
        return _process_install(
            &slice_selement,
            &stimer->_process,
            &stimer->_selement,
            stimer,
            priority
        );
    }
    return -1;
}

void stimer_suspend(stimer_t* stimer)
{
    if (stimer != NULL && stimer->_process._data != NULL)
    {
        _process_suspend(&stimer->_process);
    }
    else
    {
        _process_suspend(NULL);
    }
}

void stimer_resume(stimer_t* stimer)
{
    if (stimer != NULL && stimer->_process._data != NULL)
    {
        _process_resume(&stimer->_process);
    }
    else
    {
        _process_resume(NULL);
    }
}

void stimer_priority_set(stimer_t* stimer, uint8_t priority)
{
    if (stimer != NULL)
    {
        _process_priority_set(&stimer->_process, priority);
    }
    else
    {
        _process_priority_set(NULL, priority);
    }
}

void stimer_waitingTimer_set(stimer_t* stimer, uint32_t time_waiting_ms)
{
    if (stimer == NULL)
    {
        _running_stimer->_time_waiting_ms = time_waiting_ms;
        _running_stimer->_time_counter_ms = _running_stimer->_time_counter_ms > time_waiting_ms ? time_waiting_ms : _running_stimer->_time_counter_ms;
    }
    else
    {
        stimer->_time_waiting_ms = time_waiting_ms;
        stimer->_time_counter_ms = stimer->_time_counter_ms > time_waiting_ms ? time_waiting_ms : stimer->_time_counter_ms;
    }
}

const char* stimer_getName(stimer_t* stimer)
{
    if (stimer != NULL)
    {
        return _process_getName(&stimer->_process);
    }
    return _process_getName(NULL);
}

void _stimer_schedule(process_t* process)
{
    if (process != NULL)
    {
        stimer_t* stimer_buffer = (stimer_t*) process->_data;
        if (stimer_buffer->_time_counter_ms == 0)
        {
            process->_status = PROCESS_STATUS_RUN;
        }
        else
        {
            stimer_buffer->_time_counter_ms = stimer_buffer->_time_counter_ms > (PROCESS_MINIMUM_TIME_WAITING_MS) ? stimer_buffer->_time_counter_ms - (PROCESS_MINIMUM_TIME_WAITING_MS) : 0;
        }
    }
}

void _stimer_run(process_t* process)
{
    if (process != NULL)
    {
        _running_stimer = (stimer_t*) process->_data;
        _running_stimer->_callback(_running_stimer);
        if (process->_status != PROCESS_STATUS_STOP)
        {
            _running_stimer->_time_counter_ms = _running_stimer->_time_waiting_ms;
            if (_running_stimer->_run_mode == STIMER_RUN_MODE_FOREVER)
            {
                process->_status = PROCESS_STATUS_WAIT;
            }
            else
            {
                process->_status = PROCESS_STATUS_STOP;
            }
        }
        _running_stimer = NULL;
    }
}
