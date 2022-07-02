#ifndef _HAKO_CLIENT_H_
#define _HAKO_CLIENT_H_


#ifdef __cplusplus
extern "C" {
#endif

#include "hako_capi_types.h"

/*
 * for hakoniwa proxy
 */
extern int hako_client_init(const char* asset_name);
extern hako_time_t hako_client_get_worldtime();
extern void hako_client_notify_simtime(hako_time_t simtime);

extern int hako_client_create_pdu_channel(HakoPduChannelIdType channel_id, size_t pdu_size);
extern int hako_client_pdu_is_dirty(HakoPduChannelIdType channel_id);
extern int hako_client_write_pdu(const char* asset_name, HakoPduChannelIdType channel_id, const char *pdu_data, size_t len);
extern int hako_client_read_pdu(const char* asset_name, HakoPduChannelIdType channel_id, char *pdu_data, size_t len);
extern void hako_client_notify_read_pdu_done(const char* asset_name);
extern void hako_client_notify_write_pdu_done(const char* asset_name);
extern int hako_client_is_pdu_sync_mode(const char* asset_name);
extern int hako_client_is_simulation_mode();
extern int hako_client_is_pdu_created();


#ifdef __cplusplus
}
#endif

#endif /* _HAKO_CLIENT_H_ */