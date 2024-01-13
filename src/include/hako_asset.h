#ifndef _HAKO_ASSET_H_
#define _HAKO_ASSET_H_

#include "hako_primitive_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct hako_asset_context_s hako_asset_context_t;
typedef struct hako_asset_callbacks_s hako_asset_callbacks_t;

struct hako_asset_callbacks_s {
    int (*on_initialize)(hako_asset_context_t*);
    int (*on_simulation_step)(hako_asset_context_t*);
    int (*on_manual_timing_control)(hako_asset_context_t*);
    int (*on_reset)(hako_asset_context_t*);
};
#define HAKO_ASSET_MIN_DELTA_TIME_USEC  1000
extern int hako_asset_register(const char *asset_name, const char *config_path, hako_asset_callbacks_t *callbacks, hako_time_t delta_usec);
extern int hako_asset_start(void);
extern int hako_asset_pdu_read(const char *robo_name, HakoPduChannelIdType lchannel, char *buffer, size_t buffer_len);
extern int hako_asset_pdu_write(const char *robo_name, HakoPduChannelIdType lchannel, const char *buffer, size_t buffer_len);
extern hako_time_t hako_asset_simulation_time(void);
extern int hako_asset_usleep(hako_time_t sleep_time_usec);
extern int hako_master_start(hako_time_t delta_usec, hako_time_t max_delay_usec);
extern void hako_master_stop(void);

#ifdef __cplusplus
}
#endif

#endif /* _HAKO_ASSET_H_ */
