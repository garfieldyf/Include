///////////////////////////////////////////////////////////////////////////////
// trace.inl
//
// Author : Garfield
// Creation Date : 2020/5/4

#ifndef __TRACE_INL__
#define __TRACE_INL__

#ifndef __TRACE_H__
    #error trace.inl requires trace.h to be included first
#endif

namespace stdutil {

///////////////////////////////////////////////////////////////////////////////
// Global functions
//

#ifdef NDEBUG
#define startMethodTracing()                        ((void)0)
#define stopMethodTracing(_Prefix)                  ((void)0)
#define stopMethodTracing2(_Prefix, _TimeUnit)      ((void)0)
#else
#define startMethodTracing()                        stdutil::Trace::start_method_tracing()
#define stopMethodTracing(_Prefix)                  stopMethodTracing2(_Prefix, 'm')
#define stopMethodTracing2(_Prefix, _TimeUnit)      stdutil::Trace::stop_method_tracing(_Prefix, _TimeUnit)


///////////////////////////////////////////////////////////////////////////////
// Implementation of the Trace class
//

__INLINE__ Trace::Trace()
{
    LOGD("Trace::Trace()\n");
}

__INLINE__ Trace::~Trace()
{
    LOGD("Trace::~Trace()\n");
}

__INLINE__ Trace& Trace::get()
{
    static thread_local Trace sTrace;
    return sTrace;
}

__INLINE__ void Trace::start_method_tracing()
{
    Trace& trace = get();
    trace.mOwner = std::this_thread::get_id();
    trace.mStart = std::chrono::steady_clock::now();
}

__INLINE__ void Trace::stop_method_tracing(const char* prefix, char timeUnit)
{
    assert(prefix);

    Trace& trace = get();
    if (trace.mOwner != std::this_thread::get_id()) {
        LOGE("Only the original thread that called startMethodTracing() can be call stopMethodTracing().\n");
        assert(false);
    }

    using namespace std::chrono;
    const nanoseconds duration = steady_clock::now() - trace.mStart;
    long long runningTime;
    switch (timeUnit)
    {
    // nanoseconds
    case 'n':
        runningTime = duration.count();
        break;

    // microseconds
    case 'u':
        runningTime = duration_cast<microseconds>(duration).count();
        break;

    // milliseconds
    default:
        runningTime = duration_cast<milliseconds>(duration).count();
        break;
    }

    LOGD("%s running time = %lld%cs\n", prefix, runningTime, timeUnit);
}
#endif  // NDEBUG

}  // namespace stdutil

#endif  // __TRACE_INL__