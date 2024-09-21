#!/bin/bash
SAMPLE_NUM=10

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

# ループの回数を SAMPLE_NUM で制御
for index in $(seq 0 $(($SAMPLE_NUM - 1)))
do
    do_test ${index}
done
