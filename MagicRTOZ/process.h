/*! Process scheduler API
 *  \brief      Cooperative process scheduler with priority execution API.
 *  \details    This API is a cooperative process scheduler. It's not usable at once: you must
 *              create user interfaces and import this lib on it. The process will be inserted on a
 *              linked list and will be executed according to its priority - defined by user at
 *              creating a process.
 *  \example    1 - Inserting and getting list data:
 *  \code{.c}

 *  \endcode
 *  \author     Jorge Henrique Moreira Santana
 *  \version    0.1a
 *  \date       02/10/2021
 *  \bug        This API needs more testing to find bugs.
 *  \warning    Be very careful when using Macros.
 *              Variables and functions starting with the '_' character are private and must not be modified.
 *  \copyright  MIT License.
 */

#pragma once

#ifdef __cplusplus
    extern "C" {
#endif

#include <stdlib.h>
#include <stdint.h>
#include "./slist.h"

#define PROCESS_NEW(name, id, data)\
{\
    ._name = name,\
    ._id = id,\
    ._data = data,\
    ._last_runtime_ms = 0,\
    ._average_runtime_ms = 0,\
}

#define PROCESS_SLICE_NEW(schedule_callback, run_callback)\
{\
    ._schedule = schedule_callback,\
    ._run = run_callback,\
    ._id = 0,\
}

#define PROCESS_SLICE_CREATE(schedule_callback, run_callback, id_variable)\
{\
    static process_slice_t process_slice = PROCESS_SLICE_NEW(schedule_callback, run_callback);\
    static selement_t slice_selement = SLIST_ELEMENT_NEW(&process_slice, 0);\
    if (id_variable == 0)\
    {\
        id_variable = _process_init(&slice_selement);\
    }\
}

#define $PROCESS\
    selement_t _selement;\
    process_t _process;\

#define $PROCESS_BUILDER(name)\
    ._selement = SLIST_ELEMENT_NEW(NULL, 0),\
    ._process = PROCESS_NEW((name), 0, NULL),\

typedef enum
{
    PROCESS_STATUS_RUN = 0,
    PROCESS_STATUS_STOP,
    PROCESS_STATUS_WAIT,
}
process_status_t;

typedef struct process_t
{
    const char* _name;
    uint8_t _id;
    void* _data;
    process_status_t _status;
    selement_t* _selement;
    uint16_t _last_runtime_ms;
    uint16_t _average_runtime_ms;
}
process_t;

typedef struct process_slice_t
{
    void (*_schedule)(process_t* process);
    void (*_run)(process_t* process);
    uint8_t _id;
}
process_slice_t;

uint8_t _process_init(selement_t* slice);
void _process_schedule(void);
void _process_run(void);
void _process_install(process_t* process, uint8_t priority);
void _process_resume(process_t* process);
void _process_suspend(process_t* process);
void _process_priority_set(process_t* process, uint8_t priority);
const char* _process_getName(process_t* process);

#ifdef __cplusplus
    }
#endif
