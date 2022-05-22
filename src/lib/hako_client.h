#ifndef _HAKO_CLIENT_H_
#define _HAKO_CLIENT_H_

namespace hako::client {

#ifdef __cplusplus
extern "C" {
#endif

extern void hakoniwa_core_init(const char* server);
typedef enum {
    Ercd_OK = 0,
    Ercd_NG
} ErcdType;
typedef struct {
    char* param_filename;
} HakoniwaAssetParamInfoType;
typedef struct {
    char * name;
    int len;
} HakoniwaAssetInfoType;
extern ErcdType hakoniwa_core_asset_register(const HakoniwaAssetParamInfoType* asset);
extern ErcdType hakoniwa_core_asset_unregister(void);

extern ErcdType hakoniwa_core_start_simulation(void);
extern ErcdType hakoniwa_core_stop_simulation(void);
extern ErcdType hakoniwa_core_reset_simulation(void);

typedef enum {
    SimStatus_Stopped = 0,
    SimStatus_Runnable,
    SimStatus_Running,
    SimStatus_Stopping,
    SimStatus_Terminated,
    SimStatus_Num,
} SimStatusType;
extern ErcdType hakoniwa_core_get_simstatus(SimStatusType *status);

typedef enum {
    HakoniwaAssetEvent_Start = 0,
    HakoniwaAssetEvent_End,
    HakoniwaAssetEvent_Heartbeat,
    HakoniwaAssetEvent_None,
} HakoniwaAssetEventEnumType;

typedef struct {
    HakoniwaAssetEventEnumType type;
} HakoniwaAssetEventType;
extern ErcdType hakoniwa_core_asset_notification_start(void);
extern HakoniwaAssetEventType hakoniwa_core_asset_get_event(void);
extern ErcdType hakoniwa_core_asset_event_feedback(HakoniwaAssetEventEnumType event, ErcdType ercd);

typedef struct {
    int array_size;
    HakoniwaAssetInfoType *entries;
} HakoniwaAssetInfoArrayType;
extern ErcdType hakonwia_core_get_asset_list(HakoniwaAssetInfoArrayType *list);
extern void hakonwia_core_free_asset_list(HakoniwaAssetInfoArrayType *list);


typedef struct {
    char* tx_ipaddr;
    char* rx_ipaddr;
    int tx_portno;
    int rx_portno;
    int interval_msec; /* msec */
} HakoniwaAssetTimeSyncConfigType;
extern ErcdType hakoniwa_asset_real_time_sync_init(HakoniwaAssetTimeSyncConfigType* cfg);
extern ErcdType hakoniwa_asset_real_time_sync_start(void);
extern ErcdType hakoniwa_asset_real_time_sync_stop(void);

#ifdef __cplusplus
}
#endif

}

#endif /* _HAKO_CLIENT_H_ */