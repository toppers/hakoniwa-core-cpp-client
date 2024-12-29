#include "hako_asset.h"
#include "hako_conductor.h"
#include "std_msgs/pdu_ctype_UInt32.h"
#include <stdio.h>
#include <stdlib.h>
#ifdef _WIN32
#include <windows.h>
static inline void usleep(long microseconds) {
    Sleep(microseconds / 1000);
}
#else
#include <unistd.h>
#endif

static int my_on_initialize(hako_asset_context_t* context)
{
    (void)context;
    printf("INFO: my_on_initialize enter\n");
    printf("INFO: sleep 1sec\n");
    usleep(1000*1000);
    printf("INFO: my_on_initialize exit\n");
    return 0;
}
static int my_on_reset(hako_asset_context_t* context)
{
    (void)context;
    printf("INFO: my_on_reset enter\n");
    printf("INFO: sleep 1sec\n");
    usleep(1000*1000);
    printf("INFO: my_on_reset exit\n");
    return 0;
}
static int my_on_simulation_step(hako_asset_context_t* context)
{
    (void)context;
    static unsigned int count = 0;
    Hako_UInt32 rvalue;
    Hako_UInt32 value;
    int ret = hako_asset_pdu_read("Robot", 0, (char*)(&rvalue), sizeof(rvalue));
    if (ret != 0) {
        printf("ERROR: hako_asset_pdu_read error: %d\n", ret);
    }
    value.data = count;
    ret = hako_asset_pdu_write("Robot", 1, (const char*)(&value), sizeof(value));
    if (ret != 0) {
        printf("ERROR: hako_asset_pdu_write error: %d\n", ret);
    }
    printf("hako_asset_pdu_read: %d\n", rvalue.data);
    printf("hako_asset_pdu_write: %d\n", value.data);
    usleep(1000*1000);
    count++;
    return 0;
}
static hako_asset_callbacks_t my_callback = {
    .on_initialize = my_on_initialize,
    .on_manual_timing_control = NULL,
    .on_simulation_step = my_on_simulation_step,
    .on_reset = my_on_reset
};
int main(int argc, const char* argv[])
{
    if (argc != 4) {
        printf("Usage: %s <asset_name> <config_path> <delta_time_msec>\n", argv[0]);
        return 1;
    }
    const char* asset_name = argv[1];
    const char* config_path = argv[2];
    hako_time_t delta_time_usec = atoi(argv[3]) * 1000;

    hako_conductor_start(delta_time_usec, delta_time_usec);
    int ret = hako_asset_register(asset_name, config_path, &my_callback, delta_time_usec, HAKO_ASSET_MODEL_PLANT);
    if (ret != 0) {
        printf("ERORR: hako_asset_register() returns %d.", ret);
        return 1;
    }
    ret = hako_asset_start();
    printf("INFO: hako_asset_start() returns %d\n", ret);

    hako_conductor_stop();
    return 0;
}
