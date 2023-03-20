#!/bin/bash

IDS=`ipcs -m | grep ff | awk '{print $2}'`
for id in $IDS
do
	echo "remove shared memory $id"
	ipcrm -m $id
done

IDS=`ipcs -m | grep 100 | awk '{print $2}'`
for id in $IDS
do
	echo "remove shared memory $id"
	ipcrm -m $id
done


IDS=`ipcs -s | grep ff | awk '{print $2}'`
for id in $IDS
do
	echo "remove semaphore $id"
	ipcrm -s $id
done

IDS=`ipcs -s | grep 100 | awk '{print $2}'`
for id in $IDS
do
	echo "remove semaphore $id"
	ipcrm -s $id
done
