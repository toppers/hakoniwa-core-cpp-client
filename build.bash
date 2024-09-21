#!/bin/bash

# 環境変数をチェックして ENABLE_HAKO_TIME_MEASURE オプションを適用
if [ ! -z "${ENABLE_HAKO_TIME_MEASURE}" ] && [ "${ENABLE_HAKO_TIME_MEASURE}" = "ON" ]; then
    ENABLE_HAKO_TIME_MEASURE_FLAG="-DENABLE_HAKO_TIME_MEASURE=ON"
    echo "ENABLE_HAKO_TIME_MEASURE is ON"
else
    ENABLE_HAKO_TIME_MEASURE_FLAG=""
fi

DEFAULT_HAKO_ASSET_NUM=4
if [ ! -z "${ASSET_NUM}" ] && [ ${ASSET_NUM} -gt ${DEFAULT_HAKO_ASSET_NUM} ]; then
    :
else
    ASSET_NUM=${DEFAULT_HAKO_ASSET_NUM}
fi
echo "ASSET_NUM is ${ASSET_NUM}"

OS_TYPE="posix"
OS=`uname`
if [ "$OS" = "Linux" -o "$OS" = "Darwin"  ]
then
	:
else
    OS_TYPE="win"
fi

if [ $# -eq 0 ]
then
    cd cmake-build
    if [ ${OS_TYPE} = "posix" ]
    then
        cmake .. $ENABLE_HAKO_TIME_MEASURE_FLAG -DHAKO_DATA_MAX_ASSET_NUM=${ASSET_NUM}
        make
    else
        cmake ..
        cmake --build . --target ALL_BUILD --config Release
    fi
else
	rm -rf ./cmake-build/*
fi

