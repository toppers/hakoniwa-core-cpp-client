#!/bin/bash

if [ $# -ne 3 ]
then
    echo "Usage: $0 <asset_name> <config_path> <delta_time_msec>"
    exit 1
fi

source ~/myenv/bin/activate

ASSET_NAME=${1}
CONFIG_PATH=${2}
DELTA_TIME_MSEC=${3}

python3.12 ./asset_measure.py ${ASSET_NAME} ${CONFIG_PATH} ${DELTA_TIME_MSEC}
