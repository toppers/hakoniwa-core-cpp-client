#!/bin/bash
OS_TYPE="posix"
OS=`uname`
if [ "$OS" = "Linux" -o "$OS" = "Darwin"  ]
then
	SUDO=
	which sudo
	if [ $? -eq 0 ]
	then
		SUDO=sudo
	fi
else
    OS_TYPE="win"
	SUDO=
fi

if [ -d /usr/local/lib/hakoniwa/hako_binary ]
then
	:
else
	${SUDO} mkdir /usr/local/lib/hakoniwa/hako_binary
fi
if [ -d /usr/local/lib/hakoniwa/py/hako_binary ]
then
	:
else
	${SUDO} mkdir /usr/local/lib/hakoniwa/py/hako_binary
fi

${SUDO} cp -rp bindings/python/hako_binary/offset /usr/local/lib/hakoniwa/hako_binary/
${SUDO} cp -rp bindings/python/hako_binary/*.py /usr/local/lib/hakoniwa/py/hako_binary/
${SUDO} cp  bindings/python/hako_pdu.py /usr/local/lib/hakoniwa/py/

OS_TYPE=`uname`
if [ $OS_TYPE = "Darwin" ]
then
	${SUDO} cp cmake-build/bindings/python/libhako_asset_python.dylib /usr/local/lib/hakoniwa/py/hakopy.so
else
	${SUDO} cp cmake-build/bindings/python/libhako_asset_python.so /usr/local/lib/hakoniwa/py/hakopy.so
fi
