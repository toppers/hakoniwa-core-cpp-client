#!/bin/bash

if [ -z ${HAKONIWA_REPO_PATH} ]
then
    echo "ERORR: HAKONIWA_REPO_PATH is not defined."
    exit 1
fi

# build hakoniwa
CURR_DIR=`pwd`
cd ${HAKONIWA_REPO_PATH}

bash build.bash

cd ${CURR_DIR}
