#!/bin/bash

if [ ! -d /usr/local/bin/hakoniwa ]
then
	sudo mkdir /usr/local/bin/hakoniwa
fi
if [ ! -d /usr/local/lib/hakoniwa ]
then
	sudo mkdir /usr/local/lib/hakoniwa
fi

sudo cp cmake-build/core/sample/base-procs/hako-cmd/hako-cmd /usr/local/bin/hakoniwa/
sudo cp cmake-build/src/proxy/hako-proxy /usr/local/bin/hakoniwa/
sudo cp cmake-build/src/hakoc/libhakoarun.* /usr/local/lib/hakoniwa/
sudo cp cmake-build/src/hakoc/libshakoc.* /usr/local/lib/hakoniwa/
sudo cp cmake-build/src/hakoc/libshakoc.* /usr/local/lib/hakoniwa/hakoc.so
sudo cp cmake-build/src/assets/libassets.* /usr/local/lib/hakoniwa/

sudo cp -rp py /usr/local/lib/hakoniwa/
bash bindings/python/install.bash
