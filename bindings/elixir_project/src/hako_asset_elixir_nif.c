#include "erl_nif.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "hako_capi.h"
extern int hako_conductor_start(hako_time_t delta_usec, hako_time_t max_delay_usec);
extern void hako_conductor_stop(void);

static ERL_NIF_TERM nif_hako_asset_init(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    bool result = hako_asset_init();
    return result ? enif_make_atom(env, "true") : enif_make_atom(env, "false");
}

static ERL_NIF_TERM nif_hako_asset_register(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    ErlNifBinary name_bin;

    if (!enif_inspect_binary(env, argv[0], &name_bin)) {
        printf("error register arguments\n");
        return enif_make_badarg(env);
    }

    char name[256];
    snprintf(name, sizeof(name), "%.*s", (int)name_bin.size, (char *)name_bin.data);

    bool result = hako_asset_register_polling(name);
    printf("register(%s) error: %d\n", name, result);

    return result ? enif_make_atom(env, "true") : enif_make_atom(env, "false");
}

static ERL_NIF_TERM nif_hako_asset_get_event(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    ErlNifBinary name_bin;

    if (!enif_inspect_binary(env, argv[0], &name_bin)) {
        return enif_make_badarg(env);
    }

    char name[256];
    snprintf(name, sizeof(name), "%.*s", (int)name_bin.size, (char *)name_bin.data);

    int event = hako_asset_get_event(name);
    return enif_make_int(env, event);
}

static ERL_NIF_TERM nif_hako_asset_unregister(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    ErlNifBinary name_bin;

    if (!enif_inspect_binary(env, argv[0], &name_bin)) {
        return enif_make_badarg(env);
    }

    char name[256];
    snprintf(name, sizeof(name), "%.*s", (int)name_bin.size, (char *)name_bin.data);

    bool result = hako_asset_unregister(name);
    return result ? enif_make_atom(env, "true") : enif_make_atom(env, "false");
}

static ERL_NIF_TERM nif_hako_asset_notify_simtime(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    ErlNifBinary name_bin;
    ErlNifSInt64 simtime;

    if (!enif_inspect_binary(env, argv[0], &name_bin) || !enif_get_int64(env, argv[1], &simtime)) {
        return enif_make_badarg(env);
    }

    char name[256];
    snprintf(name, sizeof(name), "%.*s", (int)name_bin.size, (char *)name_bin.data);

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
    ErlNifBinary robo_name_bin;
    int channel_id;
    size_t pdu_size;

    if (!enif_inspect_binary(env, argv[0], &robo_name_bin) ||
        !enif_get_int(env, argv[1], &channel_id) ||
        !enif_get_ulong(env, argv[2], &pdu_size)) {
        return enif_make_badarg(env);
    }

    char robo_name[256];
    snprintf(robo_name, sizeof(robo_name), "%.*s", (int)robo_name_bin.size, (char *)robo_name_bin.data);

    bool result = hako_asset_create_pdu_lchannel(robo_name, channel_id, pdu_size);
    return result ? enif_make_atom(env, "true") : enif_make_atom(env, "false");
}

static ERL_NIF_TERM nif_hako_asset_get_pdu_channel(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    ErlNifBinary robo_name_bin;
    int channel_id;

    if (!enif_inspect_binary(env, argv[0], &robo_name_bin) || !enif_get_int(env, argv[1], &channel_id)) {
        return enif_make_badarg(env);
    }

    char robo_name[256];
    snprintf(robo_name, sizeof(robo_name), "%.*s", (int)robo_name_bin.size, (char *)robo_name_bin.data);

    HakoPduChannelIdType result = hako_asset_get_pdu_channel(robo_name, channel_id);
    return enif_make_int(env, result);
}

static ERL_NIF_TERM nif_hako_asset_is_pdu_dirty(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    ErlNifBinary asset_name_bin;
    ErlNifBinary robo_name_bin;
    int channel_id;

    if (!enif_inspect_binary(env, argv[0], &asset_name_bin) ||
        !enif_inspect_binary(env, argv[1], &robo_name_bin) ||
        !enif_get_int(env, argv[2], &channel_id)) {
        return enif_make_badarg(env);
    }

    char asset_name[256];
    char robo_name[256];
    snprintf(asset_name, sizeof(asset_name), "%.*s", (int)asset_name_bin.size, (char *)asset_name_bin.data);
    snprintf(robo_name, sizeof(robo_name), "%.*s", (int)robo_name_bin.size, (char *)robo_name_bin.data);

    bool result = hako_asset_is_pdu_dirty(asset_name, robo_name, channel_id);
    return result ? enif_make_atom(env, "true") : enif_make_atom(env, "false");
}

static ERL_NIF_TERM nif_hako_asset_write_pdu(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    ErlNifBinary asset_name_bin;
    ErlNifBinary robo_name_bin;
    int channel_id;
    ErlNifBinary pdu_data;

    if (!enif_inspect_binary(env, argv[0], &asset_name_bin) ||
        !enif_inspect_binary(env, argv[1], &robo_name_bin) ||
        !enif_get_int(env, argv[2], &channel_id) ||
        !enif_inspect_binary(env, argv[3], &pdu_data)) {
        return enif_make_badarg(env);
    }

    char asset_name[256];
    char robo_name[256];
    snprintf(asset_name, sizeof(asset_name), "%.*s", (int)asset_name_bin.size, (char *)asset_name_bin.data);
    snprintf(robo_name, sizeof(robo_name), "%.*s", (int)robo_name_bin.size, (char *)robo_name_bin.data);

    bool result = hako_asset_write_pdu(asset_name, robo_name, channel_id, (const char *)pdu_data.data, pdu_data.size);
    return result ? enif_make_atom(env, "true") : enif_make_atom(env, "false");
}

static ERL_NIF_TERM nif_hako_asset_write_pdu_nolock(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    ErlNifBinary robo_name_bin;
    int channel_id;
    ErlNifBinary pdu_data;

    if (!enif_inspect_binary(env, argv[0], &robo_name_bin) ||
        !enif_get_int(env, argv[1], &channel_id) ||
        !enif_inspect_binary(env, argv[2], &pdu_data)) {
        return enif_make_badarg(env);
    }

    char robo_name[256];
    snprintf(robo_name, sizeof(robo_name), "%.*s", (int)robo_name_bin.size, (char *)robo_name_bin.data);

    bool result = hako_asset_write_pdu_nolock(robo_name, channel_id, (const char *)pdu_data.data, pdu_data.size);
    return result ? enif_make_atom(env, "true") : enif_make_atom(env, "false");
}

static ERL_NIF_TERM nif_hako_asset_read_pdu(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    ErlNifBinary asset_name_bin;
    ErlNifBinary robo_name_bin;
    int channel_id;
    size_t pdu_size;

    if (!enif_inspect_binary(env, argv[0], &asset_name_bin) ||
        !enif_inspect_binary(env, argv[1], &robo_name_bin) ||
        !enif_get_int(env, argv[2], &channel_id) ||
        !enif_get_ulong(env, argv[3], &pdu_size)) {
        return enif_make_badarg(env);
    }

    char asset_name[256];
    char robo_name[256];
    snprintf(asset_name, sizeof(asset_name), "%.*s", (int)asset_name_bin.size, (char *)asset_name_bin.data);
    snprintf(robo_name, sizeof(robo_name), "%.*s", (int)robo_name_bin.size, (char *)robo_name_bin.data);

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

static ERL_NIF_TERM nif_hako_asset_read_pdu_nolock(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    ErlNifBinary robo_name_bin;
    int channel_id;
    size_t pdu_size;

    if (!enif_inspect_binary(env, argv[0], &robo_name_bin) ||
        !enif_get_int(env, argv[1], &channel_id) ||
        !enif_get_ulong(env, argv[2], &pdu_size)) {
        return enif_make_badarg(env);
    }

    char robo_name[256];
    snprintf(robo_name, sizeof(robo_name), "%.*s", (int)robo_name_bin.size, (char *)robo_name_bin.data);

    char *pdu_data = (char *)enif_alloc(pdu_size);
    if (pdu_data == NULL) {
        return enif_make_atom(env, "error");
    }

    bool result = hako_asset_read_pdu_nolock(robo_name, channel_id, pdu_data, pdu_size);
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
    ErlNifBinary asset_name_bin;
    int isOk;

    if (!enif_inspect_binary(env, argv[0], &asset_name_bin) || !enif_get_int(env, argv[1], &isOk)) {
        return enif_make_badarg(env);
    }

    char asset_name[256];
    snprintf(asset_name, sizeof(asset_name), "%.*s", (int)asset_name_bin.size, (char *)asset_name_bin.data);

    bool result = hako_asset_start_feedback(asset_name, isOk);
    return result ? enif_make_atom(env, "true") : enif_make_atom(env, "false");
}

// Stop Feedback NIF
static ERL_NIF_TERM nif_hako_asset_stop_feedback(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    ErlNifBinary asset_name_bin;
    int isOk;

    if (!enif_inspect_binary(env, argv[0], &asset_name_bin) || !enif_get_int(env, argv[1], &isOk)) {
        return enif_make_badarg(env);
    }

    char asset_name[256];
    snprintf(asset_name, sizeof(asset_name), "%.*s", (int)asset_name_bin.size, (char *)asset_name_bin.data);

    bool result = hako_asset_stop_feedback(asset_name, isOk);
    return result ? enif_make_atom(env, "true") : enif_make_atom(env, "false");
}

// Reset Feedback NIF
static ERL_NIF_TERM nif_hako_asset_reset_feedback(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    ErlNifBinary asset_name_bin;
    int isOk;

    if (!enif_inspect_binary(env, argv[0], &asset_name_bin) || !enif_get_int(env, argv[1], &isOk)) {
        return enif_make_badarg(env);
    }

    char asset_name[256];
    snprintf(asset_name, sizeof(asset_name), "%.*s", (int)asset_name_bin.size, (char *)asset_name_bin.data);

    bool result = hako_asset_reset_feedback(asset_name, isOk);
    return result ? enif_make_atom(env, "true") : enif_make_atom(env, "false");
}

// Notify Read PDU Done NIF
static ERL_NIF_TERM nif_hako_asset_notify_read_pdu_done(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    ErlNifBinary asset_name_bin;

    if (!enif_inspect_binary(env, argv[0], &asset_name_bin)) {
        return enif_make_badarg(env);
    }

    char asset_name[256];
    snprintf(asset_name, sizeof(asset_name), "%.*s", (int)asset_name_bin.size, (char *)asset_name_bin.data);

    hako_asset_notify_read_pdu_done(asset_name);
    return enif_make_atom(env, "ok");
}

// Notify Write PDU Done NIF
static ERL_NIF_TERM nif_hako_asset_notify_write_pdu_done(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    ErlNifBinary asset_name_bin;

    if (!enif_inspect_binary(env, argv[0], &asset_name_bin)) {
        return enif_make_badarg(env);
    }

    char asset_name[256];
    snprintf(asset_name, sizeof(asset_name), "%.*s", (int)asset_name_bin.size, (char *)asset_name_bin.data);

    hako_asset_notify_write_pdu_done(asset_name);
    return enif_make_atom(env, "ok");
}

// Is PDU Sync Mode NIF
static ERL_NIF_TERM nif_hako_asset_is_pdu_sync_mode(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    ErlNifBinary asset_name_bin;

    if (!enif_inspect_binary(env, argv[0], &asset_name_bin)) {
        return enif_make_badarg(env);
    }

    char asset_name[256];
    snprintf(asset_name, sizeof(asset_name), "%.*s", (int)asset_name_bin.size, (char *)asset_name_bin.data);

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

// Conductor Start NIF
static ERL_NIF_TERM nif_hako_conductor_start(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    ErlNifSInt64 delta_usec, max_delay_usec;

    if (!enif_get_int64(env, argv[0], &delta_usec) ||
        !enif_get_int64(env, argv[1], &max_delay_usec)) {
        return enif_make_badarg(env);
    }

    int result = hako_conductor_start(delta_usec, max_delay_usec);
    return result != 0 ? enif_make_atom(env, "true") : enif_make_atom(env, "false");
}

// Conductor Stop NIF
static ERL_NIF_TERM nif_hako_conductor_stop(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    hako_conductor_stop();
    return enif_make_atom(env, "ok");
}
// hako_simevent_init のNIFラッパー
static ERL_NIF_TERM nif_hako_simevent_init(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    bool result = hako_simevent_init();
    return result ? enif_make_atom(env, "true") : enif_make_atom(env, "false");
}

// hako_simevent_get_state のNIFラッパー
static ERL_NIF_TERM nif_hako_simevent_get_state(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    int state = hako_simevent_get_state();
    return enif_make_int(env, state);
}

// hako_simevent_start のNIFラッパー
static ERL_NIF_TERM nif_hako_simevent_start(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    bool result = hako_simevent_start();
    return result ? enif_make_atom(env, "true") : enif_make_atom(env, "false");
}

// hako_simevent_stop のNIFラッパー
static ERL_NIF_TERM nif_hako_simevent_stop(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    bool result = hako_simevent_stop();
    return result ? enif_make_atom(env, "true") : enif_make_atom(env, "false");
}

// hako_simevent_reset のNIFラッパー
static ERL_NIF_TERM nif_hako_simevent_reset(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    bool result = hako_simevent_reset();
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
    {"write_pdu_nolock", 3, nif_hako_asset_write_pdu_nolock},
    {"read_pdu_nolock", 3, nif_hako_asset_read_pdu_nolock},
    {"start_feedback", 2, nif_hako_asset_start_feedback},
    {"stop_feedback", 2, nif_hako_asset_stop_feedback},
    {"reset_feedback", 2, nif_hako_asset_reset_feedback},
    {"notify_read_pdu_done", 1, nif_hako_asset_notify_read_pdu_done},
    {"notify_write_pdu_done", 1, nif_hako_asset_notify_write_pdu_done},
    {"is_pdu_sync_mode", 1, nif_hako_asset_is_pdu_sync_mode},
    {"is_simulation_mode", 0, nif_hako_asset_is_simulation_mode},
    {"is_pdu_created", 0, nif_hako_asset_is_pdu_created},     
    {"simevent_init", 0, nif_hako_simevent_init},
    {"simevent_get_state", 0, nif_hako_simevent_get_state},
    {"simevent_start", 0, nif_hako_simevent_start},
    {"simevent_stop", 0, nif_hako_simevent_stop},
    {"simevent_reset", 0, nif_hako_simevent_reset},
    {"conductor_start", 2, nif_hako_conductor_start},
    {"conductor_stop", 0, nif_hako_conductor_stop}
};

// NIFモジュールの初期化
ERL_NIF_INIT(Elixir.HakoApi, nif_funcs, NULL, NULL, NULL, NULL)
