#include "erl_nif.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "hako_asset.h"
#include "hako_conductor.h"

ErlNifEnv* nif_env = NULL;
ErlNifPid on_initialize_pid;
ErlNifPid on_simulation_step_pid;
ErlNifPid on_manual_timing_control_pid;
ErlNifPid on_reset_pid;
ERL_NIF_TERM on_initialize_callback;
ERL_NIF_TERM on_simulation_step_callback;
ERL_NIF_TERM on_manual_timing_control_callback;
ERL_NIF_TERM on_reset_callback;

static int callback_wrapper(ErlNifPid pid, ERL_NIF_TERM callback, hako_asset_context_t* context) {
    if (!enif_is_identical(callback, enif_make_atom(nif_env, "undefined"))) {
        ErlNifEnv* msg_env = enif_alloc_env();
        ERL_NIF_TERM context_term = enif_make_resource(msg_env, context);
        ERL_NIF_TERM msg = enif_make_tuple2(msg_env, callback, context_term);

        if (enif_send(NULL, &pid, msg_env, msg)) {
            enif_free_env(msg_env);
            return 0; // success
        } else {
            enif_free_env(msg_env);
            return -1; // send failed
        }
    }
    return 0; // callback is undefined, treat as success
}

static int on_initialize_wrapper(hako_asset_context_t* context) {
    return callback_wrapper(on_initialize_pid, on_initialize_callback, context);
}

static int on_reset_wrapper(hako_asset_context_t* context) {
    return callback_wrapper(on_reset_pid, on_reset_callback, context);
}

static int on_simulation_step_wrapper(hako_asset_context_t* context) {
    return callback_wrapper(on_simulation_step_pid, on_simulation_step_callback, context);
}

static int on_manual_timing_control_wrapper(hako_asset_context_t* context) {
    return callback_wrapper(on_manual_timing_control_pid, on_manual_timing_control_callback, context);
}

static ERL_NIF_TERM nif_asset_register(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    char asset_name[256];
    char config_path[256];
    long delta_usec;
    int model;

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

    enif_get_map_value(env, callbacks, enif_make_atom(env, "on_initialize"), &on_initialize_callback);
    enif_self(env, &on_initialize_pid);
    hako_asset_callback_elixir.on_initialize = on_initialize_wrapper;

    enif_get_map_value(env, callbacks, enif_make_atom(env, "on_reset"), &on_reset_callback);
    enif_self(env, &on_reset_pid);
    hako_asset_callback_elixir.on_reset = on_reset_wrapper;

    ERL_NIF_TERM temp;
    enif_get_map_value(env, callbacks, enif_make_atom(env, "on_simulation_step"), &temp);
    if (enif_is_identical(temp, enif_make_atom(env, "undefined"))) {
        hako_asset_callback_elixir.on_simulation_step = NULL;
    } else {
        on_simulation_step_callback = temp;
        enif_self(env, &on_simulation_step_pid);
        hako_asset_callback_elixir.on_simulation_step = on_simulation_step_wrapper;
    }

    enif_get_map_value(env, callbacks, enif_make_atom(env, "on_manual_timing_control"), &temp);
    if (enif_is_identical(temp, enif_make_atom(env, "undefined"))) {
        hako_asset_callback_elixir.on_manual_timing_control = NULL;
    } else {
        on_manual_timing_control_callback = temp;
        enif_self(env, &on_manual_timing_control_pid);
        hako_asset_callback_elixir.on_manual_timing_control = on_manual_timing_control_wrapper;
    }

    int result = hako_asset_register(asset_name, config_path, &hako_asset_callback_elixir, (hako_time_t)delta_usec, (HakoAssetModelType)model);

    return result == 0 ? enif_make_atom(env, "true") : enif_make_atom(env, "false");
}


static ERL_NIF_TERM nif_hako_asset_start(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    int result = hako_asset_start();
    return result == 0 ? enif_make_atom(env, "true") : enif_make_atom(env, "false");
}

static ERL_NIF_TERM nif_hako_asset_simulation_time(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    ErlNifSInt64 sim_time = hako_asset_simulation_time();
    return enif_make_long(env, sim_time);
}

static ERL_NIF_TERM nif_hako_asset_usleep(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    ErlNifSInt64 sleep_time_usec;
    if (!enif_get_long(env, argv[0], &sleep_time_usec)) {
        return enif_make_badarg(env);
    }

    int result = hako_asset_usleep(sleep_time_usec);
    return result == 0 ? enif_make_atom(env, "true") : enif_make_atom(env, "false");
}

static ERL_NIF_TERM nif_hako_asset_pdu_read(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    char robo_name[256];
    int channel_id;
    size_t buffer_len;

    if (!enif_get_string(env, argv[0], robo_name, sizeof(robo_name), ERL_NIF_LATIN1) ||
        !enif_get_int(env, argv[1], &channel_id) ||
        !enif_get_ulong(env, argv[2], &buffer_len)) {
        return enif_make_badarg(env);
    }

    char* buffer = (char*)enif_alloc(buffer_len);
    if (buffer == NULL) {
        return enif_make_badarg(env);
    }

    int result = hako_asset_pdu_read(robo_name, channel_id, buffer, buffer_len);

    if (result != 0) {
        enif_free(buffer);
        return enif_make_badarg(env);
    }

    ERL_NIF_TERM bin;
    unsigned char* bin_data = enif_make_new_binary(env, buffer_len, &bin);
    memcpy(bin_data, buffer, buffer_len);
    enif_free(buffer);

    return bin;
}

static ERL_NIF_TERM nif_hako_asset_pdu_write(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    char robo_name[256];
    int channel_id;
    ErlNifBinary pdu_data;

    if (!enif_get_string(env, argv[0], robo_name, sizeof(robo_name), ERL_NIF_LATIN1) ||
        !enif_get_int(env, argv[1], &channel_id) ||
        !enif_inspect_binary(env, argv[2], &pdu_data)) {
        return enif_make_badarg(env);
    }

    bool ret = hako_asset_pdu_write(robo_name, channel_id, (char*)pdu_data.data, pdu_data.size);
    return ret ? enif_make_atom(env, "true") : enif_make_atom(env, "false");
}

static ERL_NIF_TERM nif_hako_conductor_start(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    ErlNifSInt64 delta_usec, max_delay_usec;

    if (!enif_get_long(env, argv[0], &delta_usec) ||
        !enif_get_long(env, argv[1], &max_delay_usec)) {
        return enif_make_badarg(env);
    }

    int result = hako_conductor_start(delta_usec, max_delay_usec);
    return result == 0 ? enif_make_atom(env, "true") : enif_make_atom(env, "false");
}

static ERL_NIF_TERM nif_hako_conductor_stop(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    hako_conductor_stop();
    return enif_make_atom(env, "ok");
}

static ERL_NIF_TERM nif_init_for_external(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    int result = hako_initialize_for_external();
    return result == 0 ? enif_make_atom(env, "true") : enif_make_atom(env, "false");
}

// NIF 関数のマッピング
static ErlNifFunc nif_funcs[] = {
    {"asset_register", 5, nif_asset_register},
    {"init_for_external", 0, nif_init_for_external},
    {"start", 0, nif_hako_asset_start},
    {"simulation_time", 0, nif_hako_asset_simulation_time},
    {"usleep", 1, nif_hako_asset_usleep},
    {"pdu_read", 3, nif_hako_asset_pdu_read},
    {"pdu_write", 3, nif_hako_asset_pdu_write},
    {"conductor_start", 2, nif_hako_conductor_start},
    {"conductor_stop", 0, nif_hako_conductor_stop}
};

// NIF モジュールの初期化
ERL_NIF_INIT(Elixir.HakoAsset, nif_funcs, NULL, NULL, NULL, NULL)
