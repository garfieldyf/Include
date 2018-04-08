///////////////////////////////////////////////////////////////////////////////
// timerutil.h
//
// Copyright(c) 2012, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.0
// Finish Time : 2010/6/20

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __TIMERUTIL_H__
#define __TIMERUTIL_H__

#include "atlsyncolls.h"

///////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// CTimerTask
// ITimerHandler

namespace atlutil {

///////////////////////////////////////////////////////////////////////////////
// Interface of the ITimerHandler class
//

interface ATL_NO_VTABLE ITimerHandler
{
    virtual void OnTimer(UINT_PTR nIDEvent) = 0;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CTimerTask class
//

class CTimerTask : public CThreadBase
{
// Constructors/Destructor
public:
    CTimerTask();
    ~CTimerTask();

// Operations
public:
    BOOL Start(ITimerHandler* pHandler, UINT_PTR nIDEvent, UINT nDelay, BOOL bFixedDelay = TRUE, DWORD dwCreationFlags = 0);
    BOOL Stop(DWORD dwTimeout = INFINITE);

    BOOL Cancel() const;
    void Set(UINT nDelay);

// Attributes
public:
    BOOL IsCancelled() const;
    UINT_PTR GetEventId() const;

// Implementation
protected:
    static DWORD WINAPI FixedTimerProc(LPVOID pParameter);
    static DWORD WINAPI DelayTimerProc(LPVOID pParameter);

// Data members
protected:
    HANDLE m_hEvent;
    UINT_PTR m_nIDEvent;
    volatile UINT m_nDelay;
    ITimerHandler* m_pHandler;
};

}  // namespace atlutil

#include "timerutil.inl"

#endif  // __TIMERUTIL_H__