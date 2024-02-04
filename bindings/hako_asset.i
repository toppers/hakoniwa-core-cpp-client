%module hako_asset

%{
#include "hako_asset.h"
%}

%include "hako_primitive_types.h"


%typemap(in) hako_asset_callbacks_t *callbacks (void *ptr) {
    $1 = (hako_asset_callbacks_t *)malloc(sizeof(hako_asset_callbacks_t));
    if (!$1) {
        PyErr_NoMemory();
        return NULL;
    }
    $1->on_initialize = NULL;
    $1->on_simulation_step = NULL;
    $1->on_manual_timing_control = NULL;
    $1->on_reset = NULL;
    ptr = $1;
}

%typemap(freearg) hako_asset_callbacks_t *callbacks {
    free($1);
}
%include "hako_asset.h"
