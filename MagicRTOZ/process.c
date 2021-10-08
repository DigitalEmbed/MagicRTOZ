#include "./process.h"
#include "./task.h"
#include "./config.h"

#include <string.h>

static slist_t _waiting_process_slist = SLIST_NEW();
static slist_t _running_process_slist = SLIST_NEW();
static slist_t _stopped_process_slist = SLIST_NEW();
static slist_t _slice_process_slist = SLIST_NEW();

static process_t* _running_process = NULL;
static uint16_t _running_process_runtime_ms = 0;

int8_t _process_init(selement_t* slice)
{
    if (slice != NULL && slice->_data != NULL)
    {
        if (((process_slice_t*) slice->_data)->_id == 0)
        {
            slist_selement_insert(&_slice_process_slist, slice);
            ((process_slice_t*) slice->_data)->_id = slist_size_get(&_slice_process_slist);
        }
        return ((process_slice_t*) slice->_data)->_id;
    }
    return -1;
}

void _process_schedule(void)
{
    static process_slice_t* current_slice = NULL;
    if (_running_process != NULL)
    {
        if (_running_process_runtime_ms <= SOFT_WATCHDOG_TIME_OVERFLOW_MS)
        {
            _running_process_runtime_ms++;
        }
        else
        {
            //reset_uC()
        }
    }
    SLIST_FOREACH_INIT(_waiting_process_slist, process_buffer, process_t)
    {
        if (current_slice == NULL || current_slice->_id != process_buffer->_id)
        {
            current_slice = (process_slice_t*) slist_position_dataGet(&_slice_process_slist, process_buffer->_id - 1);
        }
        if (current_slice != NULL && current_slice->_schedule != NULL)
        {
            switch(process_buffer->_status)
            {
                case PROCESS_STATUS_RUN: slist_selement_move(process_buffer->_selement, &_running_process_slist); break;
                case PROCESS_STATUS_STOP: slist_selement_move(process_buffer->_selement, &_stopped_process_slist); break;
                case PROCESS_STATUS_WAIT: current_slice->_schedule(process_buffer); break;
            }
        }
    }
    SLIST_FOREACH_END
}

void _process_run(void)
{
    static process_slice_t* current_slice = NULL;
    while(slist_size_get(&_running_process_slist) != 0)
    {
        _running_process = (process_t*) slist_position_dataGet(&_running_process_slist, 0);
        if (current_slice == NULL || current_slice->_id != _running_process->_id)
        {
            current_slice = (process_slice_t*) slist_position_dataGet(&_slice_process_slist, _running_process->_id - 1);
        }
        if (current_slice != NULL && current_slice->_run != NULL)
        {
            _running_process_runtime_ms = 0;
            current_slice->_run(_running_process);
            _running_process->_last_runtime_ms = _running_process_runtime_ms;
            _running_process->_average_runtime_ms = (_running_process->_average_runtime_ms == 0) ? _running_process->_last_runtime_ms : ((_running_process->_last_runtime_ms + _running_process->_average_runtime_ms) >> 1);
            printf("avg runtime: %d ms\nlst runtime: %d ms\n\n", _running_process->_average_runtime_ms, _running_process->_last_runtime_ms);
            switch(_running_process->_status){
                case PROCESS_STATUS_RUN: break;
                case PROCESS_STATUS_STOP: slist_selement_move(_running_process->_selement, &_stopped_process_slist); break;
                case PROCESS_STATUS_WAIT: slist_selement_move(_running_process->_selement, &_waiting_process_slist); break;
            }
            _running_process = NULL;
        }
    }
}

int8_t _process_install(selement_t* slice_element, process_t* object_process, selement_t* object_element, void* object, uint8_t priority)
{
    if
    (
        slice_element != NULL &&
        object_process != NULL &&
        object_element != NULL &&
        object != NULL
    )
    {
        object_process->_id = _process_init(slice_element);
        if (object_process->_id < 1)
        {
            return -1;
        }
        object_process->_status = PROCESS_STATUS_WAIT;
        object_process->_selement = object_element;
        object_process->_selement->_data = (void *) object_process;
        object_process->_data = (void *) (object);
        object_process->_selement->_priority = priority;
        slist_selement_insert(&_waiting_process_slist, object_process->_selement);
        return 1;
    }
    return -1;
}

void _process_resume(process_t* process)
{
    if (process == NULL)
    {
        process = _running_process;
    }
    if (process != NULL && process->_status == PROCESS_STATUS_STOP && process->_data != NULL)
    {
        process->_status = PROCESS_STATUS_WAIT;
        slist_selement_move(process->_selement, &_waiting_process_slist);
    }
}

void _process_priority_set(process_t* process, uint8_t priority)
{
    if (process == NULL)
    {
        process = _running_process;
    }
    if (process != NULL && process->_selement->_priority != priority)
    {
        slist_t* processList = process->_selement->_slist;
        slist_selement_delete(process->_selement);
        process->_selement->_priority = priority;
        slist_selement_insert(processList, process->_selement);
    }
}

void _process_suspend(process_t* process)
{
    if (process == NULL)
    {
        process = _running_process;
    }
    if (process != NULL && process->_status != PROCESS_STATUS_STOP)
    {
        process->_status = PROCESS_STATUS_STOP;
    }
}

const char* _process_getName(process_t* process)
{
    if(process == NULL)
    {
        return _running_process->_name;
    }
    return process->_name;
}
