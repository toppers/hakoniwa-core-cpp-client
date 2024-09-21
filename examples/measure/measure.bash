#!/bin/bash
source ~/myenv/bin/activate  

DELTA_TIME_MSEC=10
STEP_NUM=10000
if [ $# -ne 2 ]
then
    echo "Usage: $0  <multi-num> <max_delay_time_msec>"
    exit 1
fi

MULTI_NUM=${1}
MAX_DELAY_TIME_MSEC=${2}

if [ ! -d results ]
then
    mkdir results
fi

DIR_NAME=results/result-m${MULTI_NUM}-d${MAX_DELAY_TIME_MSEC}
if [ ! -d ${DIR_NAME} ]
then
    mkdir ${DIR_NAME}
fi

python3.12 master.py ${MULTI_NUM} ${DELTA_TIME_MSEC} ${MAX_DELAY_TIME_MSEC} ${STEP_NUM}

mv asset-*.csv ${DIR_NAME}/
mv elapse.txt ${DIR_NAME}/

python3.12 graph.py --multi-num ${MULTI_NUM} --type time --csv-dir ${DIR_NAME} --stddev-only
