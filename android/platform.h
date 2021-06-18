///////////////////////////////////////////////////////////////////////////////
// platform.h
//
// Author : Garfield
// Creation Date : 2011/6/23

#ifndef __PLATFORM_H__
#define __PLATFORM_H__

#ifdef __LP64__
#include <time.h>
#else
#include <time64.h>
#endif

#include <jni.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <ctype.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <dirent.h>
#include <inttypes.h>
#include <sys/stat.h>
#include <android/log.h>
#include <android/api-level.h>
#include <sys/system_properties.h>

///////////////////////////////////////////////////////////////////////////////
// Macros in this file:
//

#ifdef assert
#undef assert
#endif

#ifndef NDEBUG
#define assert(expr)                            if (!(expr)) __android_assert(::strrchr(__FILE__, '/') + 1, __LINE__, __func__, #expr)
#else
#define assert(expr)                            ((void)0)
#endif

#ifndef verify
#ifndef NDEBUG
#define verify(expr, result)                    assert((expr) == (result))
#else
#define verify(expr, result)                    (expr)
#endif
#endif

#ifndef _verify
#ifndef NDEBUG
#define _verify(expr)                           assert(expr)
#else
#define _verify(expr)                           (expr)
#endif
#endif

#ifndef __verify
#ifndef NDEBUG
#define __verify(errnum, ...)                   __android_log_error((errnum), __func__, __VA_ARGS__)
#else
#define __verify(errnum, ...)                   (errnum)
#endif
#endif

#ifndef assert_log
#ifndef NDEBUG
#define assert_log(expr, ...)                   if (!(expr)) __android_assert(::strrchr(__FILE__, '/') + 1, __LINE__, __func__, __VA_ARGS__)
#else
#define assert_log(expr, ...)                   ((void)0)
#endif
#endif

#ifndef __check_error
#ifndef NDEBUG
#define __check_error(expr, ...)                if (expr) do { char __tag[MAX_PATH]; __android_log_print(ANDROID_LOG_ERROR, __android_build_tag(__func__, __tag), __VA_ARGS__); } while (0)
#else
#define __check_error(expr, ...)                ((void)0)
#endif
#endif

#ifndef __check_error2
#ifndef NDEBUG
#define __check_error2(expr, ...)               if (expr) __android_log_error(errno, __func__, __VA_ARGS__)
#else
#define __check_error2(expr, ...)               ((void)0)
#endif
#endif

#ifndef _T
#define _T(x)                                   x
#endif

#ifndef CDECL
#define CDECL
#endif

#ifndef ATL_NO_VTABLE
#define ATL_NO_VTABLE
#endif

#ifndef TCHAR_DEFINED
#define TCHAR_DEFINED
typedef char TCHAR;
#endif

#ifdef __LP64__
#define _PRId64                                 "%ld"
#define _PRId                                   "%ld"
#else
#define _PRId64                                 "%lld"
#define _PRId                                   "%d"
#endif

#if __cplusplus >= 201703L
#define _CONSTEXPR                              constexpr
#else
#define _CONSTEXPR
#endif

#ifndef UNUSED_PARAMETER
#define UNUSED_PARAMETER(var)                   ((void)(var))
#endif

#ifndef __BEGIN_NAMESPACE
#define __BEGIN_NAMESPACE                       namespace android {
#endif

#ifndef __NS
#define __NS                                    android
#endif

#ifndef __END_NAMESPACE
#define __END_NAMESPACE                         }
#endif

#ifndef JNIEXPORT_METHOD
#define JNIEXPORT_METHOD(type)                  __STATIC_INLINE__ type JNICALL
#endif

#ifndef DECLARE_NONCOPYABLE
#define DECLARE_NONCOPYABLE(classname) \
private: \
    classname(const classname&); \
    classname& operator=(const classname&)
#endif

#ifndef _countof
template <typename _Ty, size_t _CountOfArray>
char (&__countof_helper(_Ty (&_Array)[_CountOfArray]))[_CountOfArray];
#define _countof(_Array)                        sizeof(__countof_helper(_Array))
#endif

#ifndef __INLINE__
#ifndef NDEBUG
#define __INLINE__                              inline
#else
#define __INLINE__                              __inline__ __attribute__((always_inline))
#endif
#endif

#ifndef __STATIC_INLINE__
#ifndef NDEBUG
#define __STATIC_INLINE__                       static inline
#else
#define __STATIC_INLINE__                       static __inline__ __attribute__((always_inline))
#endif
#endif

#ifndef NDEBUG
#define _DEBUG_DUMP(var)                        (var).dump()
#else
#define _DEBUG_DUMP(var)                        ((void)0)
#endif

#ifdef _CRT_DEBUG_DUMP
#define _CRT_DUMP(tag, var)                     (var).Dump(__NS::LogPrinter(tag))
#else
#define _CRT_DUMP(tag, var)                     ((void)0)
#endif

#ifndef NDEBUG
#define LOGI(...)                               do { char __tag[MAX_PATH]; __android_log_print(ANDROID_LOG_INFO,  __android_build_tag(__func__, __tag), __VA_ARGS__); } while (0)
#define LOGW(...)                               do { char __tag[MAX_PATH]; __android_log_print(ANDROID_LOG_WARN,  __android_build_tag(__func__, __tag), __VA_ARGS__); } while (0)
#define LOGD(...)                               do { char __tag[MAX_PATH]; __android_log_print(ANDROID_LOG_DEBUG, __android_build_tag(__func__, __tag), __VA_ARGS__); } while (0)
#define LOGE(...)                               do { char __tag[MAX_PATH]; __android_log_print(ANDROID_LOG_ERROR, __android_build_tag(__func__, __tag), __VA_ARGS__); } while (0)
#else
#define LOGI(...)                               ((void)0)
#define LOGW(...)                               ((void)0)
#define LOGD(...)                               ((void)0)
#define LOGE(...)                               ((void)0)
#endif

#define LOG_INFO(__TAG__, ...)                  __android_log_print(ANDROID_LOG_INFO,  (__TAG__), __VA_ARGS__)
#define LOG_WARN(__TAG__, ...)                  __android_log_print(ANDROID_LOG_WARN,  (__TAG__), __VA_ARGS__)
#define LOG_DEBUG(__TAG__, ...)                 __android_log_print(ANDROID_LOG_DEBUG, (__TAG__), __VA_ARGS__)
#define LOG_ERROR(__TAG__, ...)                 __android_log_print(ANDROID_LOG_ERROR, (__TAG__), __VA_ARGS__)

#ifdef ASSERT_THROW_EXCEPTION
#define AssertThrowErrnoException(env, expr, msg, retval)       do { if (expr) { __android_throw_exception((env), EINVAL, ::strrchr(__FILE__, '/') + 1, __LINE__, __func__, (msg)); return (retval); } } while (0)
#define AssertThrowErrnoExceptionV(env, expr, msg)              do { if (expr) { __android_throw_exception((env), EINVAL, ::strrchr(__FILE__, '/') + 1, __LINE__, __func__, (msg)); return; } } while (0)
#else
#define AssertThrowErrnoException(env, expr, msg, retval)       do { assert_log(!(expr), (msg)); if (expr) return (retval); } while (0)
#define AssertThrowErrnoExceptionV(env, expr, msg)              do { assert_log(!(expr), (msg)); if (expr) return; } while (0)
#endif


///////////////////////////////////////////////////////////////////////////////
// Enumerations in this file:
//

enum
{
    // Mutex and Condition wait infinite
    INFINITE = 0,

    // The maximum path characters
    MAX_PATH = 256,

    // Invalid file descriptor
    INVALID_FD = -1,

    // Invalid length.
    INVALID_LENGTH = -1,

    // Time unit.
    MILLISECONDS = 1000,
    MICROSECONDS = 1000000,
    NANOSECONDS  = 1000000000,

    // Thread priority.
    THREAD_PRIORITY_LOWEST     = 19,
    THREAD_PRIORITY_DEFAULT    = 0,
    THREAD_PRIORITY_BACKGROUND = 10,
    THREAD_PRIORITY_FOREGROUND = -2,
    THREAD_PRIORITY_DISPLAY    = -4,
    THREAD_PRIORITY_AUDIO      = -16,
};


///////////////////////////////////////////////////////////////////////////////
// Forward declarations
//

static inline int CDECL __android_log_error(int errnum, const char* func, const char* format, ...)
    __attribute__ ((format(printf, 3, 4)));

static inline void CDECL __android_assert(const char* file, int line, const char* func, const char* format, ...)
    __attribute__ ((format(printf, 4, 5)));


///////////////////////////////////////////////////////////////////////////////
// Global functions in this file:
//

static inline const char* __android_build_tag(const char* func, char (&tag)[MAX_PATH])
{
    // Finds the function name end pointer (including return type).
    if (const char* end = ::strchr(func, '('))
    {
        // Finds the function name start pointer (excluding return type, etc).
        const char* start = end;
        while (func < start && *start != ' ')
            --start;

        // Skips the whitespace character.
        if (start != func) ++start;
        ::snprintf(tag, _countof(tag), "%.*s", (uint32_t)(end - start), start);
    }
    else
    {
        // Couldn't find the function name, then return the original func.
        ::strlcpy(tag, func, _countof(tag));
    }

    return tag;
}

static inline int CDECL __android_log_error(int errnum, const char* func, const char* format, ...)
{
    if (errnum != 0)
    {
        char error[MAX_PATH], fmt[MAX_PATH];
        ::strerror_r(errnum, error, _countof(error));
        ::snprintf(fmt, _countof(fmt), "%s, errno = %d, error = %s\n", format, errnum, error);

        va_list args;
        va_start(args, format);

        char tag[MAX_PATH];
        ::__android_log_vprint(ANDROID_LOG_ERROR, __android_build_tag(func, tag), fmt, args);
        va_end(args);
    }

    return errnum;
}

static inline const char* __android_class_name(JNIEnv* env, jobject object, char (&buffer)[MAX_PATH])
{
    // Equivalent to java object.getClass().getName()
    jobject clazz = env->CallObjectMethod(object, env->GetMethodID(env->GetObjectClass(object), "getClass", "()Ljava/lang/Class;"));
    jstring className = (jstring)env->CallObjectMethod(clazz, env->GetMethodID(env->GetObjectClass(clazz), "getName", "()Ljava/lang/String;"));

    // Copy the class name to buffer
    const jsize length = env->GetStringUTFLength(className);
    env->GetStringUTFRegion(className, 0, env->GetStringLength(className), buffer);
    buffer[length] = '\0';

    return buffer;
}

static inline void CDECL __android_assert(const char* file, int line, const char* func, const char* format, ...)
{
    va_list args;
    va_start(args, format);

    char error[MAX_PATH];
    ::vsnprintf(error, _countof(error), format, args);
    va_end(args);

    char tag[MAX_PATH];
    ::__android_log_print(ANDROID_LOG_ERROR, __android_build_tag(func, tag), "assertion failure : %s\n\tat file : %s\n\tat line : %d\n\tat function : %s\n", error, file, line, func);
    ::__assert2(file, line, func, error);
}

#ifdef PACKAGE_UTILITIES
__STATIC_INLINE__ jint __android_throw_exception(JNIEnv* env, int errnum, const char* file, int line, const char* func, const char* errorMessage)
{
    jint result = JNI_ERR;
    if (jclass clazz = env->FindClass(PACKAGE_UTILITIES "ErrnoException"))
    {
        static const jmethodID initID = env->GetMethodID(clazz, "<init>", "(ILjava/lang/String;)V");
        assert(initID);

        char error[MAX_PATH];
        ::strerror_r(errnum, error, _countof(error));

        char detailMessage[MAX_PATH * 2];
        ::snprintf(detailMessage, _countof(detailMessage), "%s (errno: %d) - %s\n\tat file : %s\n\tat line : %d\n\tat function : %s", error, errnum, errorMessage, file, line, func);

        if (jthrowable throwable = (jthrowable)env->NewObject(clazz, initID, errnum, env->NewStringUTF(detailMessage)))
            result = env->Throw(throwable);
    }

    return result;
}
#endif  // PACKAGE_UTILITIES

#endif  // __PLATFORM_H__