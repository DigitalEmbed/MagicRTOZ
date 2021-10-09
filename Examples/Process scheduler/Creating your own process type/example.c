/*! Creating your own process type
 *  \brief      Using process API to create custom processes.
 *  \author     Jorge Henrique Moreira Santana
 *  \date       09/10/2021
 *  \warning    Be very careful when using Macros.
 *              Variables and functions starting with the '_' character are private and must not be modified.
 *              All data variables must be global or static.
 *  \copyright  MIT License.
 */

#include <MagicRTOZ.h>

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
    *number++;
    printf("Number: %d\n", *number);
}

// Testing main
__attribute__((weak))
int main()
{
    static int number = 0;
    static myProcess_t myProcess = MYPROCESS_NEW
    (
        "Test",
        myProcess_callback,
        &number
    );
    myProcess_install(&myProcess, 1);
    myProcess_start(&myProcess);
}
