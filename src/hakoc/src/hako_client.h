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

#ifdef __cplusplus
}
#endif

#endif /* _HAKO_CLIENT_H_ */