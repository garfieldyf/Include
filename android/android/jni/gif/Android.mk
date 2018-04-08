LOCAL_PATH := $(call my-dir)
LOCAL_GIF_MODULE     := gif
LOCAL_GIF_C_INCLUDES := ../..
LOCAL_GIF_SRC_FILES  := dgif_lib.c egif_lib.c gif_hash.c gifalloc.c gif_err.c
LOCAL_GIF_CFLAGS     := -O3 -finline-functions -felide-constructors -funswitch-loops -Wall -Wunused \
                        -Wcomment -Wparentheses -Wunused-label -Wchar-subscripts -Wunused-variable \
                        -Wunused-function -Wuninitialized -Wreturn-type -Wformat

###############################################################################
# Build static library

include $(CLEAR_VARS)
LOCAL_C_INCLUDES += $(LOCAL_GIF_C_INCLUDES)
LOCAL_CFLAGS     += $(LOCAL_GIF_CFLAGS)
LOCAL_MODULE     := $(LOCAL_GIF_MODULE)_static
LOCAL_SRC_FILES  := $(LOCAL_GIF_SRC_FILES)
include $(BUILD_STATIC_LIBRARY)

###############################################################################
# Build shared library

include $(CLEAR_VARS)
LOCAL_C_INCLUDES += $(LOCAL_GIF_C_INCLUDES)
LOCAL_CFLAGS     += $(LOCAL_GIF_CFLAGS) -D__STDCEXPORT__
LOCAL_MODULE     := $(LOCAL_GIF_MODULE)
LOCAL_SRC_FILES  := $(LOCAL_GIF_SRC_FILES)
LOCAL_STATIC_LIBRARIES:= $(LOCAL_GIF_MODULE)_static
include $(BUILD_SHARED_LIBRARY)