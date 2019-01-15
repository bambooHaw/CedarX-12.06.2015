#!/bin/bash
#All copyright reserved by <haoxiansen@zhitongits.com.cn> 
#Written at 20190109
#Do make sure that the shell script stands in the source codes dir

case "$1" in
*)
	TOOLCHAIN_PATH=/opt/hxs/projBoxV3/CQA83TLinux_Qt5.8.0_bv3/buildroot-2017.02.3/output/host/usr/bin
	TOOLCHAIN_HOST=arm-buildroot-linux-gnueabihf

	export CC=${TOOLCHAIN_HOST}-gcc
	export CXX=${TOOLCHAIN_HOST}-g++
	export AR=${TOOLCHAIN_HOST}-ar
	export LD=${TOOLCHAIN_HOST}-ld
	export AS=${TOOLCHAIN_HOST}-as
	export RANLIB=${TOOLCHAIN_HOST}-ranlib
	;;
esac

#comon env, allready done in the home dir's 
export PATH=$PATH:${TOOLCHAIN_PATH}
#show
${CC} -v
echo "----------------------------"
export
echo "----------------------------"

if [ -f "./Makefile" ]; then
	make clean
	make
fi

