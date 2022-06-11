#include "hako.hpp"
#include "hako_capi.h"
#include <fstream>
#include <iostream>

static std::shared_ptr<hako::IHakoMasterController>  hako_master;
static std::shared_ptr<hako::IHakoSimulationEventController>  hako_simevent;
static std::shared_ptr<hako::IHakoAssetController>  hako_asset;

/*
 * for master
 */
bool hako_master_init()
{
    try {
        hako::init();
        hako::logger::init("master");
        std::shared_ptr<hako::IHakoMasterController> hako_master = hako::create_master();
        if (hako_master == nullptr) {
            return false;
        }
        std::shared_ptr<hako::IHakoSimulationEventController> hako_simevent = hako::get_simevent_controller();
        if (hako_simevent == nullptr) {
            return false;
        }
        return true;
    } catch (std::exception *e) {
        hako::logger::get("master")->error(e->what());
        return false;
    }
}

bool hako_master_execute()
{
    try {
        return hako_master->execute();
    } catch (std::exception *e) {
        hako::logger::get("master")->error(e->what());
        return false;
    }
}

void hako_master_set_config_simtime(hako_time_t max_delay_time_usec, hako_time_t delta_time_usec)
{
    try {
        hako::logger::get("master")->info("max_delay={0} usec delta={1} usec", max_delay_time_usec, delta_time_usec);
        hako_master->set_config_simtime(max_delay_time_usec, delta_time_usec);
    } catch (std::exception *e) {
        hako::logger::get("master")->error(e->what());
        return;
    }
    return;
}
hako_time_t hako_master_get_max_deltay_time_usec()
{
    try {
        return hako_master->get_max_deltay_time_usec();
    } catch (std::exception *e) {
        hako::logger::get("master")->error(e->what());
        return 0;
    }
}
hako_time_t hako_master_get_delta_time_usec()
{
    try {
        return hako_master->get_delta_time_usec();
    } catch (std::exception *e) {
        hako::logger::get("master")->error(e->what());
        return 0;
    }
}

/*
 * for asset
 */
bool hako_asset_init()
{
    try {
        hako::logger::init("core");
        hako::logger::init("asset");
        hako_asset = hako::create_asset_controller();
        if (hako_asset == nullptr) {
            std::cout << "ERROR: Not found hako-master on this PC" << std::endl;
            return 1;
        }
        return true;
    } catch (std::exception *e) {
        hako::logger::get("asset")->error(e->what());
        return false;
    }
}

bool hako_asset_register(const char* name, hako_asset_callback_t *callbacks)
{
    try {
        std::string asset_name(name);
        AssetCallbackType cbk;
        cbk.start = callbacks->start;
        cbk.stop = callbacks->stop;
        cbk.reset = callbacks->reset;
        return hako_asset->asset_register(asset_name, cbk);
    }
    catch (std::exception *e) {
        hako::logger::get("asset")->error(e->what());
        return false;
    }
}

bool hako_asset_unregister(const char* name)
{
    try {
        std::string asset_name(name);
        return hako_asset->asset_unregister(asset_name);
    } catch (std::exception *e) {
        hako::logger::get("asset")->error(e->what());
        return false;
    }
}

void hako_asset_notify_simtime(const char* name, hako_time_t simtime)
{
    try {
        std::string asset_name(name);
        hako_asset->notify_simtime(asset_name, (HakoTimeType)simtime);
    }
    catch (std::exception *e) {
        hako::logger::get("asset")->error(e->what());
        return;
    }
}

hako_time_t hako_asset_get_worldtime()
{
    try {
        return (hako_time_t)hako_asset->get_worldtime();
    } catch (std::exception *e) {
        hako::logger::get("asset")->error(e->what());
        return 0;
    }
}


bool hako_asset_start_feedback(const char* asset_name, bool isOk)
{
    try {
        std::string hako_asset_name(asset_name);
        return hako_asset->start_feedback(hako_asset_name, isOk);
    } catch (std::exception *e) {
        hako::logger::get("asset")->error(e->what());
        return 0;
    }
}

bool hako_asset_stop_feedback(const char* asset_name, bool isOk)
{
    try {
        std::string hako_asset_name(asset_name);
        return hako_asset->stop_feedback(hako_asset_name, isOk);
    } catch (std::exception *e) {
        hako::logger::get("asset")->error(e->what());
        return 0;
    }
}

bool hako_asset_reset_feedback(const char* asset_name, bool isOk)
{
    try {
        std::string hako_asset_name(asset_name);
        return hako_asset->reset_feedback(hako_asset_name, isOk);
    } catch (std::exception *e) {
        hako::logger::get("asset")->error(e->what());
        return 0;
    }
}


/*
 * for simevent
 */
int hako_simevent_get_state()
{
    try {
        return (int)hako_simevent->state();
    } catch (std::exception *e) {
        hako::logger::get("master")->error(e->what());
        return -1;
    }
}

bool hako_simevent_start()
{
    try {
        return hako_simevent->start();
    } catch (std::exception *e) {
        hako::logger::get("master")->error(e->what());
        return false;
    }
}

bool hako_simevent_stop()
{
    try {
        return hako_simevent->stop();
    } catch (std::exception *e) {
        hako::logger::get("master")->error(e->what());
        return false;
    }
}

bool hako_simevent_reset()
{
    try {
        return hako_simevent->reset();
    } catch (std::exception *e) {
        hako::logger::get("master")->error(e->what());
        return false;
    }
}
