# 目次

1. [これは何？](#これは何)
2. [サポート対象環境](#サポート対象環境)
3. [事前インストールが必要なもの](#事前インストールが必要なもの)
   - [Git](#git)
   - [Docker](#docker)
   - [pyenv/Python3.12](#pyenv-python312)
4. [Dockerのインストール方法](#dockerのインストール方法)
   - [Mac 向けの手順](#mac-向けの手順)
   - [Ubuntu 向けの手順](#ubuntu-向けの手順)
5. [Pythonのインストール手順](#pythonのインストール手順)
   - [Mac 向けの手順](#mac-向けの手順-1)
   - [Ubuntu 向けの手順](#ubuntu-向けの手順-1)
6. [箱庭コア機能のインストール手順](#箱庭コア機能のインストール手順)
7. [サンプルプログラムのビルド方法](#サンプルプログラムのビルド方法)
8. [サンプルプログラムの実行方法](#サンプルプログラムの実行方法)
9. [箱庭PDUの作成方法](#箱庭pduの作成方法)
   - [事前準備](#事前準備)
   - [PDU作成方法](#pdu作成方法)

# これは何？

ローカルマシンに直接箱庭コア機能およびPDU作成ツールをインストールするための環境です。


# サポート対象環境

- Ubuntu 22.0.4(WSL2含む)
- MacOS

# 事前インストールが必要なもの

- Git
- Docker
- pyenv/Python3.12

## Dockerのインストール方法

### Mac 向けの手順

1. **Docker Desktop のダウンロード**

   Docker の Mac 用デスクトップアプリケーションである Docker Desktop を公式サイトからダウンロードします。

   - [Docker Desktop for Mac のダウンロードページ](https://www.docker.com/products/docker-desktop)

2. **インストーラの実行**

   ダウンロードが完了したら、ダウンロードされた `.dmg` ファイルを開いてインストーラを実行します。

3. **Docker Desktop のインストール**

   インストーラに従い、Docker Desktop をアプリケーションフォルダにドラッグしてインストールします。

4. **Docker Desktop の起動**

   インストール後、アプリケーションフォルダから Docker Desktop を起動します。初回起動時にいくつかのセットアップが行われますが、指示に従ってセットアップを完了してください。

5. **動作確認**

   Docker Desktop が正常に動作していることを確認するために、以下のコマンドをターミナルで実行します。

   ```bash
   docker --version
   ```

   バージョン情報が表示されれば、インストールは成功です。

---

### Ubuntu 向けの手順

1. **古いバージョンの削除 (既にインストール済みの場合)**

   既に Docker がインストールされている場合、まず古いバージョンを削除します。

   ```bash
   sudo apt remove docker docker-engine docker.io containerd runc
   ```

2. **必要なパッケージのインストール**

   Docker のリポジトリをセットアップするために必要なパッケージをインストールします。

   ```bash
   sudo apt update
   sudo apt install -y ca-certificates curl gnupg lsb-release
   ```

3. **Docker GPG キーの追加**

   Docker の公式 GPG キーをシステムに追加します。

   ```bash
   sudo mkdir -p /etc/apt/keyrings
   curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo gpg --dearmor -o /etc/apt/keyrings/docker.gpg
   ```

4. **Docker リポジトリの追加**

   Docker のリポジトリを設定します。

   ```bash
   echo \
     "deb [arch=$(dpkg --print-architecture) signed-by=/etc/apt/keyrings/docker.gpg] https://download.docker.com/linux/ubuntu \
     $(lsb_release -cs) stable" | sudo tee /etc/apt/sources.list.d/docker.list > /dev/null
   ```

5. **Docker のインストール**

   パッケージ情報を更新し、Docker をインストールします。

   ```bash
   sudo apt update
   sudo apt install -y docker-ce docker-ce-cli containerd.io docker-buildx-plugin docker-compose-plugin
   ```

6. **Docker の動作確認**

   Docker が正しくインストールされたことを確認するために、以下のコマンドを実行します。

   ```bash
   docker --version
   ```

   バージョン情報が表示されればインストールは成功です。

7. **（オプション）Docker を管理者権限なしで使用可能にする**

   Docker コマンドを実行する際に `sudo` を必要としないようにするためには、現在のユーザを `docker` グループに追加します。

   ```bash
   sudo usermod -aG docker $USER
   ```

   その後、再ログインまたは次のコマンドで変更を適用します。

   ```bash
   newgrp docker
   ```


## Pythonのインストール手順

### Mac 向けの手順

1. **Homebrew のインストール (未インストールの場合)**
   - Mac のパッケージ管理ツール `Homebrew` をインストールします。

   ```bash
   /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
   ```

2. **必要な依存パッケージのインストール**

   Python のインストールに必要な依存パッケージを `Homebrew` でインストールします。

   ```bash
   brew install openssl readline sqlite3 xz zlib
   ```

3. **pyenv のインストール**

   `pyenv` をインストールします。

   ```bash
   brew install pyenv
   ```

4. **シェル設定ファイルの更新**

   `pyenv` を有効にするため、シェル設定ファイル (`~/.bash_profile`, `~/.zshrc` など) に以下を追加します。

   ```bash
   export PYENV_ROOT="$HOME/.pyenv"
   export PATH="$PYENV_ROOT/bin:$PATH"
   eval "$(pyenv init --path)"
   ```

   設定を適用します。

   ```bash
   source ~/.bash_profile  # または `source ~/.zshrc`
   ```

5. **Python 3.12 のインストール**

   `pyenv` を使って Python 3.12 をインストールします。

   ```bash
   pyenv install 3.12.0
   ```

6. **Python 3.12 の使用**

   インストールした Python 3.12 をグローバルまたはローカルで使用するように設定します。

   - グローバルに設定する場合:

     ```bash
     pyenv global 3.12.0
     ```

   - ローカル (現在のプロジェクトフォルダ内) に設定する場合:

     ```bash
     pyenv local 3.12.0
     ```

---

### Ubuntu 向けの手順

1. **必要な依存パッケージのインストール**

   Python のビルドに必要な依存パッケージをインストールします。

   ```bash
   sudo apt update
   sudo apt install -y make build-essential libssl-dev zlib1g-dev libbz2-dev \
       libreadline-dev libsqlite3-dev wget curl llvm libncurses5-dev libncursesw5-dev \
       xz-utils tk-dev libffi-dev liblzma-dev python-openssl git
   ```

2. **pyenv のインストール**

   `pyenv` をインストールします。

   ```bash
   curl https://pyenv.run | bash
   ```

3. **シェル設定ファイルの更新**

   `pyenv` を有効にするため、シェル設定ファイル (`~/.bashrc`, `~/.zshrc` など) に以下を追加します。

   ```bash
   export PYENV_ROOT="$HOME/.pyenv"
   export PATH="$PYENV_ROOT/bin:$PATH"
   eval "$(pyenv init --path)"
   ```

   設定を適用します。

   ```bash
   source ~/.bashrc  # または `source ~/.zshrc`
   ```

4. **Python 3.12 のインストール**

   `pyenv` を使って Python 3.12 をインストールします。

   ```bash
   pyenv install 3.12.0
   ```

5. **Python 3.12 の使用**

   インストールした Python 3.12 をグローバルまたはローカルで使用するように設定します。

   - グローバルに設定する場合:

     ```bash
     pyenv global 3.12.0
     ```

   - ローカル (現在のプロジェクトフォルダ内) に設定する場合:

     ```bash
     pyenv local 3.12.0
     ```

# 箱庭コア機能のインストール手順

```bash
git clone --recursive https://github.com/toppers/hakoniwa-core-cpp-client.git
```

```bash
cd hakoniwa-core-cpp-client
```

```bash
mkdir workspace
```

```bash
cd workspace
```

```bash
bash ../installer/install.bash
```

```bash
source setup.bash
```

# サンプルプログラムのビルド方法

```bash
bash build.bash
```

成功すると、cmake-buildディレクトリができます。

# サンプルプログラムの実行方法

こちらを参照ください。

https://github.com/toppers/hakoniwa-core-cpp-client/tree/main/examples

# 箱庭PDUの作成方法

## 事前準備

```bash
cd hakoniwa-ros2pdu
```

```bash
bash docker/pull_image.bash
```

```bash
docker images
```

```
REPOSITORY                               TAG            IMAGE ID       CREATED         SIZE
toppersjp/hakoniwa-ros2pdu-dev           v1.1.0         fc4178775a2a   6 months ago    3.79GB
```

## PDU作成方法

こちらを参照ください。

https://github.com/toppers/hakoniwa-ros2pdu