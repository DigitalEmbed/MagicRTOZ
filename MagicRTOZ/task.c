#include "./task.h"
#include <string.h>

void _task_schedule(process_t* process);
void _task_run(process_t* process);

int8_t task_install(task_t* task, uint8_t priority)
{
    static process_slice_t process_slice = PROCESS_SLICE_NEW(&_task_schedule, &_task_run);
    static selement_t slice_selement = SLIST_ELEMENT_NEW(&process_slice, 0);
    task->_time_waiting_ms = (PROCESS_MINIMUM_TIME_WAITING_MS);
    return _process_install(
        &slice_selement,
        &task->_process,
        &task->_selement,
        task,
        priority
    );
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
    static task_t* _running_task = NULL;
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
