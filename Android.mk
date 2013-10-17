  LOCAL_PATH:= $(call my-dir)

  include $(CLEAR_VARS)

  LOCAL_SRC_FILES := last_ssu.c
 
  #LOCAL_C_INCLUDES := $(LOCAL_PATH)/shalibs

  LOCAL_LDLIBS := $(LOCAL_PATH)/lib_openssl/libcrypto.so

  LOCAL_SHARED_LIBRARIES := libc libcrypto

  LOCAL_MODULE := ssu_beta

  include $(BUILD_EXECUTABLE)
