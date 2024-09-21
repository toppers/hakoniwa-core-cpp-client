#!/bin/bash

for multi in 1 2 4 8 10
do
	for mdelay in 10 20 40 80 100
	do
		echo "DO TEST: MULTI_NUM=${multi} MAX_DELAY=${mdelay}$"
		bash measure.bash ${multi} ${mdelay}
	done
done

