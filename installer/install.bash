#!/bin/bash

if [ `uname` = "Darwin" ]
then
    export OS_TYPE="mac"
    export SHARELIB_EXT="dylib"
else
    export OS_TYPE="linux"
    export SHARELIB_EXT="so"
fi

script_path=$(realpath "$0")
echo "Script path: $script_path"

which sudo
if [ $? -eq 0 ]
then
    export SUDO=`which sudo`
else
    export SUDO=
fi

DIR_PATH=`(cd "$(dirname $script_path)" && pwd)`
UTILS_PATH=${DIR_PATH}/${OS_TYPE}/utils
export HAKONIWA_REPO_PATH=$(realpath "${DIR_PATH}/..")

# ROOT_DIR ディレクトリは好みで変えてください
CURR_DIR=`pwd`
export ROOT_DIR=${CURR_DIR}/root

bash ${DIR_PATH}/utils/install_env.bash
if [ $? -ne 0 ]
then
    echo "ERROR: instal_env.bash error"
    exit 1
fi
bash ${DIR_PATH}/utils/build_hako.bash
if [ $? -ne 0 ]
then
    echo "ERROR: build_hako.bash error"
    exit 1
fi
bash ${DIR_PATH}/utils/install_hako.bash
if [ $? -ne 0 ]
then
    echo "ERROR: instal_hako.bash error"
    exit 1
fi

#cp -rp ${DIR_PATH}/config/* ${ROOT_DIR}/var/lib/hakoniwa/config/

export default_core_mmap_path=${ROOT_DIR}/var/lib/hakoniwa/mmap
export config_file=${ROOT_DIR}/etc/hakoniwa/cpp_core_config.json
bash ${DIR_PATH}/utils/hako-mmap-set.bash -p ${ROOT_DIR}/var/lib/hakoniwa/mmap

bash ${DIR_PATH}/utils/create_setup.bash

cp ${DIR_PATH}/common/* .