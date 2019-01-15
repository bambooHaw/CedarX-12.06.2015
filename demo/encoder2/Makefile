CC=arm-buildroot-linux-gnueabihf-gcc
DEPEND_PATH := $(shell pwd)/../../src
APP_NAME=encoderApp
all:
	$(CC) encoderDemo.c \
			-L$(DEPEND_PATH)/install/lib/full-package-name/ \
			-lcedar_vencoder -lcedar_common -lcedar_base -lcedar_ion\
			-I$(DEPEND_PATH)/base/include/ \
			-I$(DEPEND_PATH)/vencoder/include/ \
			-I$(DEPEND_PATH)/ion/include/ \
			-o $(APP_NAME)

clean:
	rm -rf a.out $(APP_NAME)

