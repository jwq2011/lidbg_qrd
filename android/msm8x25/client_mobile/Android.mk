
LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../inc
LOCAL_MODULE_PATH := $(LOCAL_PATH)/../out
LOCAL_SHARED_LIBRARIES += libcutils libutils
LOCAL_LDLIBS += -llog
LOCAL_SRC_FILES:= \/par    client_mobile.c
include $(LOCAL_PATH)/../../../build_cfg.mk
LOCAL_MODULE_TAGS:= optional
LOCAL_MODULE := client_mobile
include $(BUILD_EXECUTABLE)
