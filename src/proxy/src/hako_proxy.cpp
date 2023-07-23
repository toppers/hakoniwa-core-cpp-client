#include "proc/hako_process.hpp"
#include "hako.hpp"
#include "nlohmann/json.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>

using json = nlohmann::json;

using namespace hako::client;
using namespace hako::client::proxy;

typedef struct {
    std::string asset_name_str;
    std::string robo_name_str;
    json param;
    ProcessManager process;
    std::shared_ptr<hako::IHakoSimulationEventController> hako_sim_ctrl;
    std::shared_ptr<hako::IHakoAssetController> hako_asset;
    bool is_end;
} HakoniwaProxyControllerType;

static HakoniwaProxyControllerType ctrl;

static void hako_proxy_signal_handler(int sig);
static void reset_callback();
static void start_callback();
static void stop_callback();
static int hako_init();

int main(int argc, char** argv)
{
    if ((argc != 2)) {
        printf("Usage: %s <param_file>\n", argv[0]);
        return 1;
    }
    char* param_file = nullptr;
    param_file = argv[1];
    std::ifstream ifs(param_file);
    ctrl.is_end = false;
    ctrl.param = json::parse(ifs);

    ctrl.process.set_current_dir(ctrl.param["target_exec_dir"]);
    ctrl.process.set_binary_path(ctrl.param["target_bin_path"]);
    for (unsigned int i = 0; i < ctrl.param["target_options"].size(); i++) {
        ctrl.process.add_option(ctrl.param["target_options"][i]);
    }
    signal(SIGINT, hako_proxy_signal_handler);
    signal(SIGTERM, hako_proxy_signal_handler);
    //signal(SIGCHLD, hako_proxy_signal_handler);
    std::cout << "INFO: PROXY start" << std::endl;

    if (hako_init() != 0) {
        return 1;
    }
    while (ctrl.is_end == false)
    {
        usleep(1000000);
    }
    if (ctrl.process.is_running()) {
        ctrl.process.terminate();
    }
    ctrl.hako_asset->asset_unregister(ctrl.asset_name_str);
    //hako::logger::get(ctrl.asset_name_str)->flush();

    std::cout << "INFO: PROXY exit" << std::endl;
    return 0;
}

/* ----------- static ---------- */

static void hako_proxy_signal_handler(int sig)
{
    std::cout << "INFO: SIGNAL received: " << sig << std::endl;
    ctrl.is_end = true;
}

static void reset_callback()
{
    ctrl.hako_asset->reset_feedback(ctrl.asset_name_str, true);
}
static void start_callback()
{
    bool ret = ctrl.process.invoke();
    if (ret == false) {
        std::string path = ctrl.param["target_bin_path"];
        printf("ERROR: can not invoke process %s¥n", path.c_str());
    }
    if ((ctrl.param["suppress_start_feedback"] == nullptr) || (ctrl.param["suppress_start_feedback"].get<bool>() == false)) {
        ctrl.hako_asset->start_feedback(ctrl.asset_name_str, ret);
    }
}
static void stop_callback()
{
    ctrl.process.terminate();
    ctrl.hako_asset->stop_feedback(ctrl.asset_name_str, true);
}

static int hako_init()
{
    ctrl.asset_name_str = ctrl.param["asset_name"];
    ctrl.robo_name_str = ctrl.param["robo_name"];
    //hako::logger::init("core");
    //hako::logger::init(ctrl.asset_name_str);
    //hako::logger::get(ctrl.asset_name_str)->info("asset_name={0}", ctrl.asset_name_str);

    ctrl.hako_asset = hako::create_asset_controller();
    if (ctrl.hako_asset == nullptr) {
        std::cout << "ERROR: Not found hako-master on this PC" << std::endl;
        return 1;
    }
    AssetCallbackType callback;
    callback.reset = reset_callback;
    callback.start = start_callback;
    callback.stop = stop_callback;
    bool ret = ctrl.hako_asset->asset_register(ctrl.asset_name_str, callback);
    if (ret == false) {
        std::cout << "ERROR: Can not register asset" << std::endl;
        return 1;
    }
    for (unsigned int i = 0; i < ctrl.param["target_channels"].size(); i++) {
        std::cout << "target_channels: " << i << " target_channels: " << ctrl.param["target_channels"][i].at("size") << std::endl;
        bool err = ctrl.hako_asset->create_pdu_lchannel(
            ctrl.robo_name_str,
            ctrl.param["target_channels"][i].at("channel_id"), 
            ctrl.param["target_channels"][i].at("size")
            );
        if (err == false) {
            std::cout << "ERROR: Can not create_pdu_channel()" << std::endl;
            return 1;
        }
    }
    return 0;
}
