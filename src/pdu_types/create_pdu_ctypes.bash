#!/bin/bash

if [ $# -ne 2 ]
then
    echo "Usage: $0 <search_path> <ros_msg_pkg/ros_msg_type>"
    exit 1
fi

CURR_DIR=`pwd`
TOOL_DIR=tools
JSON_DIR=${TOOL_DIR}/ros_json
TYPES_DIR=${TOOL_DIR}/ros_types

SEARCH_PATH=${CURR_DIR}/${1}
PKG_MSG=${2}

#find dependent types
cd ${TYPES_DIR}
bash find_deptypes.bash ${SEARCH_PATH} ${PKG_MSG} > ${CURR_DIR}/dep_types.txt
cd ${CURR_DIR}

#create json file
cd ${JSON_DIR}
bash convert_rosmsg2json.bash ${SEARCH_PATH} ${PKG_MSG} ${CURR_DIR}/out_json
cd ${CURR_DIR}

#convert json file to c type header
python3 tools/template_engine/generate.py ./template/pdu_ctypes_h.tpl ./out_json/${PKG_MSG}.json  

#rm -rf ${CURR_DIR}/out_json
#rm -rf dep_types.txt