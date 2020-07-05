LOCAL_PATH := $(call my-dir)
LOCAL_MD_MODULE     := md
LOCAL_MD_C_INCLUDES := ../..
LOCAL_MD_SRC_FILES  := md5.cpp sha1.cpp sha256.cpp sha512.cpp
LOCAL_MD_CPPFLAGS   := -O3 -finline-functions -felide-constructors -funswitch-loops -Wall -Wunused \
                       -Wcomment -Wparentheses -Wunused-label -Wchar-subscripts -Wunused-variable \
                       -Wunused-function -Wunused-parameter -Wuninitialized -Wreturn-type -Wformat

###############################################################################
# Build static library

include $(CLEAR_VARS)
LOCAL_C_INCLUDES += $(LOCAL_MD_C_INCLUDES)
LOCAL_CPPFLAGS   += $(LOCAL_MD_CPPFLAGS) -fvisibility=hidden
LOCAL_MODULE     := $(LOCAL_MD_MODULE)_static
LOCAL_SRC_FILES  := $(LOCAL_MD_SRC_FILES)
include $(BUILD_STATIC_LIBRARY)

###############################################################################
# Build shared library

include $(CLEAR_VARS)
ifeq ($(NDK_DEBUG),1)
LOCAL_LDLIBS += -llog
endif

LOCAL_C_INCLUDES += $(LOCAL_MD_C_INCLUDES)
LOCAL_CPPFLAGS   += $(LOCAL_MD_CPPFLAGS)
LOCAL_MODULE     := $(LOCAL_MD_MODULE)
LOCAL_SRC_FILES  := $(LOCAL_MD_SRC_FILES)
LOCAL_STATIC_LIBRARIES:= $(LOCAL_MD_MODULE)_static
include $(BUILD_SHARED_LIBRARY)