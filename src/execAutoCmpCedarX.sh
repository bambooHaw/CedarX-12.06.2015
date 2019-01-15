#!/bin/bash
#All copyright reserved by <haoxiansen@zhitongits.com.cn> 
#Written at 20181101
#Do make sure that the shell script stands in the source codes dir

function sdkQt5_8Env()
{
	TOOLCHAIN_PATH=/opt/hxs/projBoxV3/CQA83TLinux_Qt5.8.0_bv3/buildroot-2017.02.3/output/host/usr/bin
	TOOLCHAIN_HOST=arm-buildroot-linux-gnueabihf
	export PATH=$PATH:${TOOLCHAIN_PATH}
}

function crossCompileEnv()
{
	export CC=${TOOLCHAIN_HOST}-gcc
	export CXX=${TOOLCHAIN_HOST}-g++
	export AR=${TOOLCHAIN_HOST}-ar
	export LD=${TOOLCHAIN_HOST}-ld
	export AS=${TOOLCHAIN_HOST}-as
	export RANLIB=${TOOLCHAIN_HOST}-ranlib
}
function privateCompileEnv()
{
	#just do this usrs' env for ion
	export LDFLAGS=-L$(pwd)/ion
	export LIBS=-lcedar_ion
}


case "$1" in
*)
	sdkQt5_8Env
	#comon env
	crossCompileEnv
	#private env
 	privateCompileEnv
	;;
esac


#show
${CC} -v
echo "----------------------------"
export
echo "----------------------------"

#exec
./bootstrap

./configure --host=${TOOLCHAIN_HOST}  --prefix=$(pwd)/install

if [ -f "./Makefile" ]; then
	make
	make install
	#cp AllWinner's no GPL/BSD libs
	cp ../binary/* ./install/lib/full-package-name/ -v

	mkdir -p /opt/hxs/share/full-package-name
	cp ./install/lib/full-package-name/* /opt/hxs/share/full-package-name/ -v
fi

