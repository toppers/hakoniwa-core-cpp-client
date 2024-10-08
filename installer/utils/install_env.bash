#!/bin/bash

if [ $OS_TYPE = "linux" ]
then
  ${SUDO} apt update
fi

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
mkdir -p examples

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

bash ${HAKONIWA_REPO_PATH}/installer/${OS_TYPE}/install_env.bash

pip3 install -r ${HAKONIWA_REPO_PATH}/bindings/python/requirements.txt

