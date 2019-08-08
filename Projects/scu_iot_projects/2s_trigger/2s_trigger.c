#include "wiced.h"
//#include <stdlib.h>
/* DEFINES */
#define THREAD_PRIORITY     (10)
#define THREAD_STACK_SIZE   (1024)
#define TIMER_TIME          (2000)

#define I2C_ADDRESS     (0x42)
#define RETRIES         (1)
#define DISABLE_DMA     (WICED_TRUE)
#define NUM_MESSAGES    (1)
#define TEMPERATURE_REG 0x07

/* STATIC MULTITHREADING VARIABLES */
static wiced_thread_t       ledThreadHandle;
static wiced_thread_t       timerThreadHandle;
static wiced_semaphore_t    semaphoreHandle1;
static wiced_semaphore_t    semaphoreHandle2;
static wiced_timer_t        timerHandle;

/* STATIC I2C VARIABLES */
static wiced_i2c_message_t msg;
static const wiced_i2c_device_t i2cDevice = {
       .port = WICED_I2C_2,
       .address = I2C_ADDRESS,
       .address_width = I2C_ADDRESS_WIDTH_7BIT,
       .speed_mode = I2C_STANDARD_SPEED_MODE
};

//static char testchar = 'X';

/* STATIC UART VARIABLES */
static struct {
    float temp;
    float humidity;
    float light;
    float pot;
} rx_buffer;

int timer_running = 0;

static wiced_uart_config_t uart_config =
    {
            .baud_rate = 115200,
            .data_width = DATA_WIDTH_8BIT,
            .parity = NO_PARITY,
            .stop_bits = STOP_BITS_1,
            .flow_control = FLOW_CONTROL_DISABLED,
    };

/* FUNCTIONS */
/* Interrupt service routine for the button */
void button_isr(void* arg) {
    if(timer_running) return;
    wiced_rtos_set_semaphore(&semaphoreHandle1);
    wiced_rtos_set_semaphore(&semaphoreHandle2);
    wiced_rtos_start_timer(&timerHandle);
    timer_running = 1;
}

/* Not actually an isr, but behaves a LOT like one */
void timer_isr(void* arg) {
    wiced_rtos_stop_timer(&timerHandle);
    timer_running = 0;
}

void timerThread(wiced_thread_arg_t arg){
    while(1){
        wiced_rtos_get_semaphore(&semaphoreHandle2, WICED_WAIT_FOREVER);
        while(timer_running){
            wiced_i2c_transfer(&i2cDevice, &msg, NUM_MESSAGES);
            WPRINT_APP_INFO(("Temperature: %f\n", rx_buffer.temp));
            wiced_rtos_delay_milliseconds(100);
        }
    }
}

void ledThread(wiced_thread_arg_t arg) // The thread function for LED toggle
{
    static wiced_bool_t led1 = WICED_FALSE;
    while(1)
    {
        wiced_rtos_get_semaphore(&semaphoreHandle1, WICED_WAIT_FOREVER);
        //WPRINT_APP_INFO(("Count: %lu\n", semaphoreHandle1.tx_semaphore_count));
        while(timer_running){
            if ( led1 == WICED_TRUE ) {
                wiced_gpio_output_low( WICED_SH_LED1 );
                led1 = WICED_FALSE;
            }
            else {
                wiced_gpio_output_high( WICED_SH_LED1 );
                led1 = WICED_TRUE;
            }
            wiced_rtos_delay_milliseconds(100);
        }
    }
}

void application_start( )
{
    wiced_init();

    wiced_rtos_init_semaphore(&semaphoreHandle1);
    wiced_rtos_init_semaphore(&semaphoreHandle2);

    wiced_rtos_init_timer(&timerHandle, TIMER_TIME, timer_isr, NULL);
    wiced_rtos_create_thread(&ledThreadHandle, THREAD_PRIORITY,"ledThread", ledThread, THREAD_STACK_SIZE, NULL);
    wiced_rtos_create_thread(&timerThreadHandle, THREAD_PRIORITY,"timerThread", timerThread, THREAD_STACK_SIZE, NULL);

    wiced_gpio_input_irq_enable(WICED_SH_MB1, IRQ_TRIGGER_FALLING_EDGE, button_isr, NULL);

    wiced_i2c_init(&i2cDevice);

    uint8_t tx_buffer[] = {TEMPERATURE_REG};
    wiced_i2c_message_t setOffset;
    wiced_i2c_init_tx_message(&setOffset, tx_buffer, sizeof(tx_buffer), RETRIES, DISABLE_DMA);

    wiced_i2c_transfer(&i2cDevice, &setOffset, NUM_MESSAGES);

    wiced_i2c_init_rx_message(&msg, &rx_buffer, sizeof(rx_buffer), RETRIES, DISABLE_DMA);

    wiced_uart_init(WICED_UART_1, &uart_config, NULL);

    //while(1){ wiced_rtos_delay_milliseconds(1); }
}
