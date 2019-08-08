#include "wiced.h"

void printIp(wiced_ip_address_t ipV4address)
{
    WPRINT_APP_INFO(("%d.%d.%d.%d\r\n",
            (int)((ipV4address.ip.v4 >> 24) & 0xFF),
            (int)((ipV4address.ip.v4 >> 16) & 0xFF),
            (int)((ipV4address.ip.v4 >> 8) & 0xFF),
            (int)(ipV4address.ip.v4 & 0xFF)));
}

void application_start(){

    wiced_result_t connectResult;
    wiced_bool_t led = WICED_FALSE;
    uint8_t ledToBlink;
    wiced_ip_address_t ipAddress;
    wiced_mac_t mac;

    wiced_init();

    connectResult = wiced_network_up(WICED_STA_INTERFACE, WICED_USE_EXTERNAL_DHCP_SERVER, NULL);

    /* Print out network info */
    WPRINT_APP_INFO(("NETWORK INFO\r\n"));
    /* IP address */
    wiced_ip_get_ipv4_address(WICED_STA_INTERFACE, &ipAddress);
    WPRINT_APP_INFO(("IP addr: "));
    printIp(ipAddress);
    /* Netmask */
    wiced_ip_get_netmask(WICED_STA_INTERFACE, &ipAddress);
    WPRINT_APP_INFO(("Netmask: "));
    printIp(ipAddress);
    /* Gateway */
    wiced_ip_get_gateway_address(WICED_STA_INTERFACE, &ipAddress);
    WPRINT_APP_INFO(("Gateway: "));
    printIp(ipAddress);
    /* Cypress.com Address */
    wiced_hostname_lookup("www.scu.edu", &ipAddress, WICED_NEVER_TIMEOUT,
            WICED_STA_INTERFACE);
    WPRINT_APP_INFO(("SCU: "));
    printIp(ipAddress);

    /*Device MAC Address*/
    wwd_wifi_get_mac_address(&mac, WICED_STA_INTERFACE);
    WPRINT_APP_INFO(("MAC Address: "));
    WPRINT_APP_INFO(("%X:%X:%X:%X:%X:%X\r\n",
            mac.octet[0], mac.octet[1], mac.octet[2],
            mac.octet[3], mac.octet[4], mac.octet[5]));
    if(connectResult == WICED_SUCCESS)
    {
        ledToBlink = WICED_SH_LED1; /* Blink LED1 if successful */
    }
    else
    {
        ledToBlink = WICED_SH_LED0; /* Blink LED0 if unsuccessful */
    }
    while ( 1 )
    {
        /* Blink appropriate LED */
        if ( led == WICED_TRUE )
        {
            wiced_gpio_output_low( ledToBlink );
            led = WICED_FALSE;
        }
        else
        {
            wiced_gpio_output_high( ledToBlink );
            led = WICED_TRUE;
        }
        wiced_rtos_delay_milliseconds(250);
    }
}
