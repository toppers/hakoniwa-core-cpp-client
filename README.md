[![Build](https://github.com/toppers/hakoniwa-core-cpp-client/actions/workflows/build.yml/badge.svg)](https://github.com/toppers/hakoniwa-core-cpp-client/actions/workflows/build.yml)

このリポジトリは、C/C++で開発された箱庭コア機能([hakoniwa-core-cpp](https://github.com/toppers/hakoniwa-core-cpp/tree/main))を以下の３つの機能分類で、C API として提供することを目的としています。

1. 箱庭アセット
2. 箱庭コマンド
3. [箱庭コンダクタ](https://github.com/toppers/hakoniwa-conductor)

![スクリーンショット 2024-01-04 14 04 47](https://github.com/toppers/hakoniwa-core-cpp-client/assets/164193/80d2d524-c115-486a-8b99-5da2b83a6fa8)


# 箱庭アセット向け API

## 箱庭アセットのユースケース

箱庭アセットのシミュレーションの基本処理フローは以下であり、1-3が箱庭アセットとしての基本ユースケースになります。

1. 箱庭アセットを登録する **[HAKO-ASSET-UC-1]**
2. 箱庭コア機能に対してシミュレーション実行を依頼する **[HAKO-ASSET-UC-2]**
3. 箱庭コア機能からのイベント（コールバック）として、箱庭アセットの処理を実行する **[HAKO-ASSET-UC-3]**
4. シミュレーション終了まで 2-3 を繰り返す

ここで、**[HAKO-ASSET-UC-3]** の処理は、以下の３種類に分類されます。
それぞれ、シミュレーションの必要なタイミングで、箱庭コア機能からコールバックされます。

* 箱庭アセットの初期化処理 **[HAKO-ASSET-UC-3-a]**
  * 箱庭のシミュレーション開始時点で１回だけ呼び出されます。
* 箱庭アセットのシミュレーション実行処理 **[HAKO-ASSET-UC-3-b]**
  * 箱庭のシミュレーション時間ステップ毎に呼び出されます。
* 箱庭アセットのリセット処理 **[HAKO-ASSET-UC-3-c]**
  * 箱庭のシミュレーションがリセットされたタイミングで呼び出されます。

また、箱庭アセットは、必要に応じて、以下の箱庭コア機能を利用できます。

* 箱庭シミュレーション時間を取得する **[HAKO-ASSET-UC-4]**
* 箱庭PDUデータを読み込みする **[HAKO-ASSET-UC-5]**
* 箱庭PDUデータを書き込みする **[HAKO-ASSET-UC-6]**

<img width="674" alt="スクリーンショット 2024-01-04 14 36 01" src="https://github.com/toppers/hakoniwa-core-cpp-client/assets/164193/4b690689-d5ac-4eba-82cd-69dafb4fdf50">



## 箱庭 API 仕様

TODO

## サンプルコード

TODO

## 動作確認手順

TODO

# 箱庭コマンド向け API

TODO(コントリビュート募集中)

# 箱庭コンダクタ向け API 

TODO(コントリビュート募集中)
