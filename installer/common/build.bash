#!/bin/bash

if [ ! -d cmake-build ]
then
    mkdir -p cmake-build
fi
source setup.bash
if [ `uname` = "Darwin" ]
then
    echo "DYLD_LIBRARY_PATH: $DYLD_LIBRARY_PATH"
else
    echo "LD_LIBRARY_PATH: $LD_LIBRARY_PATH"
fi
cd cmake-build
cmake ..
make
