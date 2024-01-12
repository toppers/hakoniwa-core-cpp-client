#include "hako_asset_impl.hpp"

HakoAssetType hako_asset_instance;

/***********************
 * simulation control
 ***********************/

static void hako_asset_impl_parse_robots(void)
{
    const json& robots_json = hako_asset_instance.param["robots"];
    
    for (const auto& robot_json : robots_json) {
        Robot* robot = new Robot;
        
        // name を取得
        robot->name = robot_json["name"];
        
        // ロボット側が被制御対象のコンフィグファイルなので、READとWRITEは逆になることに注意

        // PduReaders を取得
        if (robot_json.find("shm_pdu_writers") != robot_json.end()) {
            const json& pdu_readers_json = robot_json["shm_pdu_writers"];
            for (const auto& reader_json : pdu_readers_json) {
                PduReader* reader = new PduReader{
                    reader_json["type"],
                    reader_json["org_name"],
                    reader_json["name"],
                    reader_json["channel_id"],
                    reader_json["pdu_size"]
                };
                robot->pdu_readers.push_back(*reader);
            }
        } else {
            // nothing to do
        }
        if (robot_json.find("rpc_pdu_writers") != robot_json.end()) {
            const json& pdu_readers_json = robot_json["rpc_pdu_writers"];
            for (const auto& reader_json : pdu_readers_json) {
                PduReader* reader = new PduReader{
                    reader_json["type"],
                    reader_json["org_name"],
                    reader_json["name"],
                    reader_json["channel_id"],
                    reader_json["pdu_size"]
                };
                robot->pdu_readers.push_back(*reader);
            }
        } else {
            // nothing to do
        }
        
        // PduWriters を取得
        if (robot_json.find("shm_pdu_readers") != robot_json.end()) {
            const json& pdu_writers_json = robot_json["shm_pdu_readers"];
            for (const auto& writer_json : pdu_writers_json) {
                PduWriter* writer = new PduWriter{
                    writer_json["type"],
                    writer_json["org_name"],
                    writer_json["name"],
                    writer_json["write_cycle"],
                    writer_json["channel_id"],
                    writer_json["pdu_size"],
                    writer_json["method_type"]
                };
                robot->pdu_writers.push_back(*writer);
            }
        } else {
            // nothing to do
        }
        if (robot_json.find("rpc_pdu_readers") != robot_json.end()) {
            const json& pdu_writers_json = robot_json["rpc_pdu_readers"];
            for (const auto& writer_json : pdu_writers_json) {
                PduWriter* writer = new PduWriter{
                    writer_json["type"],
                    writer_json["org_name"],
                    writer_json["name"],
                    writer_json["write_cycle"],
                    writer_json["channel_id"],
                    writer_json["pdu_size"],
                    writer_json["method_type"]
                };
                robot->pdu_writers.push_back(*writer);
            }
        } else {
            // nothing to do
        }        
        hako_asset_instance.robots.push_back(*robot);
    }
}

bool hako_asset_impl_init(const char* asset_name, const char* config_path, hako_time_t delta_usec)
{
    hako_asset_instance.is_initialized = false;
    hako::init();
    std::ifstream ifs(config_path);
    
    if (!ifs.is_open()) {
        std::cerr << "Error: Failed to open config file." << std::endl;
        return false;
    }

    hako_asset_instance.asset_name_str = asset_name;
    hako_asset_instance.delta_usec = delta_usec;
    hako_asset_instance.current_usec = 0;
    try {
        hako_asset_instance.param = json::parse(ifs);
        hako_asset_impl_parse_robots();
    } catch (const json::exception& e) {
        std::cerr << "JSON parsing error: " << e.what() << std::endl;
        return false;
    }
    hako_asset_instance.hako_asset = hako::create_asset_controller();
    if (hako_asset_instance.hako_asset == nullptr) {
        std::cerr << "ERROR: Not found hako-master on this PC" << std::endl;
        return false;
    }
    hako_asset_instance.hako_sim = hako::get_simevent_controller();
    if (hako_asset_instance.hako_sim == nullptr) {
        std::cerr << "ERROR: Not found hako-master on this PC" << std::endl;
        return false;
    }
    bool ret = hako_asset_instance.hako_asset->asset_register_polling(asset_name);
    if (ret == false) {
        std::cerr << "ERROR: Can not register asset" << std::endl;
        return false;
    }
    // PDUのチャネルを作成する
    for (const Robot& robot : hako_asset_instance.robots) {
        for (const PduWriter& writer : robot.pdu_writers) {
            std::cout << "Robot: " << robot.name << ", PduWriter: " << writer.name << std::endl;
            std::cout << "channel_id: " << writer.channel_id << " pdu_size: " << writer.pdu_size << std::endl;
            bool err = hako_asset_instance.hako_asset->create_pdu_lchannel(
                robot.name,
                writer.channel_id, 
                writer.pdu_size
            );
            if (err == false) {
                std::cerr << "ERROR: Can not create_pdu_channel()" << std::endl;
                return false;
            }            
        }
    }
    hako_asset_instance.is_initialized = true;
    return true;
}
bool hako_asset_impl_register_callback(const hako_asset_callbacks_t* callback)
{
    hako_asset_instance.callback = callback;
    return true;
}
HakoSimulationStateType hako_asset_impl_state()
{
    return hako_asset_instance.hako_sim->state();
}
static bool hako_asset_impl_wait_state(HakoSimulationStateType target)
{
    do {
        HakoSimulationStateType curr = hako_asset_instance.hako_sim->state();
        if (curr == target) {
            break;
        }
        usleep(WAIT_TIME_USEC);
    } while (true);
    return true;
}

static bool hako_asset_impl_wait_event(HakoSimulationAssetEventType target)
{
    bool target_event_is_occureed = false;
    while (target_event_is_occureed == false) {
        auto event = hako_asset_instance.hako_asset->asset_get_event(hako_asset_instance.asset_name_str);
        target_event_is_occureed = (target == event);
        switch (event) {
            case HakoSimAssetEvent_None:
                usleep(WAIT_TIME_USEC);
                break;
            case HakoSimAssetEvent_Start:
                hako_asset_instance.hako_asset->start_feedback(hako_asset_instance.asset_name_str, true);
                break;
            case HakoSimAssetEvent_Stop:
                hako_asset_instance.hako_asset->stop_feedback(hako_asset_instance.asset_name_str, true);
                break;
            case HakoSimAssetEvent_Reset:
                if (hako_asset_instance.callback != NULL) {
                    if (hako_asset_instance.callback->on_reset != NULL) {
                        hako_asset_instance.callback->on_reset(nullptr);
                    }
                }
                hako_asset_instance.current_usec = 0;
                hako_asset_instance.hako_asset->reset_feedback(hako_asset_instance.asset_name_str, true);
                break;
            default:
                std::cerr << "ERROR: hako_asset_impl_wait_event() unkonwn event= " << event << std::endl;
                return false;
        }
    }
    return true;
}

static bool hako_asset_impl_wait_pdu_created()
{
    do {
        bool result = hako_asset_instance.hako_asset->is_pdu_created();
        if (result) {
            break;
        }
        usleep(WAIT_TIME_USEC);
    } while (true);
    return true;
}

bool hako_asset_impl_wait_running(void)
{
    std::cout << "WAIT START" << std::endl;
    if (hako_asset_impl_wait_event(HakoSimAssetEvent_Start) == false) {
        return false;
    }
    std::cout << "WAIT RUNNING" << std::endl;
    if (hako_asset_impl_wait_state(HakoSim_Running) == false) {
        return false;
    }
    std::cout << "PDU CREATED" << std::endl;
    if (hako_asset_impl_wait_pdu_created() == false) {
        return false;
    }
    if (hako_asset_instance.callback != NULL) {
        if (hako_asset_instance.callback->on_initialize != NULL) {
            hako_asset_instance.callback->on_initialize(nullptr);
        }
    }
    else {
	// nothing to do
    }
    return true;
}
static bool hako_asset_impl_execute(void)
{
    hako_asset_instance.hako_asset->notify_simtime(hako_asset_instance.asset_name_str, hako_asset_instance.current_usec);
    HakoSimulationStateType curr = hako_asset_instance.hako_sim->state();
    if (curr != HakoSim_Running) {
        std::cout << "NOT RUNNING: curr = " << curr << std::endl;
        return false;
    }
    bool result = hako_asset_instance.hako_asset->is_pdu_created();
    if (result == false) {
        std::cout << "PDU IS NOT CREATED" << std::endl;
        return false;
    }
    result = hako_asset_instance.hako_asset->is_pdu_sync_mode(hako_asset_instance.asset_name_str);
    if (result == true) {
        std::cout << "SYNC MODE: true" << std::endl;
        return false;
    }
    result = hako_asset_instance.hako_asset->is_simulation_mode();
    if (result == false) {
        //std::cout << "NOT SIMULATION MODE" << std::endl;
        return false;
    }
    hako_time_t world_time = hako_asset_instance.hako_asset->get_worldtime();
    if (hako_asset_instance.current_usec >= world_time) {
        return false;
    }
    if (hako_asset_instance.callback != NULL) {
        if (hako_asset_instance.callback->on_simulation_step != NULL) {
            hako_asset_instance.callback->on_simulation_step(nullptr);
        }
    }
    hako_asset_instance.current_usec += hako_asset_instance.delta_usec;
    //std::cout << "# current_usec = " << hako_asset_instance.current_usec << std::endl;
    return true;
}
static void hako_asset_impl_pdus_write_done(void)
{
    for (const Robot& robot : hako_asset_instance.robots) {
        for (const PduWriter& writer : robot.pdu_writers) {
            std::cout << "INFO: hako_asset_impl_pdus_write_done() Robot: " << robot.name << ", PduWriter: " << writer.name << std::endl;
            std::cout << "channel_id: " << writer.channel_id << " pdu_size: " << writer.pdu_size << std::endl;
            bool is_called = false;
            if (is_called == false) {
                char * buffer = (char*) malloc(writer.pdu_size);
                HAKO_ASSET_ASSERT(buffer != NULL);
                memset(buffer, 0, writer.pdu_size);
                auto ret = hako_asset_impl_pdu_write(robot.name.c_str(), writer.channel_id, buffer, writer.pdu_size);
                HAKO_ASSET_ASSERT(ret == true);
                free(buffer);
            }
        }
    }

}
static bool hako_asset_impl_proc(void)
{
    while (hako_asset_impl_execute() == false) {
        HakoSimulationStateType curr = hako_asset_instance.hako_sim->state();
        if (curr != HakoSim_Running) {
            std::cout << "WAIT STOP" << std::endl;
            auto ret = hako_asset_impl_wait_event(HakoSimAssetEvent_Stop);
            HAKO_ASSET_ASSERT(ret == true);
            std::cout << "WAIT RESET" << std::endl;
            ret = hako_asset_impl_wait_event(HakoSimAssetEvent_Reset);
            HAKO_ASSET_ASSERT(ret == true);
            // WAINT FOR RUNNING STATE
            //ret = hako_asset_impl_wait_running();
            //HAKO_ASSET_ASSERT(ret == true);
            return false;
        }
        else if (hako_asset_instance.hako_asset->is_pdu_sync_mode(hako_asset_instance.asset_name_str) == true) {
            hako_asset_impl_pdus_write_done();
        }
    }
    return true;
}

bool hako_asset_impl_step(hako_time_t increment_step)
{
    hako_time_t target_time_usec = hako_asset_instance.current_usec + (increment_step * hako_asset_instance.delta_usec);

    // RUNNING PROC
    //std::cout << "current_usec = " << hako_asset_instance.current_usec << std::endl;
    //std::cout << "target_time_usec = " << target_time_usec << std::endl;
    while (hako_asset_instance.current_usec < target_time_usec) {
        if (hako_asset_impl_proc() == false) {
            return false;
        }
    }
    return true;
}
/***********************
 * pdu io
 ***********************/

bool hako_asset_impl_pdu_read(const char* robo_name, HakoPduChannelIdType lchannel, char* buffer, size_t buffer_len)
{
    bool ret = hako_asset_instance.hako_asset->read_pdu(hako_asset_instance.asset_name_str, robo_name, lchannel, buffer, buffer_len);
    if (ret == false) {
        return false;
    }
    return true;
}

bool hako_asset_impl_pdu_write(const char* robo_name, HakoPduChannelIdType lchannel, const char* buffer, size_t buffer_len)
{
    bool ret = hako_asset_instance.hako_asset->write_pdu(hako_asset_instance.asset_name_str, robo_name, lchannel, buffer, buffer_len);
    if (ret == false) {
        return false;
    }
    hako_asset_instance.hako_asset->notify_write_pdu_done(hako_asset_instance.asset_name_str);
    return true;
}

hako_time_t hako_asset_impl_get_world_time()
{
    return hako_asset_instance.hako_asset->get_worldtime();
}
