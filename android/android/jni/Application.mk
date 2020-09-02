# The ARMv7 is significanly faster due to the use of the hardware FPU
# APP_ABI := arm64-v8a armeabi-v7a x86_64 x86

ifeq ($(origin NDK_ABI), undefined)
APP_ABI := all
else
APP_ABI := $(NDK_ABI)
endif

APP_PLATFORM := android-16
APP_CPPFLAGS += -fno-rtti -fno-exceptions -std=c++11
