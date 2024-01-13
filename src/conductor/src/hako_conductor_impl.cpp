#include "hako_conductor_impl.hpp"

std::shared_ptr<hako::IHakoMasterController> hako_conductor = nullptr;
static bool hako_conductor_cmd_stop = false;
static void* hako_conductor_impl_thread_run(void* arg)
{
    std::cout << "INFO: hako_conductor thread start" << std::endl;
    if (arg) {
        //nothing to do
    }
    while (hako_conductor_cmd_stop == false) {
        try {
            hako_conductor->execute();
        } catch (std::exception *e) {
            std::cerr << "ERROR: hako_conductor.execute() Failed" << std::endl;
            return nullptr;
        }
    }
    hako_conductor = nullptr;
    return nullptr;
}

bool hako_conductor_impl_start(hako_time_t delta_usec, hako_time_t max_delay_usec)
{
    if (hako_conductor != nullptr) {
        return false;
    }
    pthread_t thread;
    try {
        hako::init();
        hako_conductor = hako::create_master();
        if (hako_conductor == nullptr) {
            std::cout << "INFO: hako::create_master() Failed" << std::endl;
            return false;
        }
        hako_conductor->set_config_simtime(max_delay_usec, delta_usec);
    } catch (std::exception *e) {
        std::cout << "INFO: hako::create_master() Failed" << std::endl;
        return false;
    }
    
    if (pthread_create(&thread, NULL, hako_conductor_impl_thread_run, nullptr) != 0) {
        std::cerr << "ERROR: Failed to create hako_conductor_impl_thread_run thread!" << std::endl;
        return false;
    }
    return true;
}
void hako_conductor_impl_stop(void)
{
    hako_conductor_cmd_stop = true;
    while (hako_conductor != nullptr) {
        usleep(HAKO_CONDUCTOR_WAIT_TIME_USEC);
    }
    return;
}
