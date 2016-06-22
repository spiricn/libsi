LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LIB_ROOT=$(LOCAL_PATH)/../..

SOURCE_DIR=../../source
INCLUDE_PATH=$(LIB_ROOT)/include

LOCAL_SRC_FILES := 	\
	$(SOURCE_DIR)/DeviceReader.cpp \
	$(SOURCE_DIR)/DeviceReaderManager.cpp \
	$(SOURCE_DIR)/DirectoryMonitor.cpp \
					
LOCAL_C_INCLUDES += $(INCLUDE_PATH)

LOCAL_MODULE := libsi

LOCAL_SHARED_LIBRARIES += liblog

LOCAL_CFLAGS:= -DANDROID

LOCAL_MODULE_TAGS := optional

include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
