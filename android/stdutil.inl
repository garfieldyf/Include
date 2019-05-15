///////////////////////////////////////////////////////////////////////////////
// stdutil.inl
//
// Copyright(c) 2019, Garfield. All rights reserved.
// Author  : Garfield
// Version : 2.0
// Creation Date : 2011/6/20

#ifndef __STDUTIL_INL__
#define __STDUTIL_INL__

#ifndef __STDUTIL_H__
    #error stdutil.inl requires stdutil.h to be included first
#endif

__BEGIN_NAMESPACE

#ifdef _MSGDIGEST_INC_
///////////////////////////////////////////////////////////////////////////////
// Implementation of the MD5 class
//

__INLINE__ MD5::MD5()
{
    ::MD5Init(this);
}

__INLINE__ void MD5::reset()
{
    ::MD5Init(this);
}

__INLINE__ void MD5::update(const u_char* data, uint32_t size)
{
    assert(data);
    ::MD5Update(this, data, size);
}

__INLINE__ void MD5::digest(u_char buffer[MESSAGE_DIGEST_LENGTH])
{
    assert(buffer);
    ::MD5Final(this, buffer);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the SHA1 class
//

__INLINE__ SHA1::SHA1()
{
    ::SHA1Init(this);
}

__INLINE__ void SHA1::reset()
{
    ::SHA1Init(this);
}

__INLINE__ void SHA1::update(const u_char* data, uint32_t size)
{
    assert(data);
    ::SHA1Update(this, data, size);
}

__INLINE__ void SHA1::digest(u_char buffer[MESSAGE_DIGEST_LENGTH])
{
    assert(buffer);
    ::SHA1Final(this, buffer);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the SHA256 class
//

__INLINE__ SHA256::SHA256()
{
    ::SHA256Init(this);
}

__INLINE__ void SHA256::reset()
{
    ::SHA256Init(this);
}

__INLINE__ void SHA256::update(const u_char* data, uint32_t size)
{
    assert(data);
    ::SHA256Update(this, data, size);
}

__INLINE__ void SHA256::digest(u_char buffer[MESSAGE_DIGEST_LENGTH])
{
    assert(buffer);
    ::SHA256Final(this, buffer);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the SHA384 class
//

__INLINE__ SHA384::SHA384()
{
    ::SHA384Init(this);
}

__INLINE__ void SHA384::reset()
{
    ::SHA384Init(this);
}

__INLINE__ void SHA384::update(const u_char* data, uint32_t size)
{
    assert(data);
    ::SHA512Update(this, data, size);
}

__INLINE__ void SHA384::digest(u_char buffer[MESSAGE_DIGEST_LENGTH])
{
    assert(buffer);
    ::SHA384Final(this, buffer);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the SHA512 class
//

__INLINE__ SHA512::SHA512()
{
    ::SHA512Init(this);
}

__INLINE__ void SHA512::reset()
{
    ::SHA512Init(this);
}

__INLINE__ void SHA512::update(const u_char* data, uint32_t size)
{
    assert(data);
    ::SHA512Update(this, data, size);
}

__INLINE__ void SHA512::digest(u_char buffer[MESSAGE_DIGEST_LENGTH])
{
    assert(buffer);
    ::SHA512Final(this, buffer);
}
#endif  // _MSGDIGEST_INC_


#ifdef _REGEX_H_
///////////////////////////////////////////////////////////////////////////////
// Implementation of the regex class
//

__INLINE__ regex::regex()
    : mStatus(REG_NOMATCH)
{
}

__INLINE__ regex::~regex()
{
    destroy();
}

__INLINE__ int regex::compile(const char* pattern, int flags/* = REG_BASIC*/)
{
    assert(pattern);

#ifndef NDEBUG
    return regerror(mStatus = ::regcomp(&mRegex, pattern, flags), "Couldn't compile (pattern = '%s', flags = 0x%08x", pattern, flags);
#else
    return (mStatus = ::regcomp(&mRegex, pattern, flags));
#endif  // NDEBUG
}

__INLINE__ void regex::destroy()
{
    if (mStatus == 0)
    {
        ::regfree(&mRegex);
        mStatus = REG_NOMATCH;
        LOGI("The regex destroy\n");
    }
}

__INLINE__ int regex::match(const char* input, int flags/* = 0*/) const
{
    assert(input);
    assert(mStatus == 0);

    regmatch result;
#ifndef NDEBUG
    const int errcode = regerror(::regexec(&mRegex, input, 1, &result, flags), "Couldn't match (input = '%s', flags = 0x%08x", input, flags);
    if (errcode == 0)
        result.dump(input);

    return errcode;
#else
    return ::regexec(&mRegex, input, 1, &result, flags);
#endif  // NDEBUG
}

__INLINE__ int regex::match(const char* input, regmatch_t* results, uint32_t count, int flags/* = 0*/) const
{
    assert(input);
    assert(mStatus == 0);

#ifndef NDEBUG
    return regerror(::regexec(&mRegex, input, count, results, flags), "Couldn't match (input = '%s', flags = 0x%08x", input, flags);
#else
    return ::regexec(&mRegex, input, count, results, flags);
#endif  // NDEBUG
}

#ifndef NDEBUG
__INLINE__ int CDECL regex::regerror(int errcode, const char* format, ...) const
{
    if (errcode != 0)
    {
        va_list args;
        va_start(args, format);

        char prefix[MAX_PATH];
        ::vsnprintf(prefix, _countof(prefix), format, args);
        va_end(args);

        char error[MAX_PATH];
        ::regerror(errcode, &mRegex, error, _countof(error));
        LOGE("%s, errcode = %d, error = %s)\n", prefix, errcode, error);
    }

    return errcode;
}
#endif  // NDEBUG


///////////////////////////////////////////////////////////////////////////////
// Implementation of the regmatch class
//

#ifndef NDEBUG
__INLINE__ void regmatch::dump(const char* input) const
{
    assert(input);
    LOGI("regex match result = %.*s\n", (uint32_t)(rm_eo - rm_so), input + rm_so);
}
#endif  // NDEBUG

#if defined(_STLP_STRING) || defined(_GLIBCXX_STRING) || defined(_LIBCPP_STRING)
__INLINE__ std::string regmatch::toString(const char* input) const
{
    assert(input);
    return std::string(input + rm_so, rm_eo - rm_so);
}
#endif  // defined(_STLP_STRING) || defined(_GLIBCXX_STRING) || defined(_LIBCPP_STRING)

__INLINE__ uint32_t regmatch::copyTo(const char* input, char* buf, uint32_t length) const
{
    assert(buf);
    assert(input);

    return ::snprintf(buf, length, "%.*s", (uint32_t)(rm_eo - rm_so), input + rm_so);
}
#endif  // _REGEX_H_


///////////////////////////////////////////////////////////////////////////////
// Implementation of the timeval_t class
//

__INLINE__ timeval_t::timeval_t()
{
}

__INLINE__ timeval_t::timeval_t(uint64_t millis)
{
    set((time_t)(millis / MILLISECONDS), (suseconds_t)((millis % MILLISECONDS) * MILLISECONDS));
}

__INLINE__ timeval_t::timeval_t(const timeval& tv)
{
    set(tv.tv_sec, tv.tv_usec);
}

__INLINE__ timeval_t::timeval_t(const timespec& ts)
{
    set(ts.tv_sec, ts.tv_nsec / MILLISECONDS);
}

__INLINE__ timeval_t::timeval_t(time_t sec, suseconds_t usec)
{
    set(sec, usec);
}

__INLINE__ timeval_t::operator timeval*()
{
    return this;
}

__INLINE__ timeval_t::operator const timeval*() const
{
    return this;
}

__INLINE__ uint64_t timeval_t::toMillis() const
{
    return toMillis(tv_sec, tv_usec);
}

__INLINE__ timespec_t timeval_t::toTimespec() const
{
    return timespec_t(*this);
}

__INLINE__ void timeval_t::set(time_t sec, suseconds_t usec)
{
    tv_sec  = sec;
    tv_usec = usec;
}

#ifndef NDEBUG
__INLINE__ void timeval_t::dump() const
{
    LOGI("timeval_t = " _PRId64 " (ms) [ seconds = %ld, microseconds = %ld ]\n", toMillis(), tv_sec, tv_usec);
}
#endif  // NDEBUG

__INLINE__ timeval_t& timeval_t::operator=(uint64_t millis)
{
    set((time_t)(millis / MILLISECONDS), (suseconds_t)((millis % MILLISECONDS) * MILLISECONDS));
    return *this;
}

__INLINE__ timeval_t& timeval_t::operator=(const timeval& tv)
{
    set(tv.tv_sec, tv.tv_usec);
    return *this;
}

__INLINE__ timeval_t& timeval_t::operator=(const timespec& ts)
{
    set(ts.tv_sec, ts.tv_nsec / MILLISECONDS);
    return *this;
}

__INLINE__ timeval_t& timeval_t::operator+=(uint64_t millis)
{
    tv_sec  += (time_t)(millis / MILLISECONDS);
    tv_usec += (suseconds_t)((millis % MILLISECONDS) * MILLISECONDS);

    if (tv_usec >= MICROSECONDS)
    {
        tv_sec  += 1;
        tv_usec -= MICROSECONDS;
    }

    return *this;
}

__INLINE__ timeval_t& timeval_t::operator-=(uint64_t millis)
{
    tv_sec  -= (time_t)(millis / MILLISECONDS);
    tv_usec -= (suseconds_t)((millis % MILLISECONDS) * MILLISECONDS);

    if (tv_usec < 0)
    {
        tv_sec  -= 1;
        tv_usec += MICROSECONDS;
    }

    return *this;
}

__INLINE__ timeval_t timeval_t::getCurrentTime()
{
    timeval_t now;
    verify(::gettimeofday(now, NULL), 0);

    return now;
}

__INLINE__ uint64_t timeval_t::toMillis(time_t sec, suseconds_t usec)
{
    return ((uint64_t)sec * MICROSECONDS + (uint64_t)usec) / MILLISECONDS;
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the timespec_t class
//

__INLINE__ timespec_t::timespec_t()
{
}

__INLINE__ timespec_t::timespec_t(uint64_t millis)
{
    set((time_t)(millis / MILLISECONDS), (long)((millis % MILLISECONDS) * MICROSECONDS));
}

__INLINE__ timespec_t::timespec_t(const timeval& tv)
{
    set(tv.tv_sec, tv.tv_usec * MILLISECONDS);
}

__INLINE__ timespec_t::timespec_t(const timespec& ts)
{
    set(ts.tv_sec, ts.tv_nsec);
}

__INLINE__ timespec_t::timespec_t(time_t sec, long nsec)
{
    set(sec, nsec);
}

__INLINE__ timespec_t::operator timespec*()
{
    return this;
}

__INLINE__ timespec_t::operator const timespec*() const
{
    return this;
}

__INLINE__ uint64_t timespec_t::toMillis() const
{
    return toMillis(tv_sec, tv_nsec);
}

__INLINE__ timeval_t timespec_t::toTimeval() const
{
    return timeval_t(*this);
}

__INLINE__ void timespec_t::set(time_t sec, long nsec)
{
    tv_sec  = sec;
    tv_nsec = nsec;
}

#ifndef NDEBUG
__INLINE__ void timespec_t::dump() const
{
    LOGI("timespec_t = " _PRId64 " (ms) [ seconds = %ld, nanoseconds = %ld ]\n", toMillis(), tv_sec, tv_nsec);
}
#endif  // NDEBUG

__INLINE__ timespec_t& timespec_t::operator=(uint64_t millis)
{
    set((time_t)(millis / MILLISECONDS), (long)((millis % MILLISECONDS) * MICROSECONDS));
    return *this;
}

__INLINE__ timespec_t& timespec_t::operator=(const timeval& tv)
{
    set(tv.tv_sec, tv.tv_usec * MILLISECONDS);
    return *this;
}

__INLINE__ timespec_t& timespec_t::operator=(const timespec& ts)
{
    set(ts.tv_sec, ts.tv_nsec);
    return *this;
}

__INLINE__ timespec_t& timespec_t::operator+=(uint64_t millis)
{
    tv_sec  += (time_t)(millis / MILLISECONDS);
    tv_nsec += (long)((millis % MILLISECONDS) * MICROSECONDS);

    if (tv_nsec >= NANOSECONDS)
    {
        tv_sec  += 1;
        tv_nsec -= NANOSECONDS;
    }

    return *this;
}

__INLINE__ timespec_t& timespec_t::operator-=(uint64_t millis)
{
    tv_sec  -= (time_t)(millis / MILLISECONDS);
    tv_nsec -= (long)((millis % MILLISECONDS) * MICROSECONDS);

    if (tv_nsec < 0)
    {
        tv_sec  -= 1;
        tv_nsec += NANOSECONDS;
    }

    return *this;
}

__INLINE__ timespec_t timespec_t::getCurrentTime()
{
    timespec_t now;
    verify(::clock_gettime(CLOCK_REALTIME, now), 0);

    return now;
}

__INLINE__ timespec_t timespec_t::toTimespec(uint64_t millis)
{
    timespec_t now;
    verify(::clock_gettime(CLOCK_REALTIME, now), 0);

    return (now += millis);
}

__INLINE__ uint64_t timespec_t::toMillis(time_t sec, long nsec)
{
    return ((uint64_t)sec * NANOSECONDS + (uint64_t)nsec) / MICROSECONDS;
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the LogPrinter class
//

__INLINE__ LogPrinter::LogPrinter(const char* TAG, int priority/* = ANDROID_LOG_INFO*/)
    : mPriority(priority), mTAG(TAG)
{
}

__INLINE__ void CDECL LogPrinter::operator()(const char* format, ...) const
{
    assert(format);

    va_list args;
    va_start(args, format);

    __android_log_vprint(mPriority, mTAG, format, args);
    va_end(args);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the TempBuffer class
//

template <typename _Ty, uint32_t t_nFixedBytes>
__INLINE__ TempBuffer<_Ty, t_nFixedBytes>::TempBuffer()
#ifndef NDEBUG
    : mSize(0), mBuffer(NULL)
#else
    : mBuffer(NULL)
#endif  // NDEBUG
{
}

template <typename _Ty, uint32_t t_nFixedBytes>
__INLINE__ TempBuffer<_Ty, t_nFixedBytes>::~TempBuffer()
{
    if (mBuffer != (_Ty*)mFixedBuffer)
    {
        ::free(mBuffer);
        LOGI("free memory [ buffer = %p, size = %u ]\n", mBuffer, mSize);
    }
#ifndef NDEBUG
    else
    {
        ::memset(mFixedBuffer, 0xCCCCCCCC, sizeof(mFixedBuffer));
    }
#endif  // NDEBUG
}

template <typename _Ty, uint32_t t_nFixedBytes>
__INLINE__ _Ty* TempBuffer<_Ty, t_nFixedBytes>::allocate(uint32_t count)
{
    return allocateBytes(count * sizeof(_Ty));
}

template <typename _Ty, uint32_t t_nFixedBytes>
__INLINE__ _Ty* TempBuffer<_Ty, t_nFixedBytes>::allocateBytes(uint32_t size)
{
    assert_log(mBuffer == NULL, "The buffer already allocated.");

#ifndef NDEBUG
    mSize = size;
    if ((mBuffer = (size > t_nFixedBytes ? (_Ty*)::malloc(size) : (_Ty*)mFixedBuffer)) == NULL)
        LOGE("Couldn't allocate memory (size = %u)\n", size);

    return mBuffer;
#else
    return (mBuffer = (size > t_nFixedBytes ? (_Ty*)::malloc(size) : (_Ty*)mFixedBuffer));
#endif  // NDEBUG
}

template <typename _Ty, uint32_t t_nFixedBytes>
__INLINE__ TempBuffer<_Ty, t_nFixedBytes>::operator _Ty*()
{
    return mBuffer;
}

template <typename _Ty, uint32_t t_nFixedBytes>
__INLINE__ TempBuffer<_Ty, t_nFixedBytes>::operator const _Ty*() const
{
    return mBuffer;
}

__END_NAMESPACE

#endif  // __STDUTIL_INL__