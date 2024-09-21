#!/bin/bash

if [ $# -ne 4 ]
then
    echo "Usage: $0 <asset_name> <config_path> <delta_time_msec> <step-num>"
    exit 1
fi

source ~/myenv/bin/activate

ASSET_NAME=${1}
CONFIG_PATH=${2}
DELTA_TIME_MSEC=${3}
STEP_NUM=${4}

# execを使ってシェルをPythonプロセスに置き換える
exec python3.12 ./asset_measure.py ${ASSET_NAME} ${CONFIG_PATH} ${DELTA_TIME_MSEC} ${STEP_NUM}
