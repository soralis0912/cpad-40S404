# Copyright 2009 The Android Open Source Project

LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
	mkfs.ubifs.c \
	crc16.c \
	crc32.c \
	devtable.c \
	lpt.c \
	compr.c \
	hashtable/hashtable.c \
	hashtable/hashtable_itr.c

LOCAL_CC := /usr/bin/gcc
LOCAL_CXX := /usr/bin/g++
LOCAL_CFLAGS = -O2 -Wall
LOCAL_CFLAGS+= -Wpointer-arith -Wwrite-strings -Wstrict-prototypes
LOCAL_CFLAGS+= -Wmissing-prototypes -Winline
LOCAL_LDFLAGS += -L/usr/lib/x86_64-linux-gnu
LOCAL_LDLIBS:= -lz -llzo2 -lm -luuid

LOCAL_C_INCLUDES += $(LOCAL_PATH)/
LOCAL_C_INCLUDES += /usr/include/
LOCAL_C_INCLUDES += $(LOCAL_PATH)/hashtable

LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := mkfs.ubifs

include $(BUILD_HOST_EXECUTABLE)
