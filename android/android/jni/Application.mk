# The ARMv7 is significanly faster due to the use of the hardware FPU
# APP_ABI := arm64-v8a mips64 x86_64
# APP_ABI := armeabi armeabi-v7a x86 mips

APP_ABI := armeabi-v7a arm64-v8a x86 x86_64
APP_PLATFORM := android-14
APP_CPPFLAGS += -fno-rtti -fno-exceptions -std=c++11
