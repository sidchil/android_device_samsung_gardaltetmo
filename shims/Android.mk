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

include $(CLEAR_VARS)
LOCAL_SRC_FILES := mali_pthread.c
LOCAL_SHARED_LIBRARIES := liblog
LOCAL_MODULE := libshim_mali
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_CLASS := SHARED_LIBRARIES
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_SRC_FILES := audio_shim.cpp
LOCAL_SHARED_LIBRARIES := libutils libmedia_helper
LOCAL_HEADER_LIBRARIES := libmedia_headers libaudioclient_headers
LOCAL_MODULE := libshim_audio
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_CLASS := SHARED_LIBRARIES
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_SRC_FILES := camera_shim.cpp
LOCAL_SHARED_LIBRARIES := libutils libcamera_client
LOCAL_MODULE := libshim_camera
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_CLASS := SHARED_LIBRARIES
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_SRC_FILES := omx_shim.cpp
LOCAL_SHARED_LIBRARIES := libui liblog
LOCAL_MODULE := libshim_omx
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_CLASS := SHARED_LIBRARIES
include $(BUILD_SHARED_LIBRARY)

include $(SHIM_COMMON)/libui/Android.mk
include $(SHIM_COMMON)/libExynosOMX/Android.mk
