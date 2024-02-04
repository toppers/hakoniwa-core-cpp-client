%module hako_asset

%{
#include "hako_asset.h"
%}

%include "hako_primitive_types.h"

%typemap(in) hako_asset_callbacks_t *callbacks (void *ptr) {
    $1 = (hako_asset_callbacks_t *)malloc(sizeof(hako_asset_callbacks_t));
    if (!$1) {
        SWIG_fail;
    }
    $1->on_initialize = NULL;
    $1->on_simulation_step = NULL;
    $1->on_manual_timing_control = NULL;
    $1->on_reset = NULL;
    ptr = $1;
}

%typemap(argout) hako_asset_callbacks_t *callbacks {
    $result = SWIG_NewPointerObj($1, $1_descriptor, SWIG_POINTER_OWN);
}

%typemap(freearg) hako_asset_callbacks_t *callbacks {
    if ($1) {
        free($1);
    }
}

%apply hako_asset_callbacks_t *callbacks { hako_asset_callbacks_t *callbacks };

%include "hako_asset.h"
