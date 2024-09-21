#!/bin/bash
source ~/myenv/bin/activate  

DELTA_TIME_MSEC=10
STEP_NUM=10000
if [ $# -ne 3 ]
then
    echo "Usage: $0  <multi-num> <max_delay_time_msec> <index>"
    exit 1
fi

if [ -d ~/myenv/bin/activate ]
then
    source ~/myenv/bin/activate
    PYTHON_BIN=python3.12
else
    PYTHON_BIN=python
fi

MULTI_NUM=${1}
MAX_DELAY_TIME_MSEC=${2}
INDEX=${3}

if [ ! -d results ]
then
    mkdir results
fi

if [ ! -d results/${INDEX} ]
then
    mkdir results/${INDEX}
fi

DIR_NAME=results/${INDEX}/result-m${MULTI_NUM}-d${MAX_DELAY_TIME_MSEC}
if [ ! -d ${DIR_NAME} ]
then
    mkdir ${DIR_NAME}
fi

${PYTHON_BIN} master.py ${MULTI_NUM} ${DELTA_TIME_MSEC} ${MAX_DELAY_TIME_MSEC} ${STEP_NUM}

mv asset-*.csv ${DIR_NAME}/
mv elapse.txt ${DIR_NAME}/

${PYTHON_BIN} graph.py --multi-num ${MULTI_NUM} --type time --csv-dir ${DIR_NAME} --stddev-only
