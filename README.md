[![Build](https://github.com/toppers/hakoniwa-core-cpp-client/actions/workflows/build.yml/badge.svg)](https://github.com/toppers/hakoniwa-core-cpp-client/actions/workflows/build.yml)

このリポジトリは、C/C++で開発された箱庭コア機能([hakoniwa-core-cpp](https://github.com/toppers/hakoniwa-core-cpp/tree/main)) を、C言語の API として提供します。

APIのカテゴリとしては、その目的の応じて以下の３ケースあります。

1. [箱庭アセット](https://github.com/toppers/hakoniwa-document/blob/main/architecture/README.md#%E7%AE%B1%E5%BA%AD%E3%82%A2%E3%82%BB%E3%83%83%E3%83%88)
2. 箱庭コマンド
3. [箱庭コンダクタ](https://github.com/toppers/hakoniwa-conductor)

![スクリーンショット 2024-01-04 14 04 47](https://github.com/toppers/hakoniwa-core-cpp-client/assets/164193/80d2d524-c115-486a-8b99-5da2b83a6fa8)

# 目次
- [箱庭アセット API](#箱庭アセット-api)
  - [箱庭アセットのユースケース分類](#箱庭アセットのユースケース分類)
  - [箱庭アセットの処理フロー](#箱庭アセットの処理フロー)
  - [箱庭 API 仕様](#箱庭-api-仕様)
    - [箱庭アセットを登録する](#箱庭アセットを登録する)
    - [箱庭アセットのシミュレーションを開始する](#箱庭アセットのシミュレーションを開始する)
    - [箱庭アセットの処理を実行する](#箱庭アセットの処理を実行する)
      - [箱庭アセットの初期化処理](#箱庭アセットの初期化処理)
      - [箱庭アセットの周期イベント実行処理](#箱庭アセットの周期イベント実行処理)
      - [箱庭アセットの手動タイミング制御処理](#箱庭アセットの手動タイミング制御処理)
      - [箱庭アセットのリセット処理](#箱庭アセットのリセット処理)
    - [箱庭PDUデータを読み込みする](#箱庭pduデータを読み込みする)
    - [箱庭PDUデータを書き込みする](#箱庭pduデータを書き込みする)
    - [箱庭シミュレーション時間を取得する](#箱庭シミュレーション時間を取得する)
    - [スリープする](#スリープする)
  - [サンプルコード](#サンプルコード)
- [インストール手順](#インストール手順)
- [Windows向けインストール手順](#Windows向けインストール手順)
- [MMAPファイルの設定](#MMAPファイルの設定)
- [箱庭コマンド API](#箱庭コマンド-api)
- [箱庭コンダクタ API](#箱庭コンダクタ-api)
- [箱庭アセットコンフィグ](#箱庭アセットコンフィグ)


# 箱庭アセット API

## 箱庭アセットのユースケース分類

箱庭アセットのユースケースは、大別して以下の３つのカテゴリがあります。

![image](https://github.com/toppers/hakoniwa-core-cpp-client/assets/164193/7b10d4a4-f0cb-4175-80c5-901c9359bd31)

* **アプリケーション固有のユースケース** は、シミュレーションしたい処理内容そのものであり、**箱庭ユーザが作成する機能**です。例えば、ロボットのシミュレーションの場合は、ロボットの制御プログラム等です。

* **アプリケーション共通のユースケース** は、シミュレーション処理において共通する**箱庭の機能セット**です。例えば、他アプリケーションとの通信等があります。

* **シミュレーション向けユースケース** は、箱庭というシミュレーション環境を利用するための**箱庭の機能セット**です。例えば、箱庭アセットの登録等があります。

## 箱庭アセットの処理フロー

箱庭アセットのシミュレーションの処理フローは以下のとおりです。

1. [箱庭アセットを登録する](#箱庭アセットを登録する) (シミュレーション向け)
2. [箱庭アセットのシミュレーションを開始する](#箱庭アセットのシミュレーションを開始する) (シミュレーション向け)
3. [箱箱庭アセットの処理を実行する](#箱庭アセットの処理を実行する) (アプリケーション固有)
4. シミュレーション終了まで 3 を繰り返す

**アプリケーション固有の処理** は、`3` の処理であり、シミュレーションの必要なタイミングで、箱庭コア機能からコールバックされます。
箱庭アセット開発者は、このコールバック関数を実装することで、箱庭アセットとしてシミュレーションを実行できます。

ここで、コールバック処理としては、以下の４種類あります。

* [箱庭アセットの初期化処理](#箱庭アセットの初期化処理)
  * 箱庭のシミュレーション開始時点で１回だけ呼び出されます。
* [箱庭アセットの周期イベント実行処理](#箱庭アセットの周期イベント実行処理)
  * 箱庭のシミュレーション時間ステップ毎に呼び出されます。
* [箱庭アセットの手動タイミング制御処理](#箱庭アセットの手動タイミング制御処理)
  * 箱庭のシミュレーションが開始されたときに一度だけ呼び出され、ユーザーは独自のタイミングで処理を制御できます。
  * 注意：この制御方式を利用する場合は、箱庭アセットの周期イベント実行処理は利用できません。
* [箱庭アセットのリセット処理](#箱庭アセットのリセット処理)
  * 箱庭のシミュレーションがリセットされたタイミングで呼び出されます。

また、アプリケーション固有の処理実装では、以下の **アプリケーション共通の機能** を利用できます。

* [箱庭PDUデータを読み込みする](#箱庭PDUデータを読み込みする)
* [箱庭PDUデータを書き込みする](#箱庭PDUデータを書き込みする)
* [箱庭シミュレーション時間を取得する](#箱庭シミュレーション時間を取得する)
* [スリープする](#スリープする)
  * 注意：[箱庭アセットの手動タイミング制御処理](#箱庭アセットの手動タイミング制御処理)でしか利用できません。

<img width="936" alt="スクリーンショット 2024-01-11 7 55 07" src="https://github.com/toppers/hakoniwa-core-cpp-client/assets/164193/3e29cacb-300b-4c97-a205-578b480bd29f">

## 箱庭 API 仕様

### 箱庭アセットを登録する

**ユースケースID**: `UC-HAKO-ASSET-SIM-1`

**関数名**: `hako_asset_register`

**目的**:  
箱庭シミュレーション環境内で使用されるアセットを登録し、コールバック関数を関連付けます。

**引数**:  
- `asset_name`: 登録するアセットの名前。アセットを一意に識別するために使用されます。型: `const char*`
- `config_path`: [箱庭アセットコンフィグ](#箱庭アセットコンフィグ)ファイルへのパス。型: `const char*`
- `callback`: アセットのイベント処理を行うコールバック関数へのポインタ。このコールバックは、アセットの初期化、シミュレーションステップ、リセットなどのイベントが発生したときに呼び出されます。型: `hako_asset_callbacks_t*`
- `delta_usec`: シミュレーション時間のタイムステップをマイクロ秒単位で指定します。この値はシミュレーションが更新される頻度を決定します。型: `hako_time_t`
- `model`: 箱庭アセットが制御プログラムの場合は `HAKO_ASSET_MODEL_CONTROLLER`。プラントモデルの場合は `HAKO_ASSET_MODEL_PLANT`。 型: `HakoAssetModelType`

**戻り値**:  
成功時は `0` を返します。失敗時は非 `0` のエラーコードを返します。

**エラーハンドリング**:  
- `asset_name` が NULL または空文字列の場合、`EINVAL`（不正な引数）エラーコードを返します。
- `config_path` が不正なパスであるか、指定された設定ファイルが存在しない場合、`ENOENT`（ファイルが存在しない）エラーコードを返します。
- `callback` が NULL の場合、`EINVAL` エラーコードを返します。
- `callback` に、[箱庭アセットの周期イベント実行処理](#箱庭アセットの周期イベント実行処理)と[箱庭アセットの手動タイミング制御処理](#箱庭アセットの手動タイミング制御処理)が両方設定されている場合は、`EINVAL` エラーコードを返します。
- `delta_usec` が `HAKO_ASSET_MIN_DELTA_TIME_USEC` より小さい場合は、`EINVAL`エラーコードを返します。
- その他のエラー発生時は、`EIO` を返します。

**使用例**:

```c
#include <errno.h>
#include "hako_asset.h"

// コールバック関数の実装
hako_asset_callbacks_t callbacks = {
    .on_initialize = my_initialize_callback,
    .on_simulation_step = my_simulation_step_callback,
    .on_reset = my_reset_callback
};

int main() {
    const char* asset_name = "my_asset";
    const char* config_path = "/path/to/config.json";

    // コールバック関数と時間ステップを指定してアセットを登録
    int result = hako_asset_register(asset_name, config_path, &callbacks, 1000000, HAKO_ASSET_MODEL_CONTROLLER); // 1秒ごとに更新

    if (result != 0) {
        // エラーハンドリング
        printf("Error: %d\n", result);
    }

    // シミュレーションの実行など、他の処理を続ける
    return 0;
}
```


### 箱庭アセットのシミュレーションを開始する 

**ユースケースID**: `UC-HAKO-ASSET-SIM-2`

**関数名**: `hako_asset_start`

**目的**:  
登録済みの箱庭アセットのシミュレーションを開始するように依頼します。
本関数を呼び出すと、シミュレーション開始イベントが発生するまでスリープ状態になります。シミュレーション開始イベントの発生により、`箱庭アセットの初期化処理`が呼び出された後、以下のいずれかのコールバック処理が呼び出されます。

* [箱庭アセットの周期イベント実行処理](#箱庭アセットの周期イベント実行処理)
  * 箱庭のシミュレーション時間ステップ毎に呼び出されます。
* [箱庭アセットの手動タイミング制御処理](#箱庭アセットの手動タイミング制御処理)
  * 箱庭のシミュレーションが開始されたときに一度だけ呼び出され、ユーザーは独自のタイミングで処理を制御できます。
  * 注意：この制御方式を利用する場合は、箱庭アセットの周期イベント実行処理は利用できません。


シミュレーション実行中にリセットイベントが発生した場合、`EINTR` エラーコードを返して処理を復帰します。復帰する前に、`箱庭アセットのリセット処理`が呼び出されます。

リセットイベントが発生後に、本関数を再度呼び出すことにより、シミュレーション開始タイミングで再実行することができます。


**引数**:  
なし

**戻り値**:  
成功時は `0` を返します。失敗時は非 `0` のエラーコードを返します。

**エラーハンドリング**:  
- 箱庭の状態が不正な場合は、`EINVAL`エラーコードを返します。
- 箱庭のシミュレーション開始のための待機が適切に行われなかった場合は、`EIO`エラーコードを返します。
- `on_manual_timing_control` が登録されている場合は、on_manual_timing_control()の戻り値が本APIの戻り値になります。
- `on_simulation_step` が登録されている場合は、リセットイベント発生時に `EINTR`エラーコードを返します。


**使用例**:

```c
#include "hako_asset.h"

// 他の初期化処理 ...

int main() {
    while (1) {
        int result = hako_asset_start();

        if (result == EINTR) {
            // シミュレーションリセットイベントが発生したための処理
            continue; // 必要に応じてリセット処理を行い、ループを継続
        } else {
            // その他のエラー処理
            printf("Error: %d\n", result);
            break; // 致命的なエラーの場合はループを抜ける
        }

        // 必要に応じてスリープを挿入してリアルタイム性を制御
    }
    return 0;
}
```


### 箱庭アセットの処理を実行する

**ユースケースID**: `UC-HAKO-ASSET-APP-1`

箱庭アセットを作成するユーザは、`hako_asset_callbacks_t` のイベントコールバック関数を実装する必要があります。

* on_initialize()
  * [箱庭アセットの初期化処理](#箱庭アセットの初期化処理)
* on_simulation_step()
  * [箱庭アセットの周期イベント実行処理](#箱庭アセットの周期イベント実行処理)
* on_manual_timing_control()
  * [箱庭アセットの手動タイミング制御処理](#箱庭アセットの手動タイミング制御処理)
* on_reset()
  * [箱庭アセットのリセット処理](#箱庭アセットのリセット処理)


上記コールバック処理では、箱庭アセットAPIを利用することができますが、一部、利用できないものがあります。
詳細は以下の通りです。

|箱庭アセットAPI|on_initialize|on_simulation_step|on_manual_timing_control|on_reset|
|---|---|---|---|---|
|hako_asset_register|X|X|X|X|
|hako_asset_start|X|X|X|X|
|hako_asset_pdu_read|O|O|O|O|
|hako_asset_pdu_write|O|O|O|O|
|hako_asset_simulation_time|O|O|O|O|
|hako_asset_usleep|X|X|O|X|

**コールバック関数テーブル**:

```c
typedef struct {
    int (*on_initialize)(hako_asset_context_t*);
    int (*on_simulation_step)(hako_asset_context_t*);
    int (*on_manual_timing_control)(hako_asset_context_t*);
    int (*on_reset)(hako_asset_context_t*);
} hako_asset_callbacks_t;
```

**コールバック関数の引数について**:
`hako_asset_context_t` は、将来的な機能拡張や追加データの管理を容易にするためのプレースホルダーとして設計されています。現時点では特に情報を持っていませんが、箱庭アセットの状態や環境設定など、箱庭アセットのユースケースの拡張に伴い、必要に応じてコンテキスト設定することを想定しています。

**コールバック関数の戻り値について**:
現時点では、コールバック関数の戻り値は特定のエラーコードを返すことを想定していませんので、常に0を返してください。将来的なバージョンの箱庭アセットAPIでは、エラーハンドリングやその他の通知機能のためにこれらの戻り値を利用する可能性があります。成功時の 0 と失敗時の非 0 の値によって、将来的なエラーや状態の変更を通知できる柔軟性を持たせています。


**使用例**:

```c
int my_initialize_callback(hako_asset_context_t* context) {
    // 初期化時の処理
}

int my_simulation_step_callback(hako_asset_context_t* context) {
    // 各シミュレーションステップ時の処理
}

int my_reset_callback(hako_asset_context_t* context) {
    // リセット時の処理
}

// 使用例
hako_asset_callbacks_t callbacks = {
    .on_initialize = my_initialize_callback,
    .on_simulation_step = my_simulation_step_callback,
    .on_manual_timing_control = NULL,
    .on_reset = my_reset_callback
};

```

#### 箱庭アセットの初期化処理

**ユースケースID**: `UC-HAKO-ASSET-APP-1a`

**コールバック関数名**: `on_initialize`

**目的**:  
このコールバック関数は、箱庭のシミュレーション開始時点で一度だけ呼び出され、箱庭アセットの初期設定や準備を行います。ここでは、リソースの割り当て、初期パラメータの設定、環境の構成など、シミュレーション開始前に必要な手順を実装してください。

**引数**:  
- `context`: 現時点では未使用ですが、将来的な拡張性を考慮して hako_asset_context_t* 型の引数を提供します。

**戻り値**:  
- 常に0を返してください。将来的にエラーハンドリングが必要になった場合には、異なるエラーコードを返すことができます。

#### 箱庭アセットの周期イベント実行処理

**ユースケースID**: `UC-HAKO-ASSET-APP-1b`

**コールバック関数名**: `on_simulation_step`

**目的**:  
本コールバック関数は、箱庭のシミュレーション時間ステップ毎に呼び出されます。
箱庭アセットとして、シミュレーション実行処理をこの関数内で実装してください。

* 注意1：本関数での処理実行中は、箱庭アセットのシミュレーション時間は進みません。そのため、本関数内で、無限ループの処理があると箱庭シミュレーションがハングアップしてしまうため、確実に処理完了したら関数復帰することを確認してください。

**引数**:  
- `context`: 現時点では未使用ですが、将来的な拡張性を考慮して hako_asset_context_t* 型の引数を提供します。

**戻り値**:  
- 常に0を返してください。将来的にエラーハンドリングが必要になった場合には、異なるエラーコードを返すことができます。



#### 箱庭アセットの手動タイミング制御処理

**ユースケースID**: `UC-HAKO-ASSET-APP-1c`

**コールバック関数名**: `on_manual_timing_control`

**目的**:  
本コールバック関数は、箱庭のシミュレーションが開始されたときに一度だけ呼び出され、ユーザーは独自のタイミングで処理を制御できます。箱庭アセットとして、シミュレーション実行処理をこの関数内で実装してください。

* 注意1：本関数での処理実行中は、箱庭のシミュレーション時間は進みません。箱庭のシミュレーション時間を進めるには、hako_asset_usleep()を呼び出して、タイミング調整をする必要があります。また、本関数内で、hako_asset_usleep()を呼び出さずに無限ループする処理があると、箱庭シミュレーションがハングアップしてしまうため、確実に定期的にhako_asset_usleep()を呼び出してください。
* 注意2：箱庭のリセットイベントが発生したら、本関数を必ず復帰してください。リセットイベントの検出は、コールバック関数かhako_asset_usleep()の戻り値で検出可能です。


**引数**:  
- `context`: 現時点では未使用ですが、将来的な拡張性を考慮して hako_asset_context_t* 型の引数を提供します。

**戻り値**:  
- 常に0を返してください。将来的にエラーハンドリングが必要になった場合には、異なるエラーコードを返すことができます。


#### 箱庭アセットのリセット処理

**ユースケースID**: `UC-HAKO-ASSET-APP-1d`

**コールバック関数名**: `on_reset`

**目的**:  
本コールバック関数は、箱庭のシミュレーションがリセットされたタイミングで呼び出されます。
箱庭アセットとして、シミュレーションリセット処理をこの関数内で実装してください。


**引数**:  
- `context`: 現時点では未使用ですが、将来的な拡張性を考慮して hako_asset_context_t* 型の引数を提供します。

**戻り値**:  
- 常に0を返してください。将来的にエラーハンドリングが必要になった場合には、異なるエラーコードを返すことができます。


### 箱庭PDUデータを読み込みする

**ユースケースID**: `UC-HAKO-ASSET-APP-COMMON-1`

**関数名**: `hako_asset_pdu_read`

**目的**:  
指定されたロボット名とチャンネルIDに基づいて、箱庭PDUデータを読み込みます。

**引数**:  
- `robo_name`: PDUデータを読み込む対象のロボット名。型: `const char*`
- `lchannel`: 読み込むデータのチャンネルID。型: `HakoPduChannelIdType`
- `buffer`: 読み込んだデータを格納するバッファへのポインタ。型: `char*`
- `buffer_len`: バッファの長さを指定します。型: `size_t`

**戻り値**:  
成功時は `0` を返します。失敗時は非 `0` のエラーコードを返します。

**エラーハンドリング**:  
- `robo_name` が NULL または空文字列の場合、`EINVAL` エラーコードを返します。
- `buffer` が NULL または `buffer_len` が 0 の場合、``EINVAL` エラーコードを返します。
- その他のエラーが発生した場合は、`EIO` エラーコードを返します

**使用例**:

```c
#include "hako_asset.h"

// PDU読み込みのサンプル実装
int read_pdu_data(const char* robo_name) {
    char buffer[1024]; // PDUデータを読み込むためのバッファ
    HakoPduChannelIdType channel_id = 1; // 使用するチャンネルID

    int result = hako_asset_pdu_read(robo_name, channel_id, buffer, sizeof(buffer));

    if (result != 0) {
        // エラーハンドリング
        printf("Failed to read PDU data: %d\n", result);
        return -1;
    }

    // 読み込んだデータの処理
    // ...

    return 0;
}

int main() {
    const char* robo_name = "example_robot";
    int result = read_pdu_data(robo_name);

    if (result != 0) {
        // エラーハンドリングが必要な場合の処理
    }

    // その他の処理...
    return 0;
}
```

### 箱庭PDUデータを書き込みする

**ユースケースID**: `UC-HAKO-ASSET-APP-COMMON-2`

**関数名**: `hako_asset_pdu_write`

**目的**:  
指定されたロボット名とチャンネルIDを使用して、箱庭PDUデータを書き込みます。

**引数**:  
- `robo_name`: PDUデータを書き込む対象のロボット名。型: `const char*`
- `lchannel`: 書き込むデータのチャンネルID。型: `HakoPduChannelIdType`
- `buffer`: 書き込むデータを格納したバッファへのポインタ。型: `const char*`
- `buffer_len`: バッファに含まれるデータの長さを指定します。型: `size_t`

**戻り値**:  
成功時は `0` を返します。失敗時は非 `0` のエラーコードを返します。

**エラーハンドリング**:  
- `robo_name` が NULL または空文字列の場合、`EINVAL` エラーコードを返します。
- `buffer` が NULL または `buffer_len` が 0 の場合、``EINVAL` エラーコードを返します。
- その他のエラーが発生した場合は、`EIO` エラーコードを返します

**使用例**:

```c
#include "hako_asset.h"

// PDU書き込みのサンプル実装
int write_pdu_data(const char* robo_name, const char* data, size_t data_len) {
    HakoPduChannelIdType channel_id = 1; // 使用するチャンネルID

    int result = hako_asset_pdu_write(robo_name, channel_id, data, data_len);

    if (result != 0) {
        // エラーハンドリング
        printf("Failed to write PDU data: %d\n", result);
        return -1;
    }

    // 書き込みが成功したことを確認
    // ...

    return 0;
}

int main() {
    const char* robo_name = "example_robot";
    const char* data = "Some PDU data";
    size_t data_len = strlen(data);

    int result = write_pdu_data(robo_name, data, data_len);

    if (result != 0) {
        // エラーハンドリングが必要な場合の処理
    }

    // その他の処理...
    return 0;
}
```

### 箱庭シミュレーション時間を取得する

**ユースケースID**: `UC-HAKO-ASSET-APP-COMMON-3`

**関数名**: `hako_asset_simulation_time`

**目的**:  
現在の箱庭シミュレーションの時間を取得します。

**引数**:  
この関数は引数を取りません。

**戻り値**:  
現在の箱庭シミュレーション時間を `hako_time_t` 型で返します。この値は、シミュレーション開始時からの経過時間をマイクロ秒単位で表します。

**エラーハンドリング**:  
この関数は、時間取得の失敗を示すためにエラーコードを返すことはありません。ただし、異常な動作が検出された場合（例えば、シミュレーション時間が初期化されていないなど）の戻り値は不定です。

**使用例**:

```c
#include "hako_asset.h"

int main() {
    hako_time_t sim_time = hako_asset_simulation_time();

    // シミュレーション時間の表示やその他の処理...
    printf("Current simulation time: %lld microseconds\n", (long long) sim_time);

    // その他の処理...
    return 0;
}
```

### スリープする

**ユースケースID**: `UC-HAKO-ASSET-APP-COMMON-4`

**関数名**: `hako_asset_usleep`

**目的**:  
指定された時間（マイクロ秒単位）だけスリープ状態にします。
スリープすると、`sleep_time_usec`だけシミュレーション時間が進んだところで復帰します。
`0` が指定された場合は、最小の時間ステップ（`delta_usec`）で復帰します。
`sleep_time_usec`が、最小の時間ステップ（`delta_usec`）で端数が出る場合は、切り上げられます。

**引数**:  
- `sleep_time_usec`: スリープする時間の長さをマイクロ秒単位で指定します。型: `hako_time_t`

**戻り値**:  
成功時は `0` を返します。失敗時は非 `0` のエラーコードを返します。

**エラーハンドリング**:  
- `sleep_time_usec` が不正な場合、`EINVAL` エラーコードを返します。
- リセットイベントが発生した場合、`EINTR` エラーコードを返します。

**使用例**:

```c
#include "hako_asset.h"

int main() {
    hako_time_t sleep_duration = 1000000; // 1秒間スリープする

    int result = hako_asset_usleep(sleep_duration);

    if (result != 0) {
        // エラーハンドリング
        printf("usleep failed: %d\n", result);
        // エラー処理...
    }

    // スリープ後の処理...
    return 0;
}
```

## サンプルコード

箱庭アセットAPIを深く利用するための動作確認およびサンプルコードを用意しています。
[こちら](https://github.com/toppers/hakoniwa-core-cpp-client/tree/main/examples)を参照ください。


# インストール手順

**リポジトリのクローン:**

```
git clone --recursive https://github.com/toppers/hakoniwa-core-cpp-client.git
```

**ビルド:**

```
cd hakoniwa-core-cpp-client
```

```
bash build.bash
```

**インストール:**

```
bash install.bash
```

インストール時に、以下へのアクセス許可が必要になることがあります。この許可を与えると、箱庭のライブラリやコマンドがこれらのディレクトリに配置されます。

* /etc/hakoniwa
* /var/lib/hakoniwa
* /usr/local/lib/hakoniwa
* /usr/local/bin/hakoniwa 

インストールが成功している場合は、以下のコマンドでチェックできます。

```
bash hako-setup-check.bash
```

成功している場合は、以下のログが出力されます。

```
=== HAKO ENV ===
OS=Linux , OS_TYPE=posix
HAKO_CORE_PREFIX=/usr/local
HAKO_CONFIG_PREFIX=/etc/hakoniwa
HAKO_CORE_MMAP_PREFIX=/var/lib/hakoniwa
HAKO_CONFIGNFIG_PATH=/etc/hakoniwa/cpp_core_config.json
HAKO_CORE_MMAP_PATH=/var/lib/hakoniwa/mmap

OK Directory exists: /usr/local/bin
OK Directory exists: /usr/local/bin/hakoniwa
OK Directory exists: /usr/local/lib
OK Directory exists: /usr/local/lib/hakoniwa
OK Directory exists: /etc/hakoniwa
OK Directory exists: /var/lib/hakoniwa
OK Directory exists: /var/lib/hakoniwa/mmap
OK File exists: /etc/hakoniwa/cpp_core_config.json
OK File exists: /usr/local/bin/hakoniwa/hako-cmd
OK File exists: /usr/local/lib/hakoniwa/libhakoarun.a
OK File exists: /usr/local/lib/hakoniwa/libshakoc.dylib
OK File exists: /usr/local/lib/hakoniwa/hakoc.so
OK File exists: /usr/local/lib/hakoniwa/libassets.dylib
OK File exists: /usr/local/lib/hakoniwa/libconductor.dylib
OK File exists: /usr/local/lib/hakoniwa/py
OK File exists: /usr/local/bin/hakoniwa/hako-proxy
Check complete.
```

**環境変数の設定:**

インストールが成功した後、これらのパスを環境変数に設定してください。

Ubuntuの場合:

~/.bashrc ファイルに以下の行を追加してください。

```sh
export LD_LIBRARY_PATH=/usr/local/lib/hakoniwa:$LD_LIBRARY_PATH
export PATH=/usr/local/bin/hakoniwa:$PATH
```

変更を反映させるために、次のコマンドを実行します。

```sh
source ~/.bashrc
```

macOSの場合：

使用しているシェルに応じて ~/.bash_profile または ~/.zshrc に以下の行を追加します。

```sh
export DYLD_LIBRARY_PATH=/usr/local/lib/hakoniwa:$DYLD_LIBRARY_PATH
export PATH=/usr/local/bin/hakoniwa:$PATH
```

変更を反映させるために、次のコマンドを実行します。

```sh
source ~/.bash_profile  # Bashの場合
source ~/.zshrc         # Zshの場合
```

# MMAPファイルの設定

箱庭アセット間のPDU通信は、は共有メモリベースで行います。

共有メモリ方式として、以下の２方式を選択できます。

* Shared Memory(SHM)※ Windows は未サポートでうs
* Memory Mapped File(MMAP)

インストール直後は、デフォルトとして、SHM に設定されています。

**MMAP の特徴：**
* ファイルベースのデータ共有:
  * MMAPはファイルベースで動作します。つまり、ディスク上のファイルを直接メモリにマッピングします。これにより、ファイルの内容を直接メモリ上で読み書きできるため、ディスクI/O操作を効率的に行うことができます。共有メモリと比べて、データの永続化が必要な場合に特に有利です。
* 自動的なデータの同期:
  * メモリマップドファイルはオペレーティングシステムによって管理されるため、変更された内容は自動的にファイルに書き戻されます。これにより、プロセスがクラッシュした場合でも、マッピングされたファイルの内容は保持されるため、データの安全性が向上します。
* 大きなデータセットの扱いやすさ:
  * MMAPは大きなファイルを扱う際に有利です。ファイル全体を一度にメモリにロードする代わりに、必要な部分だけをメモリにマッピングすることができるため、メモリ使用量を節約し、大きなデータセットの処理を効率化できます。
* 柔軟性:
  * MMAPを使用すると、ファイルの特定の部分を異なるアドレス空間にマッピングすることができます。これにより、アプリケーションにとって都合の良い方法でデータにアクセスすることが可能になります。
* ポータビリティ:
  * 多くのオペレーティングシステムがMMAPをサポートしているため、異なるプラットフォーム間でのアプリケーションの移植性が向上します。

インストール完了後、以下の方法で MMAP に設定変更できます。

```
sudo bash hako-mmap-set.bash -p /var/lib/hakoniwa/mmap
```

成功すると、`/etc/hakoniwa/cpp_core_config.json` が以下のように変更されます。

```
{
  "shm_type": "mmap",
  "core_mmap_path": "/var/lib/hakoniwa/mmap"
}
```

MMAPファイルは、`core_mmap_path`  配下に自動作成されます。

また、MMAPファイルを `ramdisk` に配置するこで、処理性能を向上させることができます。

MacOSとLinuxの場合は、`hako-ramdisk.bash` を利用して `ramdisk` を作成できます。

作成した `ramdisk` パスを `hako-mmap-set.bash` で再設定することで反映されます。

**仕様：**
```
Usage:
  hako-ramdisk.bash -c /path/to -s size   # Create a RAM disk with size in MB
  hako-ramdisk.bash -d /path/to           # Delete a RAM disk
  hako-ramdisk.bash -l                    # List created RAM disks
```

**例：64MBの `ramdisk` を `/Volumes/hakoniwa-ramdisk` に作成します。**
```
sudo bash hako-ramdisk.bash -c /Volumes/hakoniwa-ramdisk -s 64
```

```
% sudo bash hako-ramdisk.bash -l                                
Listing RAM disks...
/dev/disk4 is mounted at /Volumes/hakoniwa-ramdisk
Operation completed.
```

**`ramdisk` を削除したい場合：**
```
sudo bash hako-ramdisk.bash -d /Volumes/hakoniwa-ramdisk 
```


# Windows向けインストール手順

Windows 向けに箱庭コア機能をインストールする場合、以下のツールを利用します。

* Visual Studio(C++)
  * ビルドモードは、x64-Release としてください
* Power shell
* WSL2
* [Python 3.12](https://www.python.org/)

**リポジトリのクローン:**

WSL2 を利用して、クローンします。

```
git clone --recursive https://github.com/toppers/hakoniwa-core-cpp-client.git
```

**ビルド:**

1. Visual Stuido　を起動し、「ローカルフォルダーを開く」を選択します。
2. hakoniwa-core-cpp-client を選択します。
3. 「ビルド」→「すべてビルド」を選択するとビルドが始まります。


**インストール:**

Windowsコンソールを開き、`hakoniwa-core-cpp-client` に移動し、インストールコマンドを実行します。

```
 .\install.bat
```

成功すると、以下の環境変数が設定されます。

* HAKO_CONFIG_PATH
  * <path/to>\hakoniwa-core-cpp-client\cpp_core_config.json
* PATH
  * <path/to>\hakoniwa-core-cpp-client\out\build\x64-Release\core\sample\base-procs\hako-cmd

もし、設定されていない場合は、手動で設定してください。


# MMAPファイルの設定

Windows版の箱庭コア機能は、MMAP通信が前提となります。


```
.\hako-mmap-set.bat -p Z:\\mmap
```

成功すると、`cpp_core_config.json` が以下のように変更されます。

```
{
    "shm_type":  "mmap",
    "core_mmap_path":  "Z:\\mmap"
}
```

MMAPファイルは、`core_mmap_path`  配下に自動作成されます。

また、MMAPファイルを `ramdisk` に配置するこで、処理性能を向上させることができます。

作成した `ramdisk` パスを `hako-mmap-set.bat` で再設定することで反映されます。

# Windows向け(gitbash)インストール手順

Windows 向けに箱庭コア機能をインストールする場合、以下のツールを利用します。

* Visual Studio Build Tools
* Git for Windows
* CMake
* [Python 3.12](https://www.python.org/)
* jq

## wingetコマンドでのツールのインストール
コマンドプロンプトもしくはPowerShellからwingetコマンドを用いてツールをインストールします。

**Visual Studio Build Tools:**
```
winget install Microsoft.VisualStudio.2022.BuildTools --override "--add Microsoft.VisualStudio.Workload.VCTools --includeRecommended"
```

**Git for Windows**
```
winget install --id Git.Git -e --source winget
```

**CMake:**
```
winget install Kitware.CMake
```

**Python**
```
winget install Python.Python.3.12
```

**jq**
```
winget install jqlang.jq
```
## クローンとビルド
Git for WindowsのGit Bashを使用します。
VSCodeを起動するとTerminalの選択で「Git Bash」を選びます

**ビルド:**

```
cd hakoniwa-core-cpp-client
```

```
bash build.bash
```

**インストール:**

```
bash install.bash
```
**設定の確認:**

```
bash ./hako-setup-check.bash 
```

# 箱庭コマンド API

TODO(コントリビュート募集中)

# 箱庭コンダクタ API 

TODO(コントリビュート募集中)

# 箱庭アセットコンフィグ

箱庭アセットのコンフィグは、json形式で定義します。
コンフィグ内容は、プラントモデルのPDUデータの読み書き情報になります。

例：

```json
{
  "robots": [
    {
      "name": "ROBOT",
      "rpc_pdu_readers": [],
      "rpc_pdu_writers": [],
      "shm_pdu_readers": [
        {
          "type": "geometry_msgs/Twist",
          "org_name": "motor",
          "name": "ROBOT_motor",
          "channel_id": 0,
          "pdu_size": 48,
          "write_cycle": 1,
          "method_type": "SHM"
        }        
      ],
      "shm_pdu_writers": []
    }
  ]
}
```

## コンフィグ項目の詳細


### robots

`プラントモデル（ロボット）のコンフィグデータ`を管理するコンテナです。複数のロボットを定義することが可能です。

### プラントモデル（ロボット）コンフィグデータ

プラントモデルのコンフィグデータの内容は以下のとおり。

* name: ロボットを識別する名前。
* rpc_pdu_readers: `PDUデータのコンフィグ定義`
* rpc_pdu_writers: `PDUデータのコンフィグ定義`
* shm_pdu_readers: `PDUデータのコンフィグ定義`
* shm_pdu_writers: `PDUデータのコンフィグ定義`

RPCとSHMの違い：

* rpc: PDUのI/OをUDPやMQTTなどの通信で行います。
* shm: PDUのI/Oを共有メモリで行います。

pdu_readersとpdu_writers：

* pdu_readers: PDUデータの読み込み用。
* pdu_writers: PDUデータの書き込み用。

### PDUデータのコンフィグ定義

PDUデータに関する各設定項目の定義は以下の通りです。

* type: PDUのデータ型。ROSのIDLで定義されたデータ型を指定します。
* org_name: PDUデータ固有の名前。任意の名前を指定できます。
* name: ロボット名を接頭辞としたPDUデータ名。（複数のロボットが同じPDUデータを使用する場合の競合を避けるため）
* channel_id: PDUの論理チャネルID。
* pdu_size: PDUのサイズ（バイト単位）。
* write_cycle: PDUの書き込み周期。箱庭アセットがPDUを書き込む実行周期に対する倍数を指定します。
* method_type: PDU通信方式の指定。設定可能な値は以下の通りです。
  * SHM: 共有メモリ
  * UDP: UDP通信
  * MQTT: MQTT通信

# Windows 向け箱庭コア機能のパッケージング構成

Windows 向け箱庭コア機能のパッケージング構成は以下の通りです。

- <パッケージングディレクトリ>
  - hako-cmd.exe : 箱庭コマンド(Power Shellでのデバッグ用)
  - shakoc.dll : 箱庭コア機能のDLLであり、主に Unity プラグインとして利用する
  - hakopy.pyd : 箱庭コア機能のPythonラッパー
  - hako_pdu.py : 箱庭PDUアクセス用のPythonライブラリ
  - hako_binary : 箱庭PDUアクセス用のPythonのサブライブラリ群と箱庭PDUのデータ定義ファイル(offset配下)
  - cpp_core_config.json : 箱庭コア機能の設定ファイル

環境変数の設定は以下の通りです。

- HAKO_CONFIG_PATH : <パッケージングディレクトリ>/cpp_core_config.json
- HAKO_BINARY_PATH : <パッケージングディレクトリ>/hako_binary/offset
- PATH : <パッケージングディレクトリ>
- PYTHONPATH : <パッケージングディレクトリ>

パッケージ全体構成図：
<img width="996" alt="スクリーンショット 2025-02-27 7 25 04" src="https://github.com/user-attachments/assets/335bf959-c739-433d-953d-b1e45fd8e0a1" />

パッケージ対象ファイルのビルド配置場所：

- hako-cmd.exe : out/build/x64-Release/core/sample/base-procs/hako-cmd/hako-cmd.exe
- shakoc.dll : out/build/x64-Release/src/hakoc/shakoc.dll
- hakopy.pyd : out/build/x64-Release/bindings/python/hako_asset_python.dll
  - hako_asset_python.dll を hakopy.pyd にリネームします
- hako_pdu.py : bindings/python/hako_pdu.py
- hako_binary : bindings/python/hako_binary
- cpp_core_config.json : core/cpp_core_config.json
