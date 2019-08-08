#include "wiced.h"

void button_isr(void* arg){

    static wiced_bool_t led1 = WICED_FALSE;
    if(led1 == WICED_TRUE){
        wiced_gpio_output_low(WICED_LED1);
        led1=WICED_FALSE;
    } else {
        wiced_gpio_output_high(WICED_LED1);
        led1=WICED_TRUE;
    }
}

void button2_isr(void* arg){

    static wiced_bool_t led2 = WICED_FALSE;
    if(led2 == WICED_TRUE){
        wiced_gpio_output_low(WICED_LED2);
        led2=WICED_FALSE;
    } else {
        wiced_gpio_output_high(WICED_LED2);
        led2=WICED_TRUE;
    }
}

void application_start(){

    wiced_init();

    wiced_gpio_input_irq_enable(WICED_BUTTON2, IRQ_TRIGGER_FALLING_EDGE, button_isr, NULL);

    wiced_gpio_input_irq_enable(WICED_BUTTON1, IRQ_TRIGGER_FALLING_EDGE, button2_isr, NULL);

    while(1){}
}
