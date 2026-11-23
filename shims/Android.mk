LOCAL_PATH := $(call my-dir)
SHIM_COMMON := $(LOCAL_PATH)/../../smdk3470-common/libshims

include $(CLEAR_VARS)
LOCAL_SRC_FILES := shim_garda.cpp
LOCAL_SHARED_LIBRARIES := libutils libcutils liblog libbinder libmedia
LOCAL_CFLAGS := -Wno-unused-parameter
LOCAL_MODULE := libshim_garda
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_CLASS := SHARED_LIBRARIES
include $(BUILD_SHARED_LIBRARY)

include $(SHIM_COMMON)/libui/Android.mk
include $(SHIM_COMMON)/libExynosOMX/Android.mk
