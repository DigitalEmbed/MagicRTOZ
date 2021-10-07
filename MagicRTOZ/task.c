#include "./task.h"
#include "./process.h"

#include <string.h>

void _task_schedule(process_t* process);
void _task_run(process_t* process);

static task_t* _running_task = NULL;

void task_install(task_t* task, uint8_t priority)
{
    static uint8_t process_id = 0;
    PROCESS_SLICE_CREATE(&_task_schedule, &_task_run, process_id);
    if (process_id != 0 && task->_selement._slist == NULL)
    {
        task->_process._id = process_id;
        task->_time_waiting_ms = (PROCESS_MINIMUM_TIME_WAITING_MS);
        task->_process._data = (void*) task;
        task->_process._status = PROCESS_STATUS_WAIT;
        task->_process._selement = &task->_selement;
        task->_selement._data = (const void*) &task->_process;
        _process_install(&task->_process, priority);
    }
}

void task_suspend(task_t* task)
{
    if (task != NULL && task->_process._data != NULL)
    {
        _process_suspend(&task->_process);
    }
    else
    {
        _process_suspend(NULL);
    }
}

void task_resume(task_t* task)
{
    if (task != NULL && task->_process._data != NULL)
    {
        _process_resume(&task->_process);
    }
    else
    {
        _process_resume(NULL);
    }
}

void task_priority_set(task_t* task, uint8_t priority)
{
    if (task != NULL)
    {
        _process_priority_set(&task->_process, priority);
    }
    else
    {
        _process_priority_set(NULL, priority);
    }
}

const char* task_getName(task_t* task)
{
    if (task != NULL)
    {
        return _process_getName(&task->_process);
    }
    return _process_getName(NULL);
}

void _task_schedule(process_t* process)
{
    if (process != NULL)
    {
        task_t* task_buffer = (task_t*) process->_data;
        if (task_buffer->_time_waiting_ms == 0)
        {
            process->_status = PROCESS_STATUS_RUN;
        }
        else
        {
            task_buffer->_time_waiting_ms = task_buffer->_time_waiting_ms > (PROCESS_MINIMUM_TIME_WAITING_MS) ? task_buffer->_time_waiting_ms - (PROCESS_MINIMUM_TIME_WAITING_MS) : 0;
        }
    }
}

void _task_run(process_t* process)
{
    if (process != NULL)
    {
        _running_task = (task_t*) process->_data;
        _running_task->_callback(_running_task);
        if (process->_status != PROCESS_STATUS_STOP)
        {
            process->_status = PROCESS_STATUS_WAIT;
        }
        _running_task = NULL;
    }
}
