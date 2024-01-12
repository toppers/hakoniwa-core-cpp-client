#include "hako_asset.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static int my_on_initialize(hako_asset_context_t* context)
{
    printf("INFO: my_on_initialize enter\n");
    printf("INFO: sleep 1sec\n");
    usleep(1000*1000);
    printf("INFO: my_on_initialize exit\n");
    return 0;
}
static int my_on_reset(hako_asset_context_t* context)
{
    printf("INFO: my_on_reset enter\n");
    printf("INFO: sleep 1sec\n");
    usleep(1000*1000);
    printf("INFO: my_on_reset exit\n");
    return 0;
}
static int on_simulation_step(hako_asset_context_t* context)
{
    printf("INFO: on_simulation_step enter\n");
    printf("INFO: sleep 1sec\n");
    usleep(1000*1000);
    printf("INFO: on_simulation_step exit\n");
    return 0;
}

static hako_asset_callbacks_t my_callback = {
    .on_initialize = my_on_initialize,
    .on_manual_timing_control = NULL,
    .on_simulation_step = on_simulation_step,
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

    int ret = hako_asset_register(asset_name, config_path, &my_callback, delta_time_usec);
    if (ret != 0) {
        printf("ERORR: hako_asset_register() returns %d.", ret);
        return 1;
    }
    ret = hako_asset_start();
    printf("INFO: hako_asset_start() returns %d\n", ret);
    return 0;
}
