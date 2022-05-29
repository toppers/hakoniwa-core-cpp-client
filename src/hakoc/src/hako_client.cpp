#include "hako.hpp"
#include "hako_client.h"
#include <fstream>
#include <iostream>

static std::string hako_asset_name;
static std::shared_ptr<hako::IHakoAssetController>  hako_asset;

int hako_client_init(const char* asset_name)
{
    hako_asset = hako::create_asset_controller();
    if (hako_asset == nullptr) {
        std::cout << "ERROR: Not found hako-master on this PC" << std::endl;
        return 1;
    }
    hako_asset_name = std::string(asset_name);

    return 0;
}

hako_time_t hako_client_get_worldtime()
{
    return (hako_time_t)hako_asset->get_worldtime();
}

void hako_client_notify_simtime(hako_time_t simtime)
{
    hako_asset->notify_simtime(hako_asset_name, (HakoTimeType)simtime);
    return;
}
