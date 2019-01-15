CC=arm-buildroot-linux-gnueabihf-gcc
DEPEND_PATH := $(shell pwd)/../../src
APP_NAME=encoder
all:
	$(CC) encoderDemo.c \
			-L$(DEPEND_PATH)/install/lib/full-package-name/ \
			-lcedar_vencoder -lcedar_common -lcedar_base \
			-I$(DEPEND_PATH)/base/include/ \
			-I$(DEPEND_PATH)/vencoder/include/ \
			-o $(APP_NAME)

clean:
	rm -rf a.out $(APP_NAME)

