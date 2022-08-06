#!/bin/bash

if [ $# -ne 2 ]
then
	echo "Usage: $0 <search path file> <ros_msg_pkg/ros_msg_type>"
	exit 1
fi

SEARCH_PATH_FILE=${1}
ROS_MSG_PKG_TYPE=${2}

bash parse_msg_recursive.bash ${SEARCH_PATH_FILE} ${ROS_MSG_PKG_TYPE} | sort | uniq 


