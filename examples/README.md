# 箱庭アセットのサンプルプログラム

箱庭アセットのサンプルプログラムは、箱庭シミュレーションフレームワークを利用したアセット開発の基本を示すための一連の例です。これらのサンプルは、箱庭アセットAPIの使用方法を実演し、開発者が独自のシミュレーションアセットを効率的に構築するためのガイドとして設計されています。


## サンプルプログラムの目的
各サンプルプログラムは特定のシナリオやAPI機能に焦点を当てており、開発者は以下のような事項を学ぶことができます：

* 箱庭アセットの基本的な構成方法。
* 様々なAPI関数の使用例。
* シミュレーションの開始、実行、停止の流れ。

現在利用可能なサンプルプログラムには以下のものが含まれます：

* `hello_world`
* `pdu_communication`

## サンプルプログラムを実行するための準備

サンプルプログラムを実行するためには、以下のステップに従ってください：

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

インストール時に、/usr/local/lib/hakoniwa と /usr/local/bin/hakoniwa へのアクセス許可が必要になることがあります。この許可を与えると、箱庭のライブラリやコマンドがこれらのディレクトリに配置されます。


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

```sh
source ~/.bash_profile  # Bashの場合
source ~/.zshrc         # Zshの場合
```