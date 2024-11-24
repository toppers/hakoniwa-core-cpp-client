#!/bin/bash
OS_TYPE="posix"
OS=`uname`
INSTALL_DIR=""
HAKONIWA_DIR=""
if [ "$OS" = "Linux" -o "$OS" = "Darwin"  ]
then
	SUDO=
	which sudo > /dev/null
	if [ $? -eq 0 ]
	then
		SUDO=sudo
	fi
	INSTALL_DIR="/usr/local"
	HAKONIWA_DIR="/etc/hakoniwa"
else
    OS_TYPE="win"
	SUDO=
	INSTALL_DIR="../local"
	HAKONIWA_DIR="../hakoniwa"
fi

${SUDO} mkdir -p ${INSTALL_DIR}/bin/hakoniwa
${SUDO} mkdir -p ${INSTALL_DIR}/lib/hakoniwa
${SUDO} mkdir -p ${INSTALL_DIR}/include/hakoniwa
${SUDO} mkdir -p ${HAKONIWA_DIR}

if [ "$OS" = "Linux" -o "$OS" = "Darwin"  ]
then
	${SUDO} mkdir -p /var/lib/hakoniwa/mmap
fi

${SUDO} cp core/cpp_core_config.json ${HAKONIWA_DIR}

if [ "$OS" = "Darwin"  ]
then
	${SUDO} cp cmake-build/core/sample/base-procs/hako-cmd/hako-cmd ${INSTALL_DIR}/bin/hakoniwa/
	${SUDO} cp cmake-build/src/hakoc/libhakoarun.* ${INSTALL_DIR}/lib/hakoniwa/
	${SUDO} cp cmake-build/src/hakoc/libshakoc.* ${INSTALL_DIR}/lib/hakoniwa/
	${SUDO} cp cmake-build/src/hakoc/libshakoc.dylib ${INSTALL_DIR}/lib/hakoniwa/hakoc.so
	${SUDO} cp cmake-build/src/assets/libassets.* ${INSTALL_DIR}/lib/hakoniwa/
	${SUDO} cp cmake-build/src/conductor/libconductor.* ${INSTALL_DIR}/lib/hakoniwa/
elif [ "$OS" = "Linux"  ]
then
	${SUDO} cp cmake-build/core/sample/base-procs/hako-cmd/hako-cmd ${INSTALL_DIR}/bin/hakoniwa/
	${SUDO} cp cmake-build/src/hakoc/libhakoarun.* ${INSTALL_DIR}/lib/hakoniwa/
	${SUDO} cp cmake-build/src/hakoc/libshakoc.* ${INSTALL_DIR}/lib/hakoniwa/
	${SUDO} cp cmake-build/src/hakoc/libshakoc.* ${INSTALL_DIR}/lib/hakoniwa/hakoc.so
	${SUDO} cp cmake-build/src/assets/libassets.* ${INSTALL_DIR}/lib/hakoniwa/
	${SUDO} cp cmake-build/src/conductor/libconductor.* ${INSTALL_DIR}/lib/hakoniwa/
else
	${SUDO} cp cmake-build/core/sample/base-procs/hako-cmd/Release/hako-cmd.exe ${INSTALL_DIR}/bin/hakoniwa/
	${SUDO} cp cmake-build/core/src/Release/hako.* ${INSTALL_DIR}/lib/hakoniwa/
	${SUDO} cp cmake-build/src/hakoc/Release/hakoarun.* ${INSTALL_DIR}/lib/hakoniwa/
	${SUDO} cp cmake-build/src/hakoc/Release/shakoc.* ${INSTALL_DIR}/lib/hakoniwa/
	${SUDO} cp cmake-build/src/hakoc/Release/hakoc.* ${INSTALL_DIR}/lib/hakoniwa/
	${SUDO} cp cmake-build/src/assets/Release/assets.* ${INSTALL_DIR}/lib/hakoniwa/
	${SUDO} cp cmake-build/src/conductor/Release/conductor.* ${INSTALL_DIR}/lib/hakoniwa/
fi

${SUDO} cp src/include/* ${INSTALL_DIR}/include/hakoniwa/
${SUDO} cp -rp py ${INSTALL_DIR}/lib/hakoniwa/

if [ ${OS_TYPE} = "posix" ]
then
	${SUDO} cp cmake-build/src/proxy/hako-proxy /usr/local/bin/hakoniwa/
	# Comment out if Python installation is not needed
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
if [ "$OS" = "Linux" -o "$OS" = "Darwin"  ]
then
	${SUDO} chmod -R 755 /var/lib/hakoniwa
fi