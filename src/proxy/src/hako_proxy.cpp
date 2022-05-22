#include "proc/hako_process.hpp"
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
    json param;
    ProcessManager process;
    bool is_end;
} HakoniwaProxyControllerType;

static HakoniwaProxyControllerType ctrl;
static void hako_proxy_signal_handler(int sig)
{
    std::cout << "INFO: SIGNAL received: " << sig << std::endl;
    ctrl.is_end = true;
    ctrl.process.terminate();
}
void proc_exit(int sig)
{
    std::cout << "INFO: SIGNAL received SIGCHLD: " << sig << std::endl;
	int wstat;

    wait3(&wstat, WNOHANG, (struct rusage *)nullptr);
    ctrl.is_end = true;
}

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
    signal(SIGCHLD, proc_exit);
    std::cout << "INFO: PROXY start" << std::endl;

    bool ret = ctrl.process.invoke();
    if (ret == false) {
        std::string path = ctrl.param["target_bin_path"];
        printf("ERROR: can not invoke process %s¥n", path.c_str());
    }
    while (ctrl.is_end == false)
    {
        usleep(1000000);
    }
    std::cout << "INFO: PROXY exit" << std::endl;
    return 0;
}
