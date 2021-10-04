#include "./process.h"
#include "./task.h"
#include "./config.h"

#include <string.h>

static slist_t $waiting_process_slist = SLIST_NEW();
static slist_t $running_process_slist = SLIST_NEW();
static slist_t $stopped_process_slist = SLIST_NEW();
static slist_t $slice_process_slist = SLIST_NEW();

static process_t* $running_process = NULL;
static uint16_t $running_process_runtime_ms = 0;

uint8_t $process_init(selement_t* slice)
{
    if (slice != NULL && slice->$data != NULL)
    {
        slist_selement_insert(&$slice_process_slist, slice);
        ((process_slice_t*) slice->$data)->$id = slist_size_get(&$slice_process_slist);
        return ((process_slice_t*) slice->$data)->$id;
    }
    return 0;
}

void $process_schedule(void)
{
    static process_slice_t* current_slice = NULL;
    if ($running_process != NULL)
    {
        if ($running_process_runtime_ms <= SOFT_WATCHDOG_TIME_OVERFLOW_MS)
        {
            $running_process_runtime_ms++;
        }
        else
        {
            //reset_uC()
        }
    }
    SLIST_FOREACH_INIT($waiting_process_slist, process_buffer, process_t)
    {
        if (current_slice == NULL || current_slice->$id != process_buffer->$id)
        {
            current_slice = (process_slice_t*) slist_position_dataGet(&$slice_process_slist, process_buffer->$id - 1);
        }
        if (current_slice != NULL && current_slice->$schedule != NULL)
        {
            switch(process_buffer->$status)
            {
                case PROCESS_STATUS_RUN: slist_selement_move(process_buffer->$selement, &$running_process_slist); break;
                case PROCESS_STATUS_STOP: slist_selement_move(process_buffer->$selement, &$stopped_process_slist); break;
                case PROCESS_STATUS_WAIT: current_slice->$schedule(process_buffer); break;
            }
        }
    }
    SLIST_FOREACH_END
}

void $process_run(void)
{
    static process_slice_t* current_slice = NULL;
    while(slist_size_get(&$running_process_slist) != 0)
    {
        $running_process = (process_t*) slist_position_dataGet(&$running_process_slist, 0);
        if (current_slice == NULL || current_slice->$id != $running_process->$id)
        {
            current_slice = (process_slice_t*) slist_position_dataGet(&$slice_process_slist, $running_process->$id - 1);
        }
        if (current_slice != NULL && current_slice->$run != NULL)
        {
            $running_process_runtime_ms = 0;
            current_slice->$run($running_process);
            $running_process->$last_runtime_ms = $running_process_runtime_ms;
            $running_process->$average_runtime_ms = ($running_process->$average_runtime_ms == 0) ? $running_process->$last_runtime_ms : (($running_process->$last_runtime_ms + $running_process->$average_runtime_ms) >> 1);
            printf("avg runtime: %d ms\nlst runtime: %d ms\n\n", $running_process->$average_runtime_ms, $running_process->$last_runtime_ms);
            switch($running_process->$status){
                case PROCESS_STATUS_RUN: break;
                case PROCESS_STATUS_STOP: slist_selement_move($running_process->$selement, &$stopped_process_slist); break;
                case PROCESS_STATUS_WAIT: slist_selement_move($running_process->$selement, &$waiting_process_slist); break;
            }
            $running_process = NULL;
        }
    }
}

void $process_install(process_t* process, uint8_t priority)
{
    if(process != NULL && process->$selement != NULL)
    {
        process->$selement->$priority = priority;
        slist_selement_insert(&$waiting_process_slist, process->$selement);
    }
}

void $process_resume(process_t* process)
{
    if (process == NULL)
    {
        process = $running_process;
    }
    if (process != NULL && process->$status == PROCESS_STATUS_STOP && process->$data != NULL)
    {
        process->$status = PROCESS_STATUS_WAIT;
        slist_selement_move(process->$selement, &$waiting_process_slist);
    }
}

void $process_priority_set(process_t* process, uint8_t priority)
{
    if (process == NULL)
    {
        process = $running_process;
    }
    if (process != NULL && process->$selement->$priority != priority)
    {
        slist_t* processList = process->$selement->$slist;
        slist_selement_delete(process->$selement);
        process->$selement->$priority = priority;
        slist_selement_insert(processList, process->$selement);
    }
}

void $process_suspend(process_t* process)
{
    if (process == NULL)
    {
        process = $running_process;
    }
    if (process != NULL && process->$status != PROCESS_STATUS_STOP)
    {
        process->$status = PROCESS_STATUS_STOP;
    }
}

const char* $process_getName(process_t* process)
{
    if(process == NULL)
    {
        return $running_process->$name;
    }
    return process->$name;
}
