#!/bin/bash

if [ -z ${HAKONIWA_REPO_PATH} ]
then
    echo "ERORR: HAKONIWA_REPO_PATH is not defined."
    exit 1
fi

CURR_DIR=`pwd`
HAKO_CORE_BUILD_DIR=${HAKONIWA_REPO_PATH}/cmake-build

# install hakoniwa
cp ${HAKO_CORE_BUILD_DIR}/core/sample/base-procs/hako-cmd/hako-cmd ${ROOT_DIR}/usr/local/bin/hakoniwa/
cp ${HAKO_CORE_BUILD_DIR}/src/proxy/hako-proxy ${ROOT_DIR}/usr/local/bin/hakoniwa/
cp ${HAKO_CORE_BUILD_DIR}/src/hakoc/libhakoarun.* ${ROOT_DIR}/usr/local/lib/hakoniwa/
cp ${HAKO_CORE_BUILD_DIR}/src/hakoc/libshakoc.* ${ROOT_DIR}/usr/local/lib/hakoniwa/
cp ${HAKO_CORE_BUILD_DIR}/src/hakoc/libshakoc.* ${ROOT_DIR}/usr/local/lib/hakoniwa/hakoc.so
cp ${HAKO_CORE_BUILD_DIR}/src/assets/libassets.* ${ROOT_DIR}/usr/local/lib/hakoniwa/
cp ${HAKO_CORE_BUILD_DIR}/src/conductor/libconductor.* ${ROOT_DIR}/usr/local/lib/hakoniwa/
cp ${HAKO_CORE_BUILD_DIR}/bindings/python/libhako_asset_python.${SHARELIB_EXT} ${ROOT_DIR}/usr/local/lib/hakoniwa/py/hakopy.so

cp ${HAKONIWA_REPO_PATH}/src/include/*.h ${ROOT_DIR}/usr/local/include/hakoniwa/
cp -rp ${HAKONIWA_REPO_PATH}/bindings/python/hako_binary/offset ${ROOT_DIR}/usr/local/lib/hakoniwa/hako_binary/
cp -rp ${HAKONIWA_REPO_PATH}/bindings/python/hako_binary/*.py ${ROOT_DIR}/usr/local/lib/hakoniwa/py/hako_binary/
cp     ${HAKONIWA_REPO_PATH}/bindings/python/hako_pdu.py ${ROOT_DIR}/usr/local/lib/hakoniwa/py/

# ディレクトリのパーミッションを適切に設定
chmod -R 755 ${ROOT_DIR}/var/lib/hakoniwa

# install pdu
cp -rp hakoniwa-ros2pdu/pdu/offset  ${ROOT_DIR}/usr/local/lib/hakoniwa/hako_binary/

# install drone_api
cp -rp ${HAKONIWA_REPO_PATH}/examples/* ${CURR_DIR}/examples/
