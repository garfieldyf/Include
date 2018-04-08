LOCAL_PATH := $(call my-dir)
LOCAL_IMGALGTH_MODULE     := imgalgth
LOCAL_IMGALGTH_C_INCLUDES := ../.. ../../../common
LOCAL_IMGALGTH_SRC_FILES  := imgalgth.cpp
LOCAL_IMGALGTH_CPPFLAGS   := -O3 -finline-functions -felide-constructors -funswitch-loops -Wall -Wunused \
                             -Wcomment -Wparentheses -Wunused-label -Wchar-subscripts -Wunused-variable \
                             -Wunused-function -Wunused-parameter -Wuninitialized -Wreturn-type -Wformat

###############################################################################
# Build static library

include $(CLEAR_VARS)
LOCAL_C_INCLUDES += $(LOCAL_IMGALGTH_C_INCLUDES)
LOCAL_CPPFLAGS   += $(LOCAL_IMGALGTH_CPPFLAGS)
LOCAL_MODULE     := $(LOCAL_IMGALGTH_MODULE)_static
LOCAL_SRC_FILES  := $(LOCAL_IMGALGTH_SRC_FILES)
include $(BUILD_STATIC_LIBRARY)

###############################################################################
# Build shared library

include $(CLEAR_VARS)
ifeq ($(NDK_DEBUG),1)
LOCAL_LDLIBS += -llog
endif

LOCAL_C_INCLUDES += $(LOCAL_IMGALGTH_C_INCLUDES)
LOCAL_CPPFLAGS   += $(LOCAL_IMGALGTH_CPPFLAGS) -D__STDCEXPORT__
LOCAL_MODULE     := $(LOCAL_IMGALGTH_MODULE)
LOCAL_SRC_FILES  := $(LOCAL_IMGALGTH_SRC_FILES)
LOCAL_STATIC_LIBRARIES:= $(LOCAL_IMGALGTH_MODULE)_static
include $(BUILD_SHARED_LIBRARY)