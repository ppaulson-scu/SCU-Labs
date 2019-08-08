#include "wiced.h"

#define USE_MUTEX

#define THREAD_PRIORITY     (10)
#define THREAD_STACK_SIZE   (1024)

static wiced_thread_t ledThread1Handle;
static wiced_thread_t ledThread2Handle;

#ifdef USE_MUTEX
static wiced_mutex_t MutexHandle;
#endif

void ledThread1(wiced_thread_arg_t arg){
    while(1){

        #ifdef USE_MUTEX
            wiced_rtos_lock_mutex(&MutexHandle);
        #endif

        while(!wiced_gpio_input_get( WICED_SH_MB0 )) {
            wiced_gpio_output_high( WICED_SH_LED0 );
            wiced_rtos_delay_milliseconds(100);
            wiced_gpio_output_low( WICED_SH_LED0 );
            wiced_rtos_delay_milliseconds(100);
        }

        #ifdef USE_MUTEX
            wiced_rtos_unlock_mutex(&MutexHandle);
        #endif

            wiced_rtos_delay_milliseconds(1);
    }
}

void ledThread2(wiced_thread_arg_t arg){
    while(1){

        #ifdef USE_MUTEX
            wiced_rtos_lock_mutex(&MutexHandle);
        #endif

        while(!wiced_gpio_input_get( WICED_SH_MB1 )) {
            wiced_gpio_output_high( WICED_SH_LED0 );
            wiced_rtos_delay_milliseconds(150);
            wiced_gpio_output_low( WICED_SH_LED0 );
            wiced_rtos_delay_milliseconds(150);
        }

        #ifdef USE_MUTEX
            wiced_rtos_unlock_mutex(&MutexHandle);
        #endif

            wiced_rtos_delay_milliseconds(1);
    }
}

void application_start(){
    wiced_init();

    #ifdef USE_MUTEX
    wiced_rtos_init_mutex(&MutexHandle);
    #endif

    WPRINT_APP_INFO (("Threads Created!\n"));

    wiced_rtos_create_thread(&ledThread1Handle, THREAD_PRIORITY, "ledThread1", ledThread1, THREAD_STACK_SIZE, NULL);
    wiced_rtos_create_thread(&ledThread2Handle, THREAD_PRIORITY, "ledThread2", ledThread2, THREAD_STACK_SIZE, NULL);

}
