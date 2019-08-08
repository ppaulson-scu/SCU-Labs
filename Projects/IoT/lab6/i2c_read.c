#include "wiced.h"

#define I2C_ADDRESS     (0x42)
#define RETRIES         (1)
#define DISABLE_DMA     (WICED_TRUE)
#define NUM_MESSAGES    (1)

#define TEMPERATURE_REG 0x07

#define PWM_PIN WICED_PWM_5

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

    uint8_t tx_buffer[] = {TEMPERATURE_REG};
    wiced_i2c_message_t setOffset;
    wiced_i2c_init_tx_message(&setOffset, tx_buffer, sizeof(tx_buffer), RETRIES, DISABLE_DMA);

    wiced_i2c_transfer(&i2cDevice, &setOffset, NUM_MESSAGES);

    struct {
        float temp;
        float humidity;
        float light;
        float pot;
    } rx_buffer;

    wiced_i2c_message_t msg;
    wiced_i2c_init_rx_message(&msg, &rx_buffer, sizeof(rx_buffer), RETRIES, DISABLE_DMA);

    float duty_cycle = 0.0;
    float light_max = 1.0;

    wiced_gpio_deinit(WICED_SH_LED1);

    while(1){

        wiced_i2c_transfer(&i2cDevice, &msg, NUM_MESSAGES);

        WPRINT_APP_INFO(("Temperature: %.1f\t Humidity: %.1f\t Light: %.1f\t POT: %.1f\n", rx_buffer.temp, rx_buffer.humidity, rx_buffer.light, rx_buffer.pot));


        //Update light_max
        light_max = (light_max >= rx_buffer.light) ? light_max : rx_buffer.light;
        //Set duty cycle
        duty_cycle = (rx_buffer.light / light_max) * 100;

        wiced_pwm_init(PWM_PIN, 1000, duty_cycle);
        wiced_pwm_start(PWM_PIN);

        wiced_rtos_delay_milliseconds(20);
    }
}
