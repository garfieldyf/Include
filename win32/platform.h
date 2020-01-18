///////////////////////////////////////////////////////////////////////////////
// platform.h
//
// Author : Garfield
// Creation Date : 2011/6/23

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __PLATFORM_H__
#define __PLATFORM_H__

#include <assert.h>
#include "crtdbgutil.h"

///////////////////////////////////////////////////////////////////////////////
// Macros in this file:
//

typedef INT32   int32_t;
typedef UINT32  uint32_t;

#ifndef __INLINE__
#define __INLINE__                          inline
#endif

#ifndef __STATIC_INLINE__
#define __STATIC_INLINE__                   static inline
#endif

#ifndef INVALID_LENGTH
#define INVALID_LENGTH                      ((size_t)-1)
#endif

#ifndef verify
#ifndef NDEBUG
#define verify(expr, result)                assert((expr) == (result))
#else
#define verify(expr, result)                (expr)
#endif
#endif

#ifndef _verify
#ifndef NDEBUG
#define _verify(expr)                       assert(expr)
#else
#define _verify(expr)                       (expr)
#endif
#endif

#ifndef assert_log
#ifndef NDEBUG
#define assert_log(expr, msg)               (void)((!!(expr)) || (_wassert(_CRT_WIDE(msg), _CRT_WIDE(__FILE__), __LINE__), 0))
#else
#define assert_log(expr, msg)               ((void)0)
#endif
#endif

#ifndef __check_error
#ifndef NDEBUG
#define __check_error(cond, ...)            if (cond) stdutil::CCrtTrace::_CrtTrace.LogPrint(__FUNCTION__, __VA_ARGS__)
#else
#define __check_error(cond, ...)            __noop
#endif
#endif

#ifndef NDEBUG
#define LOGI(...)                           stdutil::CCrtTrace::_CrtTrace.LogPrint(__FUNCTION__, __VA_ARGS__)
#define LOGW(...)                           stdutil::CCrtTrace::_CrtTrace.LogPrint(__FUNCTION__, __VA_ARGS__)
#define LOGD(...)                           stdutil::CCrtTrace::_CrtTrace.LogPrint(__FUNCTION__, __VA_ARGS__)
#define LOGE(...)                           stdutil::CCrtTrace::_CrtTrace.LogPrint(__FUNCTION__, __VA_ARGS__)
#else
#define LOGI(...)                           __noop
#define LOGW(...)                           __noop
#define LOGD(...)                           __noop
#define LOGE(...)                           __noop
#endif

#ifndef NDEBUG
#define LOG_INFO(__TAG__, ...)              stdutil::CCrtTrace::_CrtTrace.LogPrint((__TAG__), __VA_ARGS__)
#define LOG_WARN(__TAG__, ...)              stdutil::CCrtTrace::_CrtTrace.LogPrint((__TAG__), __VA_ARGS__)
#define LOG_DEBUG(__TAG__, ...)             stdutil::CCrtTrace::_CrtTrace.LogPrint((__TAG__), __VA_ARGS__)
#define LOG_ERROR(__TAG__, ...)             stdutil::CCrtTrace::_CrtTrace.LogPrint((__TAG__), __VA_ARGS__)
#else
#define LOG_INFO(__TAG__, ...)              __noop
#define LOG_WARN(__TAG__, ...)              __noop
#define LOG_DEBUG(__TAG__, ...)             __noop
#define LOG_ERROR(__TAG__, ...)             __noop
#endif

#endif  // __PLATFORM_H__