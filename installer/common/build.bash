#!/bin/bash

if [ ! -d cmake-build ]
then
    mkdir -p cmake-build
fi
source setup.bash
echo "DYLD_LIBRARY_PATH: $DYLD_LIBRARY_PATH"
cd cmake-build
cmake ..
make
