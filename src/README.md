### modified points

##### 20190111 

1. modified execAutoCmpCedarx.sh for compile the proj with A83T

2. modified src/base/include/Cdxconfig.h for using ION

```
#define CONFIG_MEMORY_DRIVER    OPTION_MEMORY_DRIVER_ION
```

3. add ion file
1) add src/common/ ion_alloc.c and include/ion*.h

2) modified src/Makefile.am
```
include $(top_srcdir)/Makefile.am.inc

pkglib_LTLIBRARIES = libcedar_common.la
  

libcedar_common_la_SOURCES =ve.c \
						   ion_alloc.c
 
libcedar_common_la_CFLAGS = $(CEDAR_CFLAGS)
LOCAL_INCLUDE = -I./include \
                -I$(top_srcdir)/base/include

libcedar_common_la_CFLAGS += $(LOCAL_INCLUDE)

libcedar_common_la_LIBADD = $(top_srcdir)/base/libcedar_base.la 

libcedar_common_la_LDFLAGS = $(CEDAR_LDFLAGS)

3) modified 

```
