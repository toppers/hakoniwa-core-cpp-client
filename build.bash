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
        make
    else
        cmake ..
        cmake --build . --target ALL_BUILD --config Release
    fi
else
	rm -rf ./cmake-build/*
fi

