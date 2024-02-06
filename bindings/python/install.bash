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

sudo cp cmake-build/bindings/python/libhako_asset_python.* /usr/local/lib/hakoniwa/py/
