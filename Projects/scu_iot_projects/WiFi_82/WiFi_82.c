#include "wiced.h"
#include "wiced_time.h"
#include "math.h"

#define NUMTRIALS (10)
#define TRIALTIME (5000)

void application_start(){

    wiced_result_t connectResult;
    uint8_t count = 0;
    int noise[NUMTRIALS];
    int rssi[NUMTRIALS];

    wiced_init();

    connectResult = wiced_network_up(WICED_STA_INTERFACE, WICED_USE_EXTERNAL_DHCP_SERVER, NULL);

    while(connectResult != WICED_SUCCESS){
        wiced_rtos_delay_milliseconds(1000);
        connectResult = wiced_network_up(WICED_STA_INTERFACE, WICED_USE_EXTERNAL_DHCP_SERVER, NULL);
    }

    for(int i = 0; i < NUMTRIALS; i++){
        uint32_t noise_sample, rssi_sample;
        wwd_wifi_get_noise(&noise_sample);
        wwd_wifi_get_rssi(&rssi_sample);
        noise[i] = noise_sample;
        rssi[i] = rssi_sample;
        WPRINT_APP_INFO(("Noise Sample %d: %d\n", i, noise_sample));
        WPRINT_APP_INFO(("RSSI Sample %d: %d\n", i, rssi_sample));
        wiced_rtos_delay_milliseconds(1000);
    }

    wiced_network_down(WICED_STA_INTERFACE);

    int noise_mean = 0;
    int rssi_mean = 0;

    for(int i = 0; i < NUMTRIALS; i++){
        noise_mean += noise[i];
        rssi_mean += rssi[i];
    }

    noise_mean /= NUMTRIALS;
    rssi_mean /= NUMTRIALS;

    WPRINT_APP_INFO(("Noise Mean: %d\n", noise_mean));
    WPRINT_APP_INFO(("RSSI Mean: %d\n", rssi_mean));

    int noise_stddev = 0;
    int rssi_stddev = 0;

    for (int i = 0; i < NUMTRIALS; i++){
        noise_stddev += (noise[i] - noise_mean) * (noise[i] - noise_mean);
        rssi_stddev += (rssi[i] - rssi_mean) * (rssi[i] - rssi_mean);
    }

    noise_stddev = sqrt((1. / (NUMTRIALS - 1)) * noise_stddev);
    rssi_stddev = sqrt((1. / (NUMTRIALS - 1)) * rssi_stddev);

    WPRINT_APP_INFO(("Noise Stddev: %d.\n", noise_stddev));
    WPRINT_APP_INFO(("RSSI Stddev: %d.\n", rssi_stddev));

}
