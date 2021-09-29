#include "./task.h"
#include "./process.h"

#include <string.h>

void $task_schedule(process_t* process);
void $task_run(process_t* process);

static task_t* $running_task = NULL;

void task_install(task_t* task, uint8_t priority)
{
    static uint8_t process_id = 0;
    PROCESS_SLICE_CREATE(&$task_schedule, &$task_run, process_id);
    if (process_id != 0 && task->$selement.$slist == NULL)
    {
        task->$process.$id = process_id;
        task->$time_waiting_ms = (PROCESS_MINIMUM_TIME_WAITING_MS);
        task->$process.$data = (void*) task;
        task->$process.$status = PROCESS_STATUS_WAIT;
        task->$process.$selement = &task->$selement;
        task->$selement.$data = (const void*) &task->$process;
        $process_install(&task->$process, priority);
    }
}

void task_suspend(task_t* task)
{
    if (task != NULL && task->$process.$data != NULL)
    {
        $process_suspend(&task->$process);
    }
    else
    {
        $process_suspend(NULL);
    }
}

void task_resume(task_t* task)
{
    if (task != NULL && task->$process.$data != NULL)
    {
        $process_resume(&task->$process);
    }
    else
    {
        $process_resume(NULL);
    }
}

void task_priority_set(task_t* task, uint8_t priority)
{
    if (task != NULL)
    {
        $process_priority_set(&task->$process, priority);
    }
    else
    {
        $process_priority_set(NULL, priority);
    }
}

const char* task_getName(task_t* task)
{
    if (task != NULL)
    {
        return $process_getName(&task->$process);
    }
    return $process_getName(NULL);
}

void $task_schedule(process_t* process)
{
    if (process != NULL)
    {
        task_t* task_buffer = (task_t*) process->$data;
        if (task_buffer->$time_waiting_ms == 0)
        {
            process->$status = PROCESS_STATUS_RUN;
        }
        else
        {
            task_buffer->$time_waiting_ms = task_buffer->$time_waiting_ms > (PROCESS_MINIMUM_TIME_WAITING_MS) ? task_buffer->$time_waiting_ms - (PROCESS_MINIMUM_TIME_WAITING_MS) : 0;
        }
    }
}

void $task_run(process_t* process)
{
    if (process != NULL)
    {
        $running_task = (task_t*) process->$data;
        $running_task->$callback($running_task);
        if (process->$status != PROCESS_STATUS_STOP)
        {
            process->$status = PROCESS_STATUS_WAIT;
        }
        $running_task = NULL;
    }
}
