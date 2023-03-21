#!/bin/bash

if [ -d /usr/local/bin/hakoniwa ]
then
	:
else
	sudo mkdir /usr/local/bin/hakoniwa
fi

sudo cp cmake-build/core/sample/base-procs/hako-cmd/hako-cmd /usr/local/bin/hakoniwa/
sudo cp cmake-build/src/hakoc/libshakoc.* /usr/local/lib/hakoniwa/
sudo cp cmake-build/src/hakoc/libshakoc.* /usr/local/lib/hakoniwa/hakoc.so

if [ -d /usr/local/lib/hakoniwa/hako_binary ]
then
	:
else
	sudo mkdir /usr/local/lib/hakoniwa/hako_binary
fi

sudo cp -rp py/hako_binary/offset /usr/local/lib/hakoniwa/hako_binary/
sudo cp -rp py /usr/local/lib/hakoniwa/
