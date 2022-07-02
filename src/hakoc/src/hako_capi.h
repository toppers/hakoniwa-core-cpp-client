#ifndef _HAKO_CLIENT_H_
#define _HAKO_CLIENT_H_


#ifdef __cplusplus
extern "C" {
#endif

#include "hako_capi_types.h"

/*
 * for master
 */
extern bool hako_master_init();
extern bool hako_master_execute();
extern void hako_master_set_config_simtime(hako_time_t max_delay_time_usec, hako_time_t delta_time_usec);
extern hako_time_t hako_master_get_max_deltay_time_usec();
extern hako_time_t hako_master_get_delta_time_usec();

/*
 * for asset
 */
extern bool hako_asset_init();
extern bool hako_asset_register(const char* name, hako_asset_callback_t *callbacks);

extern bool hako_asset_register_polling(const char* name);
extern int  hako_asset_get_event(const char* name);

extern bool hako_asset_unregister(const char* name);
extern void hako_asset_notify_simtime(const char* name, hako_time_t simtime);
extern hako_time_t hako_asset_get_worldtime();

extern bool hako_asset_start_feedback(const char* asset_name, bool isOk);
extern bool hako_asset_stop_feedback(const char* asset_name, bool isOk);
extern bool hako_asset_reset_feedback(const char* asset_name, bool isOk);


extern bool hako_asset_create_pdu_channel(HakoPduChannelIdType channel_id, size_t pdu_size);
extern bool hako_asset_is_pdu_dirty(HakoPduChannelIdType channel_id);
extern bool hako_asset_write_pdu(const char* asset_name, HakoPduChannelIdType channel_id, const char *pdu_data, size_t len);
extern bool hako_asset_read_pdu(const char* asset_name, HakoPduChannelIdType channel_id, char *pdu_data, size_t len);
extern void hako_asset_notify_read_pdu_done(const char* asset_name);
extern void hako_asset_notify_write_pdu_done(const char* asset_name);
extern bool hako_asset_is_pdu_sync_mode(const char* asset_name);
extern bool hako_asset_is_simulation_mode();
extern bool hako_asset_is_pdu_created();


/*
 * for simevent
 */
extern bool hako_simevent_init();
extern int hako_simevent_get_state();
extern bool hako_simevent_start();
extern bool hako_simevent_stop();
extern bool hako_simevent_reset();

#ifdef __cplusplus
}
#endif

#endif /* _HAKO_CLIENT_H_ */