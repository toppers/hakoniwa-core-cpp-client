#!/bin/bash

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib/hakoniwa
export PYTHONPATH="/usr/local/lib/hakoniwa:$PYTHONPATH"
export PYTHONPATH="/usr/local/lib/hakoniwa/py:$PYTHONPATH"

cd test

for item in `ls`
do
    if [ -d $item ]
    then
        bash cleanup.bash
        cd $item
        python3 -m unittest
        cd ..
        bash cleanup.bash
    fi
done
