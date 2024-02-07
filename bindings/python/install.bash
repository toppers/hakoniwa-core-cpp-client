#!/bin/bash

if [ -d /usr/local/lib/hakoniwa/hako_binary ]
then
	:
else
	sudo mkdir /usr/local/lib/hakoniwa/hako_binary
fi
if [ -d /usr/local/lib/hakoniwa/py/hako_binary ]
then
	:
else
	sudo mkdir /usr/local/lib/hakoniwa/py/hako_binary
fi

sudo cp -rp bindings/python/hako_binary/offset /usr/local/lib/hakoniwa/hako_binary/
sudo cp -rp bindings/python/hako_binary/*.py /usr/local/lib/hakoniwa/py/hako_binary/
sudo cp  bindings/python/hako_pdu.py /usr/local/lib/hakoniwa/py/

OS_TYPE=`uname`
if [ $OS_TYPE = "Darwin" ]
then
	sudo cp cmake-build/bindings/python/libhako_asset_python.dylib /usr/local/lib/hakoniwa/py/hako_asset.so
else
	sudo cp cmake-build/bindings/python/libhako_asset_python.so /usr/local/lib/hakoniwa/py/hako_asset.so
fi
