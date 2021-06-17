///////////////////////////////////////////////////////////////////////////////
// trace.h
//
// Author : Garfield
// Creation Date : 2020/5/4

#ifndef __TRACE_H__
#define __TRACE_H__

#include "platform.h"
#include <chrono>
#include <thread>
#include <assert.h>

///////////////////////////////////////////////////////////////////////////////
// Global functions in this file:
//
// startMethodTracing()
// stopMethodTracing()
// stopMethodTracing2()

namespace stdutil {

#ifndef NDEBUG
///////////////////////////////////////////////////////////////////////////////
// Interface of the Trace class
//

class Trace final
{
// Constructors/Destructor
private:
    Trace();
    ~Trace();

// Operations
public:
    static void start_method_tracing();
    static void stop_method_tracing(const char* prefix, char timeUnit);

// Implementation
private:
    static Trace& get();

// Data members
private:
    std::thread::id mOwner;
    std::chrono::steady_clock::time_point mStart;
};
#endif  // NDEBUG

}  // namespace stdutil

#include "trace.inl"

#endif  // __TRACE_H__