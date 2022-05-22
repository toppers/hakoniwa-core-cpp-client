#!/bin/bash

if [ $# -ne 1 -a $# -ne 2 ]
then
	echo "Usage: $0 <build_target> [clean|all]"
	echo "<build_target>:"
	ls  -l .. | grep -v build | grep -v total | awk '{print $NF}'
	exit 1
fi

TARGET=${1}

if [ $# -eq 2 ]
then
	OPT=${2}
else
	OPT="all"
fi

if [ ${OPT} = "clean" ]
then
	rm -rf ${TARGET}
	exit 0
fi

if [ -d ${TARGET} ]
then
	:
else
	mkdir ${TARGET}
fi

cd ${TARGET}
cmake ../../${TARGET}
make
