#include "erl_nif.h"
#include <stdio.h>
#include <stdlib.h>
#include "hako_asset.h"
#include "hako_conductor.h"

static ERL_NIF_TERM on_initialize_wrapper(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]);
static ERL_NIF_TERM on_simulation_step_wrapper(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]);
static ERL_NIF_TERM on_manual_timing_control_wrapper(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]);
static ERL_NIF_TERM on_reset_wrapper(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]);


// コールバック関数のラッパー
static int callback_wrapper(ErlNifEnv* env, ERL_NIF_TERM callback, hako_asset_context_t* context) {
    if (!enif_is_identical(callback, enif_make_atom(env, "undefined"))) {
        ERL_NIF_TERM context_term = enif_make_resource(env, context);
        ERL_NIF_TERM result = enif_make_call(env, callback, 1, &context_term);
        int c_result;
        if (!enif_get_int(env, result, &c_result)) {
            return -1;
        }
        return c_result;
    }
    return 0;
}

static ERL_NIF_TERM on_initialize_wrapper(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    hako_asset_context_t* context;
    if (!enif_get_resource(env, argv[0], context_resource_type, (void**)&context)) {
        return enif_make_badarg(env);
    }
    int result = callback_wrapper(env, argv[1], context);
    return enif_make_int(env, result);
}

static ERL_NIF_TERM on_reset_wrapper(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    hako_asset_context_t* context;
    if (!enif_get_resource(env, argv[0], context_resource_type, (void**)&context)) {
        return enif_make_badarg(env);
    }
    int result = callback_wrapper(env, argv[1], context);
    return enif_make_int(env, result);
}

static ERL_NIF_TERM on_simulation_step_wrapper(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    hako_asset_context_t* context;
    if (!enif_get_resource(env, argv[0], context_resource_type, (void**)&context)) {
        return enif_make_badarg(env);
    }
    int result = callback_wrapper(env, argv[1], context);
    return enif_make_int(env, result);
}

static ERL_NIF_TERM on_manual_timing_control_wrapper(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    hako_asset_context_t* context;
    if (!enif_get_resource(env, argv[0], context_resource_type, (void**)&context)) {
        return enif_make_badarg(env);
    }
    int result = callback_wrapper(env, argv[1], context);
    return enif_make_int(env, result);
}

static ERL_NIF_TERM asset_register(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    char asset_name[256];
    char config_path[256];
    int model;
    long delta_usec;

    if (!enif_get_string(env, argv[0], asset_name, sizeof(asset_name), ERL_NIF_LATIN1) ||
        !enif_get_string(env, argv[1], config_path, sizeof(config_path), ERL_NIF_LATIN1) ||
        !enif_get_long(env, argv[3], &delta_usec) ||
        !enif_get_int(env, argv[4], &model)) {
        return enif_make_badarg(env);
    }

    ERL_NIF_TERM callbacks = argv[2];
    if (!enif_is_map(env, callbacks)) {
        return enif_make_badarg(env);
    }

    hako_asset_callbacks_t hako_asset_callback_elixir = {0};

    ERL_NIF_TERM on_initialize_term;
    if (enif_get_map_value(env, callbacks, enif_make_atom(env, "on_initialize"), &on_initialize_term)) {
        hako_asset_callback_elixir.on_initialize = on_initialize_wrapper;
    }

    ERL_NIF_TERM on_reset_term;
    if (enif_get_map_value(env, callbacks, enif_make_atom(env, "on_reset"), &on_reset_term)) {
        hako_asset_callback_elixir.on_reset = on_reset_wrapper;
    }

    ERL_NIF_TERM on_simulation_step_term;
    if (enif_get_map_value(env, callbacks, enif_make_atom(env, "on_simulation_step"), &on_simulation_step_term)) {
        hako_asset_callback_elixir.on_simulation_step = on_simulation_step_wrapper;
    }

    ERL_NIF_TERM on_manual_timing_control_term;
    if (enif_get_map_value(env, callbacks, enif_make_atom(env, "on_manual_timing_control"), &on_manual_timing_control_term)) {
        hako_asset_callback_elixir.on_manual_timing_control = on_manual_timing_control_wrapper;
    }

    int result = hako_asset_register(asset_name, config_path, &hako_asset_callback_elixir, delta_usec, (HakoAssetModelType)model);

    if (result == 0) {
        return enif_make_atom(env, "true");
    } else {
        return enif_make_atom(env, "false");
    }
}

static ERL_NIF_TERM hako_asset_start(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    int result = hako_asset_start();
    if (result == 0) {
        return enif_make_atom(env, "true");
    } else {
        return enif_make_atom(env, "false");
    }
}

static ERL_NIF_TERM hako_asset_simulation_time(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    hako_time_t sim_time = hako_asset_simulation_time();
    return enif_make_long(env, sim_time);
}

static ERL_NIF_TERM hako_asset_usleep(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    hako_time_t sleep_time_usec;
    if (!enif_get_long(env, argv[0], &sleep_time_usec)) {
        return enif_make_badarg(env);
    }
    int result = hako_asset_usleep(sleep_time_usec);
    if (result == 0) {
        return enif_make_atom(env, "true");
    } else {
        return enif_make_atom(env, "false");
    }
}

static ERL_NIF_TERM hako_asset_pdu_read(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    char robo_name[256];
    HakoPduChannelIdType lchannel;
    unsigned long buffer_len;

    if (!enif_get_string(env, argv[0], robo_name, sizeof(robo_name), ERL_NIF_LATIN1) ||
        !enif_get_uint(env, argv[1], &lchannel) ||
        !enif_get_ulong(env, argv[2], &buffer_len)) {
        return enif_make_badarg(env);
    }

    char* buffer = (char*)malloc(buffer_len * sizeof(char));
    if (buffer == NULL) {
        return enif_make_badarg(env);
    }

    int result = hako_asset_pdu_read(robo_name, lchannel, buffer, (size_t)buffer_len);

    if (result != 0) {
        free(buffer);
        return enif_make_atom(env, "error");
    }

    ERL_NIF_TERM py_data = enif_make_binary(env, buffer_len);
    free(buffer);
    return py_data;
}

static ERL_NIF_TERM hako_asset_pdu_write(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    char robo_name[256];
    HakoPduChannelIdType lchannel;
    ErlNifBinary pdu_data;

    if (!enif_get_string(env, argv[0], robo_name, sizeof(robo_name), ERL_NIF_LATIN1) ||
        !enif_get_uint(env, argv[1], &lchannel) ||
        !enif_inspect_binary(env, argv[2], &pdu_data)) {
        return enif_make_badarg(env);
    }

    bool ret = hako_asset_pdu_write(robo_name, lchannel, (char*)pdu_data.data, pdu_data.size);
    if (ret == 0) {
        return enif_make_atom(env, "true");
    } else {
        return enif_make_atom(env, "false");
    }
}

static ERL_NIF_TERM hako_conductor_start(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    hako_time_t delta_usec, max_delay_usec;

    if (!enif_get_long(env, argv[0], &delta_usec) ||
        !enif_get_long(env, argv[1], &max_delay_usec)) {
        return enif_make_badarg(env);
    }

    int result = hako_conductor_start(delta_usec, max_delay_usec);
    if (result == 0) {
        return enif_make_atom(env, "true");
    } else {
        return enif_make_atom(env, "false");
    }
}

static ERL_NIF_TERM hako_conductor_stop(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    hako_conductor_stop();
    return enif_make_atom(env, "ok");
}

static ERL_NIF_TERM init_for_external(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    int result = hako_initialize_for_external();
    if (result == 0) {
        return enif_make_atom(env, "true");
    } else {
        return enif_make_atom(env, "false");
    }
}

static ErlNifFunc nif_funcs[] = {
    {"asset_register", 5, asset_register},
    {"start", 0, hako_asset_start},
    {"simulation_time", 0, hako_asset_simulation_time},
    {"usleep", 1, hako_asset_usleep},
    {"pdu_read", 3, hako_asset_pdu_read},
    {"pdu_write", 3, hako_asset_pdu_write},
    {"conductor_start", 2, hako_conductor_start},
    {"conductor_stop", 0, hako_conductor_stop},
    {"init_for_external", 0, init_for_external}
};

ERL_NIF_INIT(Elixir.HakoAsset, nif_funcs, NULL, NULL, NULL, NULL)
