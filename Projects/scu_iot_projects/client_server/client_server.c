#include "wiced.h"

#define TCP_CLIENT_STACK_SIZE   (6200)
#define SERVER_PORT             (6999)

static wiced_ip_address_t serverAddress;
static wiced_thread_t listen;
static wiced_mac_t myMac;


void setState(int led, int value){
    if(led){
        if(value){
            wiced_gpio_output_high(WICED_SH_LED1);
        } else {
            wiced_gpio_output_low(WICED_SH_LED1);
        }
    } else {
        if(value){
            wiced_gpio_output_high(WICED_SH_LED0);
        } else {
            wiced_gpio_output_low(WICED_SH_LED0);
        }
    }
}

void connectionListener(){
    wiced_tcp_socket_t socket;
    wiced_tcp_stream_t stream;
    char sendMessage[20];
    wiced_result_t result, conStatus;

    WPRINT_APP_INFO(("Beginning connection.\n"));

    sprintf(sendMessage,"WRITE-%02X%02X%02X%02X%02X%02X-%01X\n",
                    myMac.octet[0], myMac.octet[1], myMac.octet[2],
                    myMac.octet[3], myMac.octet[4], myMac.octet[5],
                    0);

    wiced_tcp_create_socket(&socket, WICED_STA_INTERFACE);
    wiced_tcp_bind(&socket, WICED_ANY_PORT);
    conStatus = wiced_tcp_connect(&socket, &serverAddress, SERVER_PORT, 2000);

    if(conStatus == WICED_SUCCESS){
        WPRINT_APP_INFO(("Connection successful.\n"));
    } else {
        WPRINT_APP_INFO(("Connection failed.\n"));
        wiced_tcp_delete_socket(&socket);
        return;
    }

    wiced_tcp_stream_init(&stream, &socket);

    wiced_tcp_stream_write(&stream, sendMessage, strlen(sendMessage));
    wiced_tcp_stream_flush(&stream);
    while(1){

            wiced_tcp_stream_write(&stream, sendMessage, strlen(sendMessage));
            wiced_tcp_stream_flush(&stream);

            char rbuffer[30] = {0};

            uint32_t read_count;
            result = wiced_tcp_stream_read_with_count(&stream, rbuffer, 28, 600, &read_count);

            if(result == WICED_SUCCESS){
                WPRINT_APP_INFO(("Server response: %s\n\n\n", rbuffer));
                setState(rbuffer[0] - '0', rbuffer[2] - '0');
            } else {
                WPRINT_APP_INFO(("Malformed response: %s\n\n\n", rbuffer));
            }
    }
}

void application_start(void){
    wiced_init();
    wiced_network_up(WICED_STA_INTERFACE, WICED_USE_EXTERNAL_DHCP_SERVER, NULL);

    wiced_result_t result;

    wwd_wifi_get_mac_address(&myMac, WICED_STA_INTERFACE);

    WPRINT_APP_INFO(("DNS Lookup IoT Server\n"));
    result = wiced_hostname_lookup( "Landon-Laptop", &serverAddress, 5000, WICED_STA_INTERFACE);

    if(result == WICED_ERROR || serverAddress.ip.v4 == 0){
        WPRINT_APP_INFO(("Failed to find address using DNS lookup.\n"));
        SET_IPV4_ADDRESS( serverAddress, MAKE_IPV4_ADDRESS(129, 210, 115, 240));
    }

    WPRINT_APP_INFO(("server's IP : %u.%u.%u.%u\n\n",
            (uint8_t)(GET_IPV4_ADDRESS(serverAddress) >> 24),
            (uint8_t)(GET_IPV4_ADDRESS(serverAddress) >> 16),
            (uint8_t)(GET_IPV4_ADDRESS(serverAddress) >> 8),
            (uint8_t)(GET_IPV4_ADDRESS(serverAddress) >> 0)));

    WPRINT_APP_INFO(("MY MAC Address: "));
    WPRINT_APP_INFO(("%X:%X:%X:%X:%X:%X\r\n",
            myMac.octet[0], myMac.octet[1], myMac.octet[2],
            myMac.octet[3], myMac.octet[4], myMac.octet[5]));

    //wiced_rtos_init_semaphore(&button0_semaphore);
    //wiced_rtos_init_semaphore(&button1_semaphore);

    //wiced_gpio_input_irq_enable(WICED_SH_MB0,
    //        IRQ_TRIGGER_FALLING_EDGE, button_isr0, NULL);
    //wiced_gpio_input_irq_enable(WICED_SH_MB1,
    //        IRQ_TRIGGER_FALLING_EDGE, button_isr1, NULL);
    wiced_rtos_create_thread(&listen, WICED_DEFAULT_LIBRARY_PRIORITY,
            "Connection Listener", connectionListener, TCP_CLIENT_STACK_SIZE, 0);
    //WPRINT_APP_INFO(("Activated button threads..."));

}
