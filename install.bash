#!/bin/bash
OS_TYPE="posix"
OS=`uname`
if [ "$OS" = "Linux" -o "$OS" = "Darwin"  ]
then
	:
	SUDO=sudo
else
    OS_TYPE="win"
	SUDO=
fi
if [ ! -d /usr/local/bin ]
then
	${SUDO} mkdir /usr/local/bin
fi
if [ ! -d /usr/local/lib ]
then
	${SUDO} mkdir /usr/local/lib
fi

if [ ! -d /usr/local/bin/hakoniwa ]
then
	${SUDO} mkdir /usr/local/bin/hakoniwa
fi
if [ ! -d /usr/local/lib/hakoniwa ]
then
	${SUDO} mkdir /usr/local/lib/hakoniwa
fi
if [ ! -d /etc/hakoniwa ]
then
	${SUDO} mkdir /etc/hakoniwa
fi
if [ ! -d /var/lib/hakoniwa ]
then
	${SUDO} mkdir /var/lib/hakoniwa
fi
if [ ! -d /var/lib/hakoniwa/mmap ]
then
	${SUDO} mkdir /var/lib/hakoniwa/mmap
fi
${SUDO} cp core/cpp_core_config.json /etc/hakoniwa/

${SUDO} cp cmake-build/core/sample/base-procs/hako-cmd/hako-cmd /usr/local/bin/hakoniwa/
${SUDO} cp cmake-build/src/hakoc/libhakoarun.* /usr/local/lib/hakoniwa/
${SUDO} cp cmake-build/src/hakoc/libshakoc.* /usr/local/lib/hakoniwa/
${SUDO} cp cmake-build/src/hakoc/libshakoc.* /usr/local/lib/hakoniwa/hakoc.so
${SUDO} cp cmake-build/src/assets/libassets.* /usr/local/lib/hakoniwa/
${SUDO} cp cmake-build/src/conductor/libconductor.* /usr/local/lib/hakoniwa/

${SUDO} cp -rp py /usr/local/lib/hakoniwa/
if [ ${OS_TYPE} = "posix" ]
then
	${SUDO} cp cmake-build/src/proxy/hako-proxy /usr/local/bin/hakoniwa/
	bash bindings/python/install.bash
fi

# ディレクトリの所有者をインストールユーザーに変更
${SUDO} chown -R $USER /var/lib/hakoniwa

# ディレクトリのパーミッションを適切に設定
${SUDO} chmod -R 755 /var/lib/hakoniwa
