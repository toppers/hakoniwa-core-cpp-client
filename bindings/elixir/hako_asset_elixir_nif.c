#include "erl_nif.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "hako_capi.h"

static ERL_NIF_TERM nif_hako_asset_init(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    bool result = hako_asset_init();
    return result ? enif_make_atom(env, "true") : enif_make_atom(env, "false");
}

static ERL_NIF_TERM nif_hako_asset_register(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    char name[256];

    if (!enif_get_string(env, argv[0], name, sizeof(name), ERL_NIF_LATIN1)) {
        return enif_make_badarg(env);
    }

    bool result = hako_asset_register_polling(name);
    return result ? enif_make_atom(env, "true") : enif_make_atom(env, "false");
}

static ERL_NIF_TERM nif_hako_asset_get_event(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    char name[256];
    if (!enif_get_string(env, argv[0], name, sizeof(name), ERL_NIF_LATIN1)) {
        return enif_make_badarg(env);
    }

    int event = hako_asset_get_event(name);
    return enif_make_int(env, event);
}

static ERL_NIF_TERM nif_hako_asset_unregister(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    char name[256];
    if (!enif_get_string(env, argv[0], name, sizeof(name), ERL_NIF_LATIN1)) {
        return enif_make_badarg(env);
    }

    bool result = hako_asset_unregister(name);
    return result ? enif_make_atom(env, "true") : enif_make_atom(env, "false");
}

static ERL_NIF_TERM nif_hako_asset_notify_simtime(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    char name[256];
    ErlNifSInt64 simtime;

    if (!enif_get_string(env, argv[0], name, sizeof(name), ERL_NIF_LATIN1) ||
        !enif_get_int64(env, argv[1], &simtime)) {
        return enif_make_badarg(env);
    }

    hako_asset_notify_simtime(name, simtime);
    return enif_make_atom(env, "ok");
}

static ERL_NIF_TERM nif_hako_asset_get_worldtime(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    hako_time_t worldtime = hako_asset_get_worldtime();
    return enif_make_long(env, worldtime);
}

static ERL_NIF_TERM nif_hako_asset_create_pdu_channel(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    int channel_id;
    size_t pdu_size;

    if (!enif_get_int(env, argv[0], &channel_id) || !enif_get_ulong(env, argv[1], &pdu_size)) {
        return enif_make_badarg(env);
    }

    bool result = hako_asset_create_pdu_channel(channel_id, pdu_size);
    return result ? enif_make_atom(env, "true") : enif_make_atom(env, "false");
}

static ERL_NIF_TERM nif_hako_asset_create_pdu_lchannel(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    char robo_name[256];
    int channel_id;
    size_t pdu_size;

    if (!enif_get_string(env, argv[0], robo_name, sizeof(robo_name), ERL_NIF_LATIN1) ||
        !enif_get_int(env, argv[1], &channel_id) ||
        !enif_get_ulong(env, argv[2], &pdu_size)) {
        return enif_make_badarg(env);
    }

    bool result = hako_asset_create_pdu_lchannel(robo_name, channel_id, pdu_size);
    return result ? enif_make_atom(env, "true") : enif_make_atom(env, "false");
}

static ERL_NIF_TERM nif_hako_asset_get_pdu_channel(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    char robo_name[256];
    int channel_id;

    if (!enif_get_string(env, argv[0], robo_name, sizeof(robo_name), ERL_NIF_LATIN1) ||
        !enif_get_int(env, argv[1], &channel_id)) {
        return enif_make_badarg(env);
    }

    HakoPduChannelIdType result = hako_asset_get_pdu_channel(robo_name, channel_id);
    return enif_make_int(env, result);
}

static ERL_NIF_TERM nif_hako_asset_is_pdu_dirty(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    char asset_name[256];
    char robo_name[256];
    int channel_id;

    if (!enif_get_string(env, argv[0], asset_name, sizeof(asset_name), ERL_NIF_LATIN1) ||
        !enif_get_string(env, argv[1], robo_name, sizeof(robo_name), ERL_NIF_LATIN1) ||
        !enif_get_int(env, argv[2], &channel_id)) {
        return enif_make_badarg(env);
    }

    bool result = hako_asset_is_pdu_dirty(asset_name, robo_name, channel_id);
    return result ? enif_make_atom(env, "true") : enif_make_atom(env, "false");
}

static ERL_NIF_TERM nif_hako_asset_write_pdu(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    char asset_name[256];
    char robo_name[256];
    int channel_id;
    ErlNifBinary pdu_data;

    if (!enif_get_string(env, argv[0], asset_name, sizeof(asset_name), ERL_NIF_LATIN1) ||
        !enif_get_string(env, argv[1], robo_name, sizeof(robo_name), ERL_NIF_LATIN1) ||
        !enif_get_int(env, argv[2], &channel_id) ||
        !enif_inspect_binary(env, argv[3], &pdu_data)) {
        return enif_make_badarg(env);
    }

    bool result = hako_asset_write_pdu(asset_name, robo_name, channel_id, (const char *)pdu_data.data, pdu_data.size);
    return result ? enif_make_atom(env, "true") : enif_make_atom(env, "false");
}

static ERL_NIF_TERM nif_hako_asset_read_pdu(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    char asset_name[256];
    char robo_name[256];
    int channel_id;
    size_t pdu_size;

    if (!enif_get_string(env, argv[0], asset_name, sizeof(asset_name), ERL_NIF_LATIN1) ||
        !enif_get_string(env, argv[1], robo_name, sizeof(robo_name), ERL_NIF_LATIN1) ||
        !enif_get_int(env, argv[2], &channel_id) ||
        !enif_get_ulong(env, argv[3], &pdu_size)) {
        return enif_make_badarg(env);
    }

    char *pdu_data = (char *)enif_alloc(pdu_size);
    if (pdu_data == NULL) {
        return enif_make_atom(env, "error");
    }

    bool result = hako_asset_read_pdu(asset_name, robo_name, channel_id, pdu_data, pdu_size);
    ERL_NIF_TERM pdu_binary;

    if (result) {
        ErlNifBinary binary;
        if (!enif_alloc_binary(pdu_size, &binary)) {
            enif_free(pdu_data);
            return enif_make_atom(env, "error");
        }
        memcpy(binary.data, pdu_data, pdu_size);
        pdu_binary = enif_make_binary(env, &binary);
    }

    enif_free(pdu_data);
    return result ? pdu_binary : enif_make_atom(env, "error");
}

// Start Feedback NIF
static ERL_NIF_TERM nif_hako_asset_start_feedback(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    char asset_name[256];
    int isOk;

    if (!enif_get_string(env, argv[0], asset_name, sizeof(asset_name), ERL_NIF_LATIN1) ||
        !enif_get_int(env, argv[1], &isOk)) {
        return enif_make_badarg(env);
    }

    bool result = hako_asset_start_feedback(asset_name, isOk);
    return result ? enif_make_atom(env, "true") : enif_make_atom(env, "false");
}

// Stop Feedback NIF
static ERL_NIF_TERM nif_hako_asset_stop_feedback(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    char asset_name[256];
    int isOk;

    if (!enif_get_string(env, argv[0], asset_name, sizeof(asset_name), ERL_NIF_LATIN1) ||
        !enif_get_int(env, argv[1], &isOk)) {
        return enif_make_badarg(env);
    }

    bool result = hako_asset_stop_feedback(asset_name, isOk);
    return result ? enif_make_atom(env, "true") : enif_make_atom(env, "false");
}

// Reset Feedback NIF
static ERL_NIF_TERM nif_hako_asset_reset_feedback(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    char asset_name[256];
    int isOk;

    if (!enif_get_string(env, argv[0], asset_name, sizeof(asset_name), ERL_NIF_LATIN1) ||
        !enif_get_int(env, argv[1], &isOk)) {
        return enif_make_badarg(env);
    }

    bool result = hako_asset_reset_feedback(asset_name, isOk);
    return result ? enif_make_atom(env, "true") : enif_make_atom(env, "false");
}
#include "erl_nif.h"
#include <stdbool.h>
#include <string.h>
#include "hako_capi.h"

// Notify Read PDU Done NIF
static ERL_NIF_TERM nif_hako_asset_notify_read_pdu_done(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    char asset_name[256];

    if (!enif_get_string(env, argv[0], asset_name, sizeof(asset_name), ERL_NIF_LATIN1)) {
        return enif_make_badarg(env);
    }

    hako_asset_notify_read_pdu_done(asset_name);
    return enif_make_atom(env, "ok");
}

// Notify Write PDU Done NIF
static ERL_NIF_TERM nif_hako_asset_notify_write_pdu_done(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    char asset_name[256];

    if (!enif_get_string(env, argv[0], asset_name, sizeof(asset_name), ERL_NIF_LATIN1)) {
        return enif_make_badarg(env);
    }

    hako_asset_notify_write_pdu_done(asset_name);
    return enif_make_atom(env, "ok");
}

// Is PDU Sync Mode NIF
static ERL_NIF_TERM nif_hako_asset_is_pdu_sync_mode(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    char asset_name[256];

    if (!enif_get_string(env, argv[0], asset_name, sizeof(asset_name), ERL_NIF_LATIN1)) {
        return enif_make_badarg(env);
    }

    bool result = hako_asset_is_pdu_sync_mode(asset_name);
    return result ? enif_make_atom(env, "true") : enif_make_atom(env, "false");
}

// Is Simulation Mode NIF
static ERL_NIF_TERM nif_hako_asset_is_simulation_mode(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    bool result = hako_asset_is_simulation_mode();
    return result ? enif_make_atom(env, "true") : enif_make_atom(env, "false");
}

// Is PDU Created NIF
static ERL_NIF_TERM nif_hako_asset_is_pdu_created(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    bool result = hako_asset_is_pdu_created();
    return result ? enif_make_atom(env, "true") : enif_make_atom(env, "false");
}

// NIF関数のマッピング
static ErlNifFunc nif_funcs[] = {
    {"init_asset", 0, nif_hako_asset_init},
    {"register_asset", 1, nif_hako_asset_register},
    {"get_event", 1, nif_hako_asset_get_event},
    {"unregister_asset", 1, nif_hako_asset_unregister},
    {"notify_simtime", 2, nif_hako_asset_notify_simtime},
    {"get_worldtime", 0, nif_hako_asset_get_worldtime},
    {"create_pdu_channel", 2, nif_hako_asset_create_pdu_channel},
    {"create_pdu_lchannel", 3, nif_hako_asset_create_pdu_lchannel},
    {"get_pdu_channel", 2, nif_hako_asset_get_pdu_channel},
    {"is_pdu_dirty", 3, nif_hako_asset_is_pdu_dirty},
    {"write_pdu", 4, nif_hako_asset_write_pdu},
    {"read_pdu", 4, nif_hako_asset_read_pdu},
    {"start_feedback", 2, nif_hako_asset_start_feedback},
    {"stop_feedback", 2, nif_hako_asset_stop_feedback},
    {"reset_feedback", 2, nif_hako_asset_reset_feedback},
    {"notify_read_pdu_done", 1, nif_hako_asset_notify_read_pdu_done},
    {"notify_write_pdu_done", 1, nif_hako_asset_notify_write_pdu_done},
    {"is_pdu_sync_mode", 1, nif_hako_asset_is_pdu_sync_mode},
    {"is_simulation_mode", 0, nif_hako_asset_is_simulation_mode},
    {"is_pdu_created", 0, nif_hako_asset_is_pdu_created},     
};

// NIFモジュールの初期化
ERL_NIF_INIT(Elixir.HakoAsset, nif_funcs, NULL, NULL, NULL, NULL)
