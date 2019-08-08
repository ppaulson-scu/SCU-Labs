#include "wiced.h"
#define THREAD_PRIORITY (10)
#define THREAD_STACK_SIZE (10000)

uint8_t ledToBlink = WICED_SH_LED0;
wiced_bool_t led = WICED_FALSE;

void connectThread(wiced_thread_arg_t arg){
    wiced_result_t connectResult;
    wiced_bool_t connected = WICED_FALSE;

    while(connected == WICED_FALSE){
        wiced_network_down(WICED_STA_INTERFACE);
        connectResult = wiced_network_up(WICED_STA_INTERFACE, WICED_USE_EXTERNAL_DHCP_SERVER, NULL);

        if(connectResult == WICED_SUCCESS){
            connected = WICED_TRUE;
            ledToBlink = WICED_SH_LED1;
        }
        else {
            wiced_rtos_delay_milliseconds(5000);
        }
    }
}

void application_start(){
    wiced_thread_t connectThreadHandle;
    wiced_init();

    wiced_rtos_create_thread(&connectThreadHandle, THREAD_PRIORITY, "connectionThread", connectThread, THREAD_STACK_SIZE, NULL);
    while(1){
        if(led == WICED_TRUE){
            wiced_gpio_output_low( ledToBlink);
            led = WICED_FALSE;
        }
        else {
            wiced_gpio_output_high(ledToBlink);
            led = WICED_TRUE;
        }
        wiced_rtos_delay_milliseconds(250);
    }
}
