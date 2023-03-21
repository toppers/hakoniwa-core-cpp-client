#include "hako.hpp"
#include "hako_client.h"
#include <fstream>
#include <iostream>

static std::string hako_client_name;
static std::shared_ptr<hako::IHakoAssetController>  hako_asset;

int hako_client_init(const char* asset_name)
{
    hako_asset = hako::create_asset_controller();
    if (hako_asset == nullptr) {
        std::cout << "ERROR: Not found hako-master on this PC" << std::endl;
        return 1;
    }
    hako_client_name = std::string(asset_name);

    return 0;
}

hako_time_t hako_client_get_worldtime()
{
    return (hako_time_t)hako_asset->get_worldtime();
}

void hako_client_notify_simtime(hako_time_t simtime)
{
    hako_asset->notify_simtime(hako_client_name, (HakoTimeType)simtime);
    return;
}

int hako_client_create_pdu_channel(HakoPduChannelIdType channel_id, size_t pdu_size)
{
    try {
        if (hako_asset->create_pdu_channel(channel_id, pdu_size) == true) {
            return 0;
        }
        else {
            return -1;
        }
    } catch (std::exception *e) {
        //hako::logger::get("core")->error(e->what());
        return -1;
    }
}

int hako_client_create_pdu_lchannel(const char* robo_name, HakoPduChannelIdType channel_id, size_t pdu_size)
{
    try {
        std::string hako_client_name(robo_name);
        if (hako_asset->create_pdu_lchannel(hako_client_name, channel_id, pdu_size) == true) {
            return 0;
        }
        else {
            return -1;
        }
    } catch (std::exception *e) {
        //hako::logger::get("core")->error(e->what());
        return -1;
    }
}
int hako_client_start_feedback()
{
    try {
        if (hako_asset->start_feedback(hako_client_name, true) == true) {
            return 0;
        }
        else {
            return -1;
        }
    } catch (std::exception *e) {
        //hako::logger::get("core")->error(e->what());
        return -1;
    }
}

int hako_client_pdu_is_dirty(const char* asset_name, const char* robo_name, HakoPduChannelIdType channel_id)
{
    try {
        std::string hako_asset_name(asset_name);
        std::string hako_client_name(robo_name);
        if (hako_asset->is_pdu_dirty(hako_asset_name, hako_client_name, channel_id) == true) {
            return 0;
        }
        else {
            return -1;
        }
    } catch (std::exception *e) {
        //hako::logger::get("core")->error(e->what());
        return -1;
    }
}
int hako_client_write_pdu(const char* asset_name, const char* robo_name, HakoPduChannelIdType channel_id, const char *pdu_data, size_t len)
{
    try {
        std::string hako_asset_name(asset_name);
        std::string hako_client_name(robo_name);
        if (hako_asset->write_pdu(hako_asset_name, hako_client_name, channel_id, pdu_data, len) == true) {
            return 0;
        }
        else {
            return -1;
        }
    } catch (std::exception *e) {
        //hako::logger::get("core")->error(e->what());
        return -1;
    }
}

int hako_client_read_pdu(const char* asset_name, const char* robo_name, HakoPduChannelIdType channel_id, char *pdu_data, size_t len)
{
    try {
        std::string hako_asset_name(asset_name);
        std::string hako_client_name(robo_name);
        if (hako_asset->read_pdu(hako_asset_name, hako_client_name, channel_id, pdu_data, len) == true) {
            return 0;
        }
        else {
            return -1;
        }
    } catch (std::exception *e) {
        //hako::logger::get("core")->error(e->what());
        return -1;
    }
}

void hako_client_notify_read_pdu_done(const char* asset_name)
{
    try {
        std::string hako_client_name(asset_name);
        hako_asset->notify_read_pdu_done(hako_client_name);
    } catch (std::exception *e) {
        //hako::logger::get("core")->error(e->what());
        return;
    }
}

void hako_client_notify_write_pdu_done(const char* asset_name)
{
    try {
        std::string hako_client_name(asset_name);
        hako_asset->notify_write_pdu_done(hako_client_name);
    } catch (std::exception *e) {
        //hako::logger::get("core")->error(e->what());
        return;
    }
}

int hako_client_is_pdu_sync_mode(const char* asset_name)
{
    try {
        std::string hako_client_name(asset_name);
        if (hako_asset->is_pdu_sync_mode(hako_client_name) == true) {
            return 0;
        }
        else {
            return -1;
        }
    } catch (std::exception *e) {
        //hako::logger::get("core")->error(e->what());
        return -1;
    }
}

int hako_client_is_simulation_mode()
{
    try {
        if (hako_asset->is_simulation_mode() == true) {
            return 0;
        }
        else {
            return -1;
        }
    } catch (std::exception *e) {
        //hako::logger::get("core")->error(e->what());
        return -1;
    }
}

int hako_client_is_pdu_created()
{
    try {
        if (hako_asset->is_pdu_created() == true) {
            return 0;
        }
        else {
            return -1;
        }
    } catch (std::exception *e) {
        //hako::logger::get("core")->error(e->what());
        return -1;
    }
}