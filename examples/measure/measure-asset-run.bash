#!/bin/bash

if [ $# -ne 4 ]
then
    echo "Usage: $0 <asset_name> <config_path> <delta_time_msec> <step-num>"
    exit 1
fi

if [ -d ~/myenv/bin/activate ]
then
    source ~/myenv/bin/activate
    PYTHON_BIN=python3.12
else
    PYTHON_BIN=python
fi

ASSET_NAME=${1}
CONFIG_PATH=${2}
DELTA_TIME_MSEC=${3}
STEP_NUM=${4}

# execを使ってシェルをPythonプロセスに置き換える
exec ${PYTHON_BIN} ./asset_measure.py ${ASSET_NAME} ${CONFIG_PATH} ${DELTA_TIME_MSEC} ${STEP_NUM}
