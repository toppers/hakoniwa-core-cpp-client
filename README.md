[![Build](https://github.com/toppers/hakoniwa-core-cpp-client/actions/workflows/build.yml/badge.svg)](https://github.com/toppers/hakoniwa-core-cpp-client/actions/workflows/build.yml)

このリポジトリは、C/C++で開発された箱庭コア機能([hakoniwa-core-cpp](https://github.com/toppers/hakoniwa-core-cpp/tree/main))を以下の３つの機能分類で、C API として提供することを目的としています。

1. [箱庭アセット](https://github.com/toppers/hakoniwa-document/blob/main/architecture/README.md#%E7%AE%B1%E5%BA%AD%E3%82%A2%E3%82%BB%E3%83%83%E3%83%88)
2. 箱庭コマンド
3. [箱庭コンダクタ](https://github.com/toppers/hakoniwa-conductor)

![スクリーンショット 2024-01-04 14 04 47](https://github.com/toppers/hakoniwa-core-cpp-client/assets/164193/80d2d524-c115-486a-8b99-5da2b83a6fa8)


# 箱庭アセット向け API

## 箱庭アセットのユースケース分類

箱庭アセットのユースケースは、大別して以下の３つのカテゴリがあります。

<img width="567" alt="スクリーンショット 2024-01-10 7 03 25" src="https://github.com/toppers/hakoniwa-core-cpp-client/assets/164193/7b10d4a4-f0cb-4175-80c5-901c9359bd31">

* **アプリケーション固有のユースケース** は、シミュレーションしたい処理内容そのものであり、**箱庭ユーザが作成する機能**です。例えば、ロボットのシミュレーションの場合は、ロボットの制御プログラム等です。

* **アプリケーション共通のユースケース** は、シミュレーション処理において共通する**箱庭の機能セット**です。例えば、他アプリケーションとの通信等があります。

* **シミュレーション向けユースケース** は、箱庭というシミュレーション環境を利用するための**箱庭の機能セット**です。例えば、箱庭アセットの登録等があります。

## 箱庭アセットの処理フロー

箱庭アセットのシミュレーションの処理フローは以下のとおりであり、**シミュレーション向けのユースケース** が中心となります。

1. [箱庭アセットを登録する](#箱庭アセットを登録する) (シミュレーション向け)
2. [箱庭アセットのシミュレーションを開始する](#箱庭アセットのシミュレーションを開始する) (シミュレーション向け)
3. [箱庭コア機能からのイベント（コールバック）として、箱庭アセットの処理を実行する](#箱庭コア機能からのイベント（コールバック）として、箱庭アセットの処理を実行する) (アプリケーション固有)
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

![スクリーンショット 2024-01-10 7 33 58](https://github.com/toppers/hakoniwa-core-cpp-client/assets/164193/d296a70f-3393-47b1-82c1-a54cf9eb7506)


## 箱庭 API 仕様

### 箱庭アセットを登録する

**ユースケースID**: `UC-HAKO-ASSET-SIM-1`

**関数名**: `hako_asset_register`

**目的**:  
箱庭シミュレーション環境内で使用されるアセットを登録し、コールバック関数を関連付けます。

**引数**:  
- `asset_name`: 登録するアセットの名前。アセットを一意に識別するために使用されます。型: `const char*`
- `config_path`: アセットの設定ファイルへのパス。アセットの初期化や設定に必要な情報が含まれることを想定します。型: `const char*`
- `callback`: アセットのイベント処理を行うコールバック関数へのポインタ。このコールバックは、アセットの初期化、シミュレーションステップ、リセットなどのイベントが発生したときに呼び出されます。型: `hako_asset_callbacks_t*`
- `delta_usec`: シミュレーション時間のタイムステップをマイクロ秒単位で指定します。この値はシミュレーションが更新される頻度を決定します。型: `hako_time_t`

**戻り値**:  
成功時は `0` を返します。失敗時は非 `0` のエラーコードを返します。

**エラーハンドリング**:  
- `asset_name` が NULL または空文字列の場合、`EINVAL`（不正な引数）エラーコードを返します。
- `config_path` が不正なパスであるか、指定された設定ファイルが存在しない場合、`ENOENT`（ファイルが存在しない）エラーコードを返します。
- `callback` が NULL の場合、`EINVAL` エラーコードを返します。
- `callback` に、[箱庭アセットの周期イベント実行処理](#箱庭アセットの周期イベント実行処理)と[箱庭アセットの手動タイミング制御処理](#箱庭アセットの手動タイミング制御処理)が両方設定されている場合は、`EINVAL` エラーコードを返します。
- `delta_usec` が許容される範囲外の場合、`ERANGE`（数値の範囲外）エラーコードを返します。

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
    int result = hako_asset_register(asset_name, config_path, my_asset_callback, 1000000); // 1秒ごとに更新

    if (result != 0) {
        // エラーハンドリング
        printf("Error: %s\n", strerror(errno));
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


**使用例**:

```c
#include <errno.h>
#include "hako_asset.h"

// 他の初期化処理 ...

int main() {
    while (1) {
        int result = hako_asset_start();

        if (result != 0) {
            if (errno == EINTR) {
                // シミュレーションリセットイベントが発生したための処理
                continue; // 必要に応じてリセット処理を行い、ループを継続
            } else {
                // その他のエラー処理
                printf("Error: %s\n", strerror(errno));
                break; // 致命的なエラーの場合はループを抜ける
            }
        }

        // 必要に応じてスリープを挿入してリアルタイム性を制御
    }
    return 0;
}
```


### 箱庭コア機能からのイベント（コールバック）として、箱庭アセットの処理を実行する

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
|---|---|---|---|
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


**コールバック関数の引数について**:
`hako_asset_context_t`` は、将来的な機能拡張や追加データの管理を容易にするためのプレースホルダーとして設計されています。現時点では特に情報を持っていませんが、箱庭アセットの状態や環境設定など、箱庭アセットのユースケースの拡張に伴い、必要に応じてコンテキスト設定することを想定しています。

**コールバック関数の戻り値について**:
現時点では、コールバック関数の戻り値は特定のエラーコードを返すことを想定していませんので、常に0を返してください。将来的なバージョンの箱庭アセットAPIでは、エラーハンドリングやその他の通知機能のためにこれらの戻り値を利用する可能性があります。成功時の 0 と失敗時の非 0 の値によって、将来的なエラーや状態の変更を通知できる柔軟性を持たせています。


**使用例**:


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
成功時は読み込んだデータのバイト数を返します。失敗時は `-1` を返し、`errno` がエラーの原因を示します。

**エラーハンドリング**:  
- `robo_name` が NULL または空文字列の場合、`errno` に `EINVAL` を設定します。
- `buffer` が NULL または `buffer_len` が 0 の場合、`errno` に `EINVAL` を設定します。
- 指定された `robo_name` や `lchannel` が存在しない場合、`errno` に `ENODEV` を設定します。

**使用例**:

```c
#include <errno.h>
#include "hako_asset.h"

// PDU読み込みのサンプル実装
int read_pdu_data(const char* robo_name) {
    char buffer[1024]; // PDUデータを読み込むためのバッファ
    HakoPduChannelIdType channel_id = 1; // 使用するチャンネルID

    int bytes_read = hako_asset_pdu_read(robo_name, channel_id, buffer, sizeof(buffer));

    if (bytes_read == -1) {
        // エラーハンドリング
        printf("Failed to read PDU data: %s\n", strerror(errno));
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
成功時は書き込んだデータのバイト数を返します。失敗時は `-1` を返し、`errno` がエラーの原因を示します。

**エラーハンドリング**:  
- `robo_name` が NULL または空文字列の場合、`errno` に `EINVAL` を設定します。
- `buffer` が NULL または `buffer_len` が 0 の場合、`errno` に `EINVAL` を設定します。
- 指定された `robo_name` や `lchannel` に対応する書き込み先が存在しない場合、`errno` に `ENODEV` を設定します。

**使用例**:

```c
#include <errno.h>
#include "hako_asset.h"

// PDU書き込みのサンプル実装
int write_pdu_data(const char* robo_name, const char* data, size_t data_len) {
    HakoPduChannelIdType channel_id = 1; // 使用するチャンネルID

    int bytes_written = hako_asset_pdu_write(robo_name, channel_id, data, data_len);

    if (bytes_written == -1) {
        // エラーハンドリング
        printf("Failed to write PDU data: %s\n", strerror(errno));
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
指定された時間（マイクロ秒単位）だけプロセスをスリープ状態にします。
スリープすると、`sleep_time_usec`だけシミュレーション時間が進んだところで復帰します。
本APIでシミュレーション時間を進める場合は、コールバック関数は呼び出されません。


**引数**:  
- `sleep_time_usec`: スリープする時間の長さをマイクロ秒単位で指定します。型: `hako_time_t`

**戻り値**:  
成功時は `0` を返します。失敗時は `-1` を返し、`errno` がエラーの原因を示します。

**エラーハンドリング**:  
- `sleep_time_usec` が不正な場合、`EINVAL` エラーコードを返します。
- リセットイベントが発生した場合、`EINTR` エラーコードを返します。

**使用例**:

```c
#include <errno.h>
#include "hako_asset.h"

int main() {
    hako_time_t sleep_duration = 1000000; // 1秒間スリープする

    int result = hako_asset_usleep(sleep_duration);

    if (result == -1) {
        // エラーハンドリング
        printf("usleep failed: %s\n", strerror(errno));
        // エラー処理...
    }

    // スリープ後の処理...
    return 0;
}
```


## サンプルコード

TODO

## 動作確認手順

TODO

# 箱庭コマンド向け API

TODO(コントリビュート募集中)

# 箱庭コンダクタ向け API 

TODO(コントリビュート募集中)
