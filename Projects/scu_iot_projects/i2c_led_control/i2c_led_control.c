#include "wiced.h"

#define I2C_ADDRESS     (0x42)
#define RETRIES         (1)
#define DISABLE_DMA     (WICED_TRUE)
#define NUM_MESSAGES    (1)

#define CONTROL_REG     (0x05)
#define LED_REG         (0x04)

volatile wiced_bool_t forward = WICED_TRUE;
volatile wiced_bool_t buttonPress = WICED_FALSE;

/* ISR */
void button_isr(void* arg){
    buttonPress = WICED_TRUE;
}

void application_start(){

    wiced_init();

    //Setup Shield Button 1 to call button_isr
    wiced_gpio_input_irq_enable(WICED_SH_MB1, IRQ_TRIGGER_FALLING_EDGE, button_isr, NULL);

    const wiced_i2c_device_t i2cDevice = {
            .port = WICED_I2C_2,
            .address = I2C_ADDRESS,
            .address_width = I2C_ADDRESS_WIDTH_7BIT,
            .speed_mode = I2C_STANDARD_SPEED_MODE
    };

    wiced_i2c_init(&i2cDevice);

    uint8_t tx_buffer[] = {0,0};
    wiced_i2c_message_t msg;
    wiced_i2c_init_tx_message(&msg, tx_buffer, sizeof(tx_buffer), RETRIES, DISABLE_DMA);

    tx_buffer[0] = CONTROL_REG;
    tx_buffer[1] = 0x01;
    wiced_i2c_transfer(&i2cDevice, &msg, NUM_MESSAGES);

    tx_buffer[0] = LED_REG;

    while(1){
        wiced_rtos_delay_milliseconds(200);
        if(buttonPress){
            forward = (forward ? WICED_FALSE : WICED_TRUE);
            buttonPress = WICED_FALSE;
        }
        if(forward){
            tx_buffer[1] = tx_buffer[1] >> 1;
            if(tx_buffer[1] < 0x01){
                tx_buffer[1] = 0x08;
            }
            wiced_i2c_transfer(&i2cDevice, &msg, NUM_MESSAGES);
        } else {
            tx_buffer[1] = tx_buffer[1] << 1;
            if(tx_buffer[1] > 0x08){
                tx_buffer[1] = 0x01;
            }
            wiced_i2c_transfer(&i2cDevice, &msg, NUM_MESSAGES);
        }
    }
}
