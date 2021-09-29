#include "./process.h"
#include "./task.h"
#include "./config.h"
#include <stdbool.h>

#if defined(__linux__)
    #include <unistd.h>
#endif

static bool $app_weak = false;

__attribute__((weak)) void boot(void)
{
    $app_weak = true;
    return;
}

int main()
{
    //$process_init();
    printf("Initializing system...\n");
    boot();
    printf("System initialized!\n\n");
    while($app_weak == false)
    {
        $process_schedule(); //put it on a timer interrupt callback
        //execute all events
        $process_run();
        //sleep_until_interrupt
        #if defined(__linux__)
            usleep(1000 * PROCESS_MINIMUM_TIME_WAITING_MS); //configure your timer to overflow at THREAD_MINIMUM_TIME_WAITING ms.
        #endif
    }
    return 0;
}
