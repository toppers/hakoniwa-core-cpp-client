#ifndef _HAKO_CAPI_TYPES_H_
#define _HAKO_CAPI_TYPES_H_

typedef long long int hako_time_t;
typedef struct {
    void (*start) ();
    void (*stop) ();
    void (*reset) ();
} hako_asset_callback_t;


#endif /* _HAKO_CAPI_TYPES_H_ */