#!/bin/bash

OS_TYPE="posix"
uname -a | grep Linux > /dev/null
if [ $? -ne 0 ]
then
    OS_TYPE="win"
fi

if [ $# -eq 0 ]
then
	cd cmake-build
    if [ ${OS_TYPE} = "posix" ]
    then
        cmake ..
    else
        cmake .. -G "MSYS Makefiles"
    fi
	make
else
	rm -rf ./cmake-build/*
fi

