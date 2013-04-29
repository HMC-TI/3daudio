LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := convolve
LOCAL_C_INCLUDES := convolve.c
include $(BUILD_SHARED_LIBRARY)