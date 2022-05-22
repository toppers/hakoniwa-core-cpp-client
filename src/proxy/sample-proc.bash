#!/bin/bash

echo "ARG1=$1"
echo "ARG1=$2"

while [ 1 ]
do
    read -p "ok? (y/N): " yn
    echo "HELLO: `date` $yn"
    if [ $yn = "y" ]
    then
        echo "OK!!"
        sleep 5        
    else
        echo "Bye!!"
        exit 1
    fi
done
