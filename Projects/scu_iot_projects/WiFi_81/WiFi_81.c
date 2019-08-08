#include "wiced.h"
#include "wiced_time.h"
#include "math.h"

#define NUMTRIALS (20)
#define TRIALTIME (5000)

void application_start(){

    wiced_result_t connectResult;
    uint8_t count = 0;
    uint32_t durations[NUMTRIALS];

    wiced_init();


    while(1){
        if (count >= NUMTRIALS) break;

        connectResult = wiced_network_up(WICED_STA_INTERFACE, WICED_USE_EXTERNAL_DHCP_SERVER, NULL);

        //If connection failed, retry
        if(connectResult != WICED_SUCCESS) continue;

        //Connection success
        //Stay connected for 5s
        uint32_t startTime, endTime;
        wiced_time_get_time(&startTime);
        wiced_rtos_delay_milliseconds(TRIALTIME);
        //Disconnect
        wiced_network_down(WICED_STA_INTERFACE);
        wiced_time_get_time(&endTime);
        durations[count] = endTime - startTime;
        WPRINT_APP_INFO(("Duration %d: %d.%.3d s\n", count + 1, (endTime - startTime)/1000, (endTime - startTime)%1000));
        wiced_rtos_delay_milliseconds(TRIALTIME);
        count++;
    }

    uint32_t mean = 0;;

    for(int i = 0; i < NUMTRIALS; i++){
        mean += durations[i];
    }

    mean /= NUMTRIALS;

    WPRINT_APP_INFO(("Mean: %d.%.3ds\n", mean/1000, mean%1000));

    uint32_t stddev = 0;

    for (int i = 0; i < NUMTRIALS; i++){
        stddev += (durations[i] - mean) * (durations[i] - mean);
    }

    stddev = (uint32_t)sqrt((1. / (NUMTRIALS - 1)) * stddev);

    WPRINT_APP_INFO(("Stddev: %d.%.3ds\n", stddev/1000, stddev%1000));
}
