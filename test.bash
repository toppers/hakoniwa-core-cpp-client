#!/bin/bash

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib/hakoniwa
export PYTHONPATH="/usr/local/lib/hakoniwa:$PYTHONPATH"
export PYTHONPATH="/usr/local/lib/hakoniwa/py:$PYTHONPATH"

cd test

TEST_COUNT=0
TEST_PASSED=0
TEST_FAILED=0
for item in `ls`
do
    if [ -d $item ]
    then
        bash cleanup.bash
        cd $item
        python3 -m unittest
        TEST_RESULT=$?
        cd ..
        bash cleanup.bash
        if [ $TEST_RESULT -ne 0 ]
        then
            echo "TEST FAILED: $item"
            TEST_FAILED=`expr ${TEST_FAILED} + 1`
        else
            echo "TEST PASSED: $item"
            TEST_PASSED=`expr ${TEST_PASSED} + 1`
        fi
        TEST_COUNT=`expr ${TEST_COUNT} + 1`
    fi
done

echo "TEST SUMMARY:"
echo "TOTAL : ${TEST_COUNT}"
echo "PASSED: ${TEST_PASSED}"
echo "FALIED: ${TEST_FAILED}"

if [ $TEST_FAILED -gt 0 ]
then
    exit 1
else
    exit 0
fi
