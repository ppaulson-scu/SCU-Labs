#include "wiced.h"
volatile wiced_bool_t newPress = WICED_FALSE;


void button_isr(void* arg) // Interrupt service routine for the button
{
    static wiced_bool_t led1 = WICED_FALSE;
    /* Toggle LED1 */
    if ( led1 == WICED_TRUE ) {
        wiced_gpio_output_low( WICED_LED1 );
        led1 = WICED_FALSE;
    }
    else {
        wiced_gpio_output_high( WICED_LED1 );
        led1 = WICED_TRUE;
    }
    newPress = WICED_TRUE; /* Need to send new button count value */
}
/* Main application */
void application_start( )
{
    uint8_t pressCount = 0;
    char printChar;
    wiced_init(); /* Initialize the WICED device */
    wiced_gpio_input_irq_enable(WICED_SH_MB1, IRQ_TRIGGER_FALLING_EDGE, button_isr, NULL); /* Setup interrupt */
    wiced_uart_config_t uart_config =

    {
            .baud_rate
            = 115200,
            .data_width
            = DATA_WIDTH_8BIT,
            .parity
            = NO_PARITY,
            .stop_bits
            = STOP_BITS_1,
            .flow_control = FLOW_CONTROL_DISABLED,
    };

    wiced_uart_init(WICED_UART_1, &uart_config, NULL);
    while ( 1 ) {
        if(newPress) {
            pressCount ++; /* Increment counter */
            if(pressCount > 9) pressCount = 0;
            printChar = pressCount + '0';

            wiced_uart_transmit_bytes(WICED_UART_1, &printChar , 1);
            newPress = WICED_FALSE;
        }
    }
}
