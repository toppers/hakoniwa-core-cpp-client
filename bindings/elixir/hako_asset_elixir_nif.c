#include "erl_nif.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "hako_capi.h"

static ERL_NIF_TERM nif_hako_asset_init(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    bool result = hako_asset_init();
    if (result) {
        return enif_make_atom(env, "true");
    } else {
        return enif_make_atom(env, "false");
    }
}

static ERL_NIF_TERM nif_hako_asset_register(ErlNifEnv* env, int argc, const ERL_NIF_TERM argv[]) {
    char name[256];

    if (!enif_get_string(env, argv[0], name, sizeof(name), ERL_NIF_LATIN1)) {
        return enif_make_badarg(env);
    }

    // コールバックを設定しないシンプルなバージョン
    bool result = hako_asset_register_polling(name);

    if (result) {
        return enif_make_atom(env, "true");
    } else {
        return enif_make_atom(env, "false");
    }
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
    if (result) {
        return enif_make_atom(env, "true");
    } else {
        return enif_make_atom(env, "false");
    }
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

// NIF関数のマッピング
static ErlNifFunc nif_funcs[] = {
    {"init_asset", 0, nif_hako_asset_init},
    {"register_asset", 1, nif_hako_asset_register},
    {"get_event", 1, nif_hako_asset_get_event},
    {"unregister_asset", 1, nif_hako_asset_unregister},
    {"notify_simtime", 2, nif_hako_asset_notify_simtime},
    {"get_worldtime", 0, nif_hako_asset_get_worldtime},
    // 他のAPIも同様に追加可能
};

// NIFモジュールの初期化
ERL_NIF_INIT(Elixir.HakoAsset, nif_funcs, NULL, NULL, NULL, NULL)
