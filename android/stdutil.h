///////////////////////////////////////////////////////////////////////////////
// stdutil.h
//
// Copyright(c) 2018, Garfield. All rights reserved.
// Author  : Garfield
// Version : 2.0
// Creation Date : 2011/6/20

#ifndef __STDUTIL_H__
#define __STDUTIL_H__

#include "platform.h"

///////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// MD5
// SHA1
// SHA256
// SHA384
// SHA512
// regex
// regmatch
// timeval_t
// timespec_t
// LogPrinter
// atomic_t<_Ty>
// TempBuffer<_Ty, t_nFixedBytes>
//
// Global functions in this file:
//
// isEmpty()
// nanoTime()
// currentTimeMillis()

__BEGIN_NAMESPACE

#ifdef _MSGDIGEST_INC_
///////////////////////////////////////////////////////////////////////////////
// Interface of the MD5 class
//

class MD5 : public MD5_CTX
{
// Constructors
public:
    MD5();

// Implementation
public:
    enum { MESSAGE_DIGEST_LENGTH = MD5_DIGEST_LENGTH };

// Operations
public:
    void reset();
    void update(const u_char* data, uint32_t size);
    void digest(u_char buffer[MESSAGE_DIGEST_LENGTH]);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the SHA1 class
//

class SHA1 : public SHA1_CTX
{
// Constructors
public:
    SHA1();

// Implementation
public:
    enum { MESSAGE_DIGEST_LENGTH = SHA1_DIGEST_LENGTH };

// Operations
public:
    void reset();
    void update(const u_char* data, uint32_t size);
    void digest(u_char buffer[MESSAGE_DIGEST_LENGTH]);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the SHA256 class
//

class SHA256 : public SHA256_CTX
{
// Constructors
public:
    SHA256();

// Implementation
public:
    enum { MESSAGE_DIGEST_LENGTH = SHA256_DIGEST_LENGTH };

// Operations
public:
    void reset();
    void update(const u_char* data, uint32_t size);
    void digest(u_char buffer[MESSAGE_DIGEST_LENGTH]);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the SHA384 class
//

class SHA384 : public SHA512_CTX
{
// Constructors
public:
    SHA384();

// Implementation
public:
    enum { MESSAGE_DIGEST_LENGTH = SHA384_DIGEST_LENGTH };

// Operations
public:
    void reset();
    void update(const u_char* data, uint32_t size);
    void digest(u_char buffer[MESSAGE_DIGEST_LENGTH]);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the SHA512 class
//

class SHA512 : public SHA512_CTX
{
// Constructors
public:
    SHA512();

// Implementation
public:
    enum { MESSAGE_DIGEST_LENGTH = SHA512_DIGEST_LENGTH };

// Operations
public:
    void reset();
    void update(const u_char* data, uint32_t size);
    void digest(u_char buffer[MESSAGE_DIGEST_LENGTH]);
};
#endif  // _MSGDIGEST_INC_


#ifdef _REGEX_H_
///////////////////////////////////////////////////////////////////////////////
// Interface of the regex class
//

class regex
{
    DECLARE_NONCOPYABLE(regex);

// Constructors/Destructor
public:
    regex();
    ~regex();

// Operations
public:
    int compile(const char* pattern, int flags = REG_BASIC);
    void destroy();

    int match(const char* input, int flags = 0) const;
    int match(const char* input, regmatch_t* results, uint32_t count, int flags = 0) const;

// Implementation
private:
#ifndef NDEBUG
    int CDECL regerror(int errcode, const char* format, ...) const
        __attribute__ ((format(printf, 3, 4)));
#endif

// Data members
private:
    int mStatus;
    regex_t mRegex;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the regmatch class
//

class regmatch : public regmatch_t
{
// Operations
public:
#ifndef NDEBUG
    void dump(const char* input) const;
#endif

#if defined(_STLP_STRING) || defined(_GLIBCXX_STRING)
    std::string toString(const char* input) const;
#endif

    uint32_t copyTo(const char* input, char* buf, uint32_t length) const;
};
#endif  // _REGEX_H_


///////////////////////////////////////////////////////////////////////////////
// Forward declarations
//

class timeval_t;
class timespec_t;


///////////////////////////////////////////////////////////////////////////////
// Interface of the timeval_t class
//

class timeval_t : public timeval
{
// Constructors
public:
    timeval_t();
    explicit timeval_t(uint64_t millis);
    explicit timeval_t(const timeval& tv);
    explicit timeval_t(const timespec& ts);
    timeval_t(time_t sec, suseconds_t usec);

// Operations
public:
    operator timeval*();
    operator const timeval*() const;

    uint64_t toMillis() const;
    timespec_t toTimespec() const;
    void set(time_t sec, suseconds_t usec);

#ifndef NDEBUG
    void dump() const;
#endif

    timeval_t& operator=(uint64_t millis);
    timeval_t& operator=(const timeval& tv);
    timeval_t& operator=(const timespec& ts);
    timeval_t& operator+=(uint64_t millis);
    timeval_t& operator-=(uint64_t millis);

    static timeval_t getCurrentTime();
    static uint64_t toMillis(time_t sec, suseconds_t usec);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the timespec_t class
//

class timespec_t : public timespec
{
// Constructors
public:
    timespec_t();
    timespec_t(time_t sec, long nsec);
    explicit timespec_t(uint64_t millis);
    explicit timespec_t(const timeval& tv);
    explicit timespec_t(const timespec& ts);

// Operations
public:
    operator timespec*();
    operator const timespec*() const;

    uint64_t toMillis() const;
    timeval_t toTimeval() const;
    void set(time_t sec, long nsec);

#ifndef NDEBUG
    void dump() const;
#endif

    timespec_t& operator=(uint64_t millis);
    timespec_t& operator=(const timeval& tv);
    timespec_t& operator=(const timespec& ts);
    timespec_t& operator+=(uint64_t millis);
    timespec_t& operator-=(uint64_t millis);

    static timespec_t getCurrentTime();
    static timespec_t toTimespec(uint64_t millis);
    static uint64_t toMillis(time_t sec, long nsec);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the LogPrinter class
//

class LogPrinter
{
    DECLARE_NONCOPYABLE(LogPrinter);

// Constructors
public:
    explicit LogPrinter(const char* TAG, int priority = ANDROID_LOG_INFO);

// Operations
public:
    void CDECL operator()(const char* format, ...) const
        __attribute__ ((format(printf, 2, 3)));

// Data members
private:
    int mPriority;
    const char* mTAG;
};


#ifdef _SYS_ATOMICS_H
///////////////////////////////////////////////////////////////////////////////
// Interface of the atomic_t class
//

template <typename _Ty = int>
class atomic_t
{
// Constructors
public:
    explicit atomic_t(_Ty value = 0);
    atomic_t(const atomic_t<_Ty>& that);

// Operations
public:
    operator _Ty() const;
    atomic_t<_Ty>& operator=(_Ty value);
    atomic_t<_Ty>& operator=(const atomic_t<_Ty>& that);

    _Ty operator++();       // pre-increment (++i)
    _Ty operator++(int);    // post-increment (i++)

    _Ty operator--();       // pre-decrement (--i)
    _Ty operator--(int);    // post-decrement (i--)

    _Ty operator+=(_Ty value);
    _Ty operator-=(_Ty value);

    _Ty getAndAdd(_Ty value);
    _Ty getAndSet(_Ty value);
    int compareAndSet(_Ty oldValue, _Ty newValue);

#ifndef NDEBUG
    void dump() const;
#endif

// Data members
private:
    int mValue;
};
#endif  // _SYS_ATOMICS_H


///////////////////////////////////////////////////////////////////////////////
// Interface of the TempBuffer class
//

template <typename _Ty, uint32_t t_nFixedBytes = 128>
class TempBuffer
{
    DECLARE_NONCOPYABLE(TempBuffer);

// Constructors/Destructor
public:
    TempBuffer();
    ~TempBuffer();

// Operations
public:
    _Ty* allocate(uint32_t count);
    _Ty* allocateBytes(uint32_t size);

    operator _Ty*();
    operator const _Ty*() const;

// Data members
private:
#ifndef NDEBUG
    uint32_t mSize;
#endif

    _Ty* mBuffer;
    char mFixedBuffer[t_nFixedBytes];
};


///////////////////////////////////////////////////////////////////////////////
// Global functions
//

__STATIC_INLINE__ bool isEmpty(const char* str)
{
    return (str == NULL || *str == '\0');
}

__STATIC_INLINE__ uint64_t nanoTime()
{
    struct timespec now;
    verify(::clock_gettime(CLOCK_MONOTONIC, &now), 0);
    return (uint64_t)now.tv_sec * NANOSECONDS + now.tv_nsec;
}

__STATIC_INLINE__ uint64_t currentTimeMillis()
{
    return timeval_t::getCurrentTime().toMillis();
}

__END_NAMESPACE

#include "stdutil.inl"

#endif  // __STDUTIL_H__