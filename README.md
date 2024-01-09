[![Build](https://github.com/toppers/hakoniwa-core-cpp-client/actions/workflows/build.yml/badge.svg)](https://github.com/toppers/hakoniwa-core-cpp-client/actions/workflows/build.yml)

このリポジトリは、C/C++で開発された箱庭コア機能([hakoniwa-core-cpp](https://github.com/toppers/hakoniwa-core-cpp/tree/main))を以下の３つの機能分類で、C API として提供することを目的としています。

1. [箱庭アセット](https://github.com/toppers/hakoniwa-document/blob/main/architecture/README.md#%E7%AE%B1%E5%BA%AD%E3%82%A2%E3%82%BB%E3%83%83%E3%83%88)
2. 箱庭コマンド
3. [箱庭コンダクタ](https://github.com/toppers/hakoniwa-conductor)

![スクリーンショット 2024-01-04 14 04 47](https://github.com/toppers/hakoniwa-core-cpp-client/assets/164193/80d2d524-c115-486a-8b99-5da2b83a6fa8)


# 箱庭アセット向け API

## 箱庭アセットのユースケース分類

箱庭アセットのユースケースとして、大別して以下の３分類があります。

<img width="567" alt="スクリーンショット 2024-01-10 7 03 25" src="https://github.com/toppers/hakoniwa-core-cpp-client/assets/164193/7b10d4a4-f0cb-4175-80c5-901c9359bd31">

* **アプリケーション固有のユースケース** は、シミュレーションしたい処理内容そのものであり、**箱庭ユーザが作成する機能**です。例えば、ロボットのシミュレーションの場合は、ロボットの制御プログラム等です。

* **アプリケーション共通のユースケース** は、シミュレーション処理において共通する**箱庭の機能セット**です。例えば、他アプリケーションとの通信等があります。

* **シミュレーション向けユースケース** は、箱庭というシミュレーション環境を利用するための**箱庭の機能セット**です。例えば、シミュレーション時間を進める等があります。

## 箱庭アセットのユースケース

箱庭アセットのシミュレーションの基本処理フローは以下であり、1-3が箱庭アセットとしての基本ユースケースになります。

1. 箱庭アセットを登録する **[UC-HAKO-ASSET-1]**
2. 箱庭コア機能に対してシミュレーション実行を依頼する **[UC-HAKO-ASSET-2]**
3. 箱庭コア機能からのイベント（コールバック）として、箱庭アセットの処理を実行する **[UC-HAKO-ASSET-3]**
4. シミュレーション終了まで 2-3 を繰り返す

ここで、**[UC-HAKO-ASSET-3]** の処理は、以下の３種類に分類されます。
それぞれ、シミュレーションの必要なタイミングで、箱庭コア機能からコールバックされます。

* 箱庭アセットの初期化処理 **[UC-HAKO-ASSET-3-a]**
  * 箱庭のシミュレーション開始時点で１回だけ呼び出されます。
* 箱庭アセットのシミュレーション実行処理 **[UC-HAKO-ASSET-3-b]**
  * 箱庭のシミュレーション時間ステップ毎に呼び出されます。
* 箱庭アセットのリセット処理 **[UC-HAKO-ASSET-3-c]**
  * 箱庭のシミュレーションがリセットされたタイミングで呼び出されます。

また、箱庭アセットは、必要に応じて、以下の箱庭コア機能を利用できます。

* 箱庭PDUデータを読み込みする **[UC-HAKO-ASSET-4]**
* 箱庭PDUデータを書き込みする **[UC-HAKO-ASSET-5]**
* 箱庭シミュレーション時間を取得する **[UC-HAKO-ASSET-6]**
* スリープする **[UC-HAKO-ASSET-7]**

<img width="876" alt="スクリーンショット 2024-01-04 15 26 06" src="https://github.com/toppers/hakoniwa-core-cpp-client/assets/164193/f32eaf6f-725d-4c80-8b23-e6aebb96f7c1">



## 箱庭 API 仕様

### 箱庭アセットを登録する [UC-HAKO-ASSET-1]


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


### 箱庭コア機能に対してシミュレーション実行を依頼する [UC-HAKO-ASSET-2]

**関数名**: `hako_asset_execute`

**目的**:  
登録済みの箱庭アセットに対して、指定されたステップ数だけシミュレーションを進めるように依頼します。

**引数**:  
- `increment_step`: シミュレーションを進めるステップ数。登録APIで指定したシミュレーション時間精度に従います。2以上の値を指定した場合は、1step進む毎に、`箱庭アセットのシミュレーション実行処理`が呼び出されます。型: `hako_time_t`

**戻り値**:  
成功時は `0` を返します。失敗時は非 `0` のエラーコードを返します。


**特殊ケース**:  
1. **シミュレーション未開始時の動作**:  
   シミュレーション開始イベントが発生するまでスリープ状態になります。シミュレーション開始イベントの発生により、`箱庭アセットの初期化処理`が呼び出された後、`箱庭アセットのシミュレーション実行処理`が呼び出されます。
   
2. **シミュレーション・リセットイベント発生時の動作**:  
   シミュレーション実行中にリセットイベントが発生した場合、`EINTR` エラーコードを返して処理を復帰します。復帰する前に、`箱庭アセットのリセット処理`が呼び出されます。

**エラーハンドリング**:  
- `increment_step` が不正な場合、`EINVAL` エラーコードを返します。
- シミュレーションリセットイベントが発生した場合、`EINTR` エラーコードを返します。

**使用例**:

```c
#include <errno.h>
#include "hako_asset.h"

// 他の初期化処理 ...

int main() {
    hako_time_t step = 10; // シミュレーションを10ステップ進める

    while (1) {
        int result = hako_asset_execute(step);

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


### 箱庭コア機能からのイベント（コールバック）として、箱庭アセットの処理を実行する [UC-HAKO-ASSET-3]

箱庭アセットを作成するユーザは、`hako_asset_callbacks_t` のイベントコールバック関数を実装する必要があります。

* on_initialize()
  * 箱庭アセットの初期化処理 [UC-HAKO-ASSET-3-a]
* on_simulation_step()
  * 箱庭アセットのシミュレーション実行処理 [UC-HAKO-ASSET-3-b]
* on_reset()
  * 箱庭アセットのリセット処理 [UC-HAKO-ASSET-3-c]


上記コールバック処理では、箱庭アセットAPIを利用することができますが、一部、利用できないものがあります。
詳細は以下の通りです。

|箱庭アセットAPI|on_initialize|on_simulation_step|on_reset|
|---|---|---|---|
|hako_asset_register|X|X|X|
|hako_asset_execute|X|X|X|
|hako_asset_pdu_read|O|O|O|
|hako_asset_pdu_write|O|O|O|
|hako_asset_simulation_time|O|O|O|
|hako_asset_usleep|X|O|X|


**使用例**:

```c
typedef struct {
    void (*on_initialize)(void);
    void (*on_simulation_step)(void);
    void (*on_reset)(void);
} hako_asset_callbacks_t;

void my_initialize_callback(void* context) {
    // 初期化時の処理
}

void my_simulation_step_callback(void* context) {
    // 各シミュレーションステップ時の処理
}

void my_reset_callback(void* context) {
    // リセット時の処理
}

// 使用例
hako_asset_callbacks_t callbacks = {
    .on_initialize = my_initialize_callback,
    .on_simulation_step = my_simulation_step_callback,
    .on_reset = my_reset_callback
};

```
#### 箱庭アセットの初期化処理 [UC-HAKO-ASSET-3-a]

**コールバック関数名**: `on_initialize`

**目的**:  
本コールバック関数は、箱庭のシミュレーション開始時点で１回だけ呼び出されます。
箱庭アセットとして、必要な初期化をこの関数内で実装してください。

**引数**:  
なし

**戻り値**:  
なし

#### 箱庭アセットのシミュレーション実行処理 [UC-HAKO-ASSET-3-b]

**コールバック関数名**: `on_simulation_step`

**目的**:  
本コールバック関数は、箱庭のシミュレーション時間ステップ毎に呼び出されます。
箱庭アセットとして、シミュレーション実行処理をこの関数内で実装してください。

**引数**:  
なし

**戻り値**:  
なし

#### 箱庭アセットのリセット処理 [UC-HAKO-ASSET-3-c]

**コールバック関数名**: `on_reset`

**目的**:  
本コールバック関数は、箱庭のシミュレーションがリセットされたタイミングで呼び出されます。
箱庭アセットとして、シミュレーションリセット処理をこの関数内で実装してください。

**引数**:  
なし

**戻り値**:  
なし

### 箱庭PDUデータを読み込みする [UC-HAKO-ASSET-4]

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

### 箱庭PDUデータを書き込みする [UC-HAKO-ASSET-5]

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

### 箱庭シミュレーション時間を取得する [UC-HAKO-ASSET-6]

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

### スリープする [UC-HAKO-ASSET-7]

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
- システムコールが割り込まれた場合、`EINTR` エラーコードを返します。

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
