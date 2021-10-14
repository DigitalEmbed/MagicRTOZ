/*! Process scheduler API
 *  \brief      Cooperative process scheduler with priority execution API.
 *  \details    This API is a cooperative process scheduler. It's not usable at once: you must
 *              create user interfaces and import this lib on it. The process will be inserted on a
 *              linked list and will be executed according to its priority - defined by user at
 *              creating a process. To do this, follow the examples below.
 *  \example    Using process API:
 *  \code{.c}

    // First of all, you must create a struct containing PROCESS_STRUCT
    // macro and others variables.
    typedef struct myProcess_t
    {
        PROCESS_STRUCT;
        const void* _arguments;
        const void (*_callback)(void* arguments);
    } myProcess_t;

    // Creation of myProcess "constructor".
    #define MYPROCESS_NEW(name, callback, arguments) \
    {\
        PROCESS_BUILD(name),\
        ._arguments = (const void*) (arguments),\
        ._callback = (const void (*)(void*)) (callback)\
    }

    // Running myProcess process pointer.
    static myProcess_t* _running_myProcess = NULL;

    // Function that will be schedule myProcess process.
    void _myProcess_schedule(process_t* process)
    {
        if (process != NULL)
        {
            myProcess_t* myProcess_buffer = (myProcess_t*) process->_data;

            // Queuing the process.
            process->_status = PROCESS_STATUS_RUN;
        }
    }

    // Function that will be run myProcess process callback.
    void _myProcess_run(process_t* process)
    {
        if (process != NULL)
        {
            _running_myProcess = (myProcess_t*) process->_data;

            // Executing process callback.
            _running_myProcess->_callback
            (
                (void*) _running_myProcess->_arguments
            );
            _running_myProcess = NULL;

            // Unqueuing the process.
            process->_status = PROCESS_STATUS_STOP;
        }
    }

    // Function that will be install myProcess processes.
    int8_t myProcess_install(myProcess_t* myProcess, uint8_t priority)
    {
        // Creating a process slice object.
        static process_slice_t process_slice = PROCESS_SLICE_NEW
        (
            &_myProcess_schedule,
            &_myProcess_run
        );

        // Encapsulating the process slice into a static element.
        static selement_t slice_selement = SLIST_ELEMENT_NEW
        (
            &process_slice,
            0
        );

        // Installing process slice.
        return process_install
        (
            &slice_selement,
            &myProcess->_process,
            &myProcess->_selement,
            myProcess,
            priority
        );
    }

    // Function that will be to queue myProcess process.
    void myProcess_start(myProcess_t* myProcess)
    {
        if (myProcess != NULL && myProcess->_process._data != NULL)
        {
            process_resume(&myProcess->_process);
        }
        else
        {
            process_resume(NULL);
        }
    }

    // Testing function callback
    void myProcess_callback(void* arguments)
    {
        int* number = (int*) arguments;
        printf("Number: %d\n", *number);
    }

    // Testing main
    int main()
    {
        static int number = 55;
        static myProcess_t myProcess = MYPROCESS_NEW
        (
            "Test",
            myProcess_callback,
            &number
        );
        myProcess_install(&myProcess, 1);
        myProcess_start(&myProcess);
    }

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

#define PROCESS_SLICE_NEW(schedule_callback, run_callback)\
{\
    ._schedule = (schedule_callback),\
    ._run = (run_callback),\
    ._id = 0,\
}

#define PROCESS_STRUCT \
selement_t _selement;\
process_t _process

#define PROCESS_BUILD(name)\
._selement = SLIST_ELEMENT_NEW(NULL, 0),\
._process = {\
    ._name = (name),\
    ._id = 0,\
    ._data = NULL,\
    ._last_runtime_ms = 0,\
    ._average_runtime_ms = 0,\
}

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

void process_schedule(void);
void process_run(void);
int8_t process_install(selement_t* slice_element, process_t* object_process, selement_t* object_element, void* object, uint8_t priority);
void process_resume(process_t* process);
void process_suspend(process_t* process);
void process_priority_set(process_t* process, uint8_t priority);
const char* process_getName(process_t* process);

#ifdef __cplusplus
    }
#endif
