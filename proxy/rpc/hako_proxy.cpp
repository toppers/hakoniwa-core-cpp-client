#include "lib/hako_client.h"
#include "proc/hako_process.hpp"
#include "nlohmann/json.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
using json = nlohmann::json;

using namespace hako::client;
using namespace hako::client::proxy;

typedef struct {
    HakoniwaAssetParamInfoType param_file;
    json param;
    ProcessManager process;
    HakoniwaAssetInfoType* asset;
} HakoniwaProxyControllerType;


static ErcdType init(char* ip_port, HakoniwaProxyControllerType* ctrlp)
{
    ErcdType err;

    hakoniwa_core_init(ip_port);

    err = hakoniwa_core_asset_register(&ctrlp->param_file);
    if (err != Ercd_OK) {
        printf("ERROR: hakoniwa_core_asset_register() returns %d\n", err);
        return err;
    }
    printf("INFO: Register Asset %s success\n", ctrlp->param["asset_name"].get<std::string>().c_str());
    err = hakoniwa_core_asset_notification_start();
    if (err != Ercd_OK) {
        printf("hakoniwa_core_asset_notification_start() returns %d\n", err);
        return err;
    }
    printf("INFO: Notification Setting success\n");
    ctrlp->process.set_current_dir(ctrlp->param["target_exec_dir"]);
    ctrlp->process.set_binary_path(ctrlp->param["target_bin_path"]);
    for (int i = 0; i < ctrlp->param["target_options"].size(); i++) {
        ctrlp->process.add_option(ctrlp->param["target_options"][i]);
    }
    return Ercd_OK;
}

int main(int argc, char** argv)
{
    HakoniwaProxyControllerType ctrl;
    bool real_time_sync = false;
    char ip_port[128];
    char* param_file = nullptr;

    if ((argc != 4) && (argc != 5)) {
        printf("Usage: %s <param_file> <ipaddr> <portno> [sync]\n", argv[0]);
        return 1;
    }
    HakoniwaAssetInfoType asset;
    param_file = argv[1];
    ctrl.param_file.param_filename = param_file;
    std::ifstream ifs(param_file);
    ctrl.param = json::parse(ifs);

    sprintf(ip_port, "%s:%s", argv[2], argv[3]);
    ErcdType err = init(ip_port, &ctrl);
    if (err != Ercd_OK) {
        return 1;
    }
    if (argc == 5) {
        HakoniwaAssetTimeSyncConfigType cfg;
        cfg.tx_ipaddr = argv[2];
        cfg.tx_portno = ctrl.param["sync_tx_portno"].get<int>();
        cfg.rx_ipaddr = (char*)ctrl.param["sync_rx_ipaddr"].get<std::string>().c_str();
        cfg.rx_portno = ctrl.param["sync_rx_portno"].get<int>();
        cfg.interval_msec = ctrl.param["sync_interval_msec"].get<int>(); /* msec */
        err = hakoniwa_asset_real_time_sync_init(&cfg);
        if (err != Ercd_OK) {
            return 1;
        }
        real_time_sync = true;
    }

    while (true) {
        HakoniwaAssetEventType ev = hakoniwa_core_asset_get_event();
        if (ev.type == HakoniwaAssetEvent_None) {
            break;
        }
        bool result = false;
        switch (ev.type) {
        case HakoniwaAssetEvent_Start:
            result = ctrl.process.invoke();
            if (real_time_sync) {
                (void)hakoniwa_asset_real_time_sync_start();
            }
            break;
        case HakoniwaAssetEvent_End:
            result = true;
            ctrl.process.terminate();
            if (real_time_sync) {
                (void)hakoniwa_asset_real_time_sync_stop();
            }
            break;
        case HakoniwaAssetEvent_Heartbeat:
            result = true;
            break;
        default:
            break;
        }
        if (result) {
            hakoniwa_core_asset_event_feedback(ev.type, Ercd_OK);
        }
        else {
            hakoniwa_core_asset_event_feedback(ev.type, Ercd_NG);
        }
    }
    if (ctrl.process.is_running()) {
        ctrl.process.terminate();
    }
    err = hakoniwa_core_asset_unregister();
    printf("INFO Unregister Asset %s result=%d\n", ctrl.param["asset_name"].get<std::string>().c_str(), err);
    return 0;
}