#!/bin/bash

function do_test()
{
	index=${1}
	# Ensure results directory exists
	mkdir -p results/${index}

	for multi in 1 2 4 8 10
	do
		for mdelay in 10 20 40 80 100
		do
			echo "DO TEST: MULTI_NUM=${multi} MAX_DELAY=${mdelay}"
			bash measure.bash ${multi} ${mdelay} ${index}
		done
	done

	python calc-result.py --index ${index}
}

for index in 0 1
do
	do_test ${index}
done
