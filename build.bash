#!/bin/bash

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
        cmake ..
    else
        cmake .. -G "Unix Makefiles" -D WIN32=true
    fi
	make
else
	rm -rf ./cmake-build/*
fi

