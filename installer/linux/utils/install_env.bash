#!/bin/bash

${SUDO} apt update

# root ディレクトリ作成 (存在する場合は作成しない)
mkdir -p ${ROOT_DIR}/etc/hakoniwa 
mkdir -p ${ROOT_DIR}/var/lib/hakoniwa/mmap
mkdir -p ${ROOT_DIR}/var/lib/hakoniwa/config
mkdir -p ${ROOT_DIR}/usr/local/lib/hakoniwa
mkdir -p ${ROOT_DIR}/usr/local/bin/hakoniwa
mkdir -p ${ROOT_DIR}/usr/local/include/hakoniwa
mkdir -p ${ROOT_DIR}/usr/local/lib/hakoniwa/hako_binary
mkdir -p ${ROOT_DIR}/usr/local/lib/hakoniwa/py
mkdir -p ${ROOT_DIR}/usr/local/lib/hakoniwa/py/hako_binary
mkdir -p ${ROOT_DIR}/examples

# hakoniwa-ros2pduのクローン
git clone https://github.com/toppers/hakoniwa-ros2pdu.git


# config ファイルのコピー (存在確認してからコピー)
config_src="${HAKONIWA_REPO_PATH}/core/cpp_core_config.json"
config_dst="${ROOT_DIR}/etc/hakoniwa/cpp_core_config.json"
if [ -f "$config_src" ]; then
  cp "$config_src" "$config_dst"
else
  echo "Config file not found: $config_src"
  exit 1
fi

# Google Test のインストール (失敗時はエラーメッセージ表示)
if ! ${SUDO} apt install -y libgtest-dev; then
  echo "Failed to install Google Test."
  exit 1
fi

# jq のインストール (失敗時はエラーメッセージ表示)
if ! ${SUDO} apt install -y jq; then
  echo "Failed to install jq."
  exit 1
fi

# python3-dev のインストール (失敗時はエラーメッセージ表示)
if ! ${SUDO} apt install -y python3-dev; then
  echo "Failed to install python3-dev."
  exit 1
fi

if ! ${SUDO} apt install -y python3; then
  echo "Failed to install python3."
  exit 1
fi


if ! ${SUDO} apt install -y python3-pip; then
  echo "Failed to install python3-pip."
  exit 1
fi

if ! ${SUDO} apt install -y cmake; then
  echo "Failed to install cmake."
  exit 1
fi

pip3 install -r ${HAKONIWA_REPO_PATH}/requirements.txt

