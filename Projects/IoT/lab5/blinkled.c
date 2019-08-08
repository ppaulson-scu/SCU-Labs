#include "wiced.h"

void application_start( ){

    wiced_init();

    while(1){
        wiced_gpio_output_low( WICED_LED1);
        wiced_gpio_output_low( WICED_LED2);
        wiced_gpio_output_low( WICED_SH_LED1);
        wiced_rtos_delay_milliseconds(250);

        wiced_gpio_output_high( WICED_LED1);
        wiced_gpio_output_high( WICED_LED2);
        wiced_gpio_output_high( WICED_SH_LED1);
        wiced_rtos_delay_milliseconds(250);
    }
}
