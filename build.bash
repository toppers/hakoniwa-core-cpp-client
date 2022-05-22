#!/bin/bash

if [ $# -eq 0 ]
then
	cd cmake-build
	cmake ..
	make
else
	rm -rf ./cmake-build/*
fi

