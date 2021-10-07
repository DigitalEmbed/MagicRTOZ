#include "./process.h"
#include "./task.h"
#include "./config.h"
#include <stdbool.h>

#if defined(__linux__)
    #include <unistd.h>
#endif

__attribute__((constructor))
void system_init(void)
{
    printf("Initializing system...\n");
}

__attribute__((destructor))
void system_run(void)
{
    printf("System initialized!\n\n");
    for(;;)
    {
        _process_schedule(); //put it on a timer interrupt callback
        //execute all events
        _process_run();
        //sleep_until_interrupt
        #if defined(__linux__)
            usleep(1000 * PROCESS_MINIMUM_TIME_WAITING_MS); //configure your timer to overflow at THREAD_MINIMUM_TIME_WAITING ms.
        #endif
    }
}
