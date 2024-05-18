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

${SUDO} mkdir -p /usr/local/bin/hakoniwa
${SUDO} mkdir -p /usr/local/lib/hakoniwa
${SUDO} mkdir -p /usr/local/include/hakoniwa
${SUDO} mkdir -p /etc/hakoniwa
${SUDO} mkdir -p /var/lib/hakoniwa/mmap

${SUDO} cp core/cpp_core_config.json /etc/hakoniwa/

${SUDO} cp cmake-build/core/sample/base-procs/hako-cmd/hako-cmd /usr/local/bin/hakoniwa/
${SUDO} cp cmake-build/src/hakoc/libhakoarun.* /usr/local/lib/hakoniwa/
${SUDO} cp cmake-build/src/hakoc/libshakoc.* /usr/local/lib/hakoniwa/
if [ "$OS" = "Darwin"  ]
then
	${SUDO} cp cmake-build/src/hakoc/libshakoc.dylib /usr/local/lib/hakoniwa/hakoc.so
else
	${SUDO} cp cmake-build/src/hakoc/libshakoc.* /usr/local/lib/hakoniwa/hakoc.so
fi
${SUDO} cp cmake-build/src/assets/libassets.* /usr/local/lib/hakoniwa/
${SUDO} cp cmake-build/src/conductor/libconductor.* /usr/local/lib/hakoniwa/

${SUDO} cp src/include/*.h /usr/local/include/hakoniwa/
${SUDO} cp -rp py /usr/local/lib/hakoniwa/
if [ ${OS_TYPE} = "posix" ]
then
	${SUDO} cp cmake-build/src/proxy/hako-proxy /usr/local/bin/hakoniwa/
	bash bindings/python/install.bash
fi

if [ -z $USER ]
then
	:
else
	# ディレクトリの所有者をインストールユーザーに変更
	${SUDO} chown -R $USER /var/lib/hakoniwa
fi

# ディレクトリのパーミッションを適切に設定
${SUDO} chmod -R 755 /var/lib/hakoniwa
