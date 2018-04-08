///////////////////////////////////////////////////////////////////////////////
// animutil.h
//
// Copyright(c) 2012, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.5
// Finish Time : 2010/10/19

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __ANIMUTIL_H__
#define __ANIMUTIL_H__

#include "sysutil.h"
#include "atlsyncutil.h"

///////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// IAnimation
// IAnimationEvents
// CAnimationPlayer
// CScaleAnimation
// CTranslateAnimation

namespace atlutil {

enum
{
    // CScaleAnimation used
    SAF_MASK         = 0x0F,
    SAF_TOPLEFT      = 0x00,
    SAF_TOPRIGHT     = 0x01,
    SAF_BOTTOMLEFT   = 0x02,
    SAF_BOTTOMRIGHT  = 0x03,
    SAF_CENTERCORNER = 0x04,

    // CAnimationPlayer used
    APF_FRAME_DELAY_MS = 0x22,
    APF_SHOWWINDOW     = 0x01,
    APF_HIDEWINDOW     = 0x02,
    APF_UPDATEWINDOW   = 0x04,
    APF_SYNCHRONIZED   = 0x08,
    APF_FILLAFTER      = 0x10,
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the IAnimation class
//

interface ATL_NO_VTABLE IAnimation
{
    virtual void Start(HWND hWnd, UINT nFrameIndex, UINT nFrameCount) = 0;
    virtual void Play(HWND hWnd, UINT nFrameIndex, UINT nFrameCount) = 0;
    virtual void Stop(HWND hWnd, UINT nFrameIndex, UINT nFrameCount) = 0;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the IAnimationEvents class
//

interface ATL_NO_VTABLE IAnimationEvents
{
    virtual void OnAnimationStart(IAnimation* pAnimation, HWND hWnd, UINT nFlags) = 0;
    virtual void OnAnimationEnd(IAnimation* pAnimation, HWND hWnd, UINT nFlags) = 0;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CAnimationPlayerBase class
//

class ATL_NO_VTABLE CAnimationPlayerBase
{
// Constructors
protected:
    CAnimationPlayerBase();

// Implementation
protected:
    void InitAnimation(HWND hWnd, IAnimation* pAnimation, UINT nFrameCount, IAnimationEvents* pEvents, UINT nFlags);
    void StopAnimation();

    void RunMessageLoop();

// Data members
protected:
    HWND m_hWnd;
    UINT m_nFlags;
    UINT m_nFrameIndex;
    UINT m_nFrameCount;
    IAnimation* m_pAnimation;
    IAnimationEvents* m_pEvents;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CAnimationPlayer class
//

template <typename _TimerTask = CTimerTask>
class CAnimationPlayer : public CAnimationPlayerBase, public ITimerHandler
{
// Constructors
public:
    CAnimationPlayer();

// Operations
public:
    BOOL StartAnimation(HWND hWnd, IAnimation* pAnimation, UINT nFrameCount, IAnimationEvents* pEvents = NULL, UINT nFlags = APF_SYNCHRONIZED | APF_SHOWWINDOW, UINT nFrameDelay = APF_FRAME_DELAY_MS);
    void StopAnimation();

#ifdef _CRT_DEBUG_DUMP
    template <typename _Pr> void Dump(const _Pr& _Printer);
#endif

// Attributes
public:
    BOOL IsAnimationStopped() const;

// Overrides
public:
    virtual void OnTimer(UINT_PTR nIDEvent, DWORD dwTime);

// Data members
protected:
    _TimerTask m_timerTask;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CScaleAnimation class
//

class CScaleAnimation : public IAnimation
{
// Constructors
public:
    explicit CScaleAnimation(int cx = -1, int cy = -1, UINT nFlags = SAF_TOPLEFT);

// Operations
public:
    void SetScale(int cx, int cy, UINT nFlags);

// Overrides
public:
    virtual void Start(HWND hWnd, UINT nFrameIndex, UINT nFrameCount);
    virtual void Play(HWND hWnd, UINT nFrameIndex, UINT nFrameCount);
    virtual void Stop(HWND hWnd, UINT nFrameIndex, UINT nFrameCount);

// Implementation
protected:
    void PlayAnimation(HWND hWnd, int dx, int dy);

// Data members
protected:
    UINT  m_nFlags;
    CRect m_rcOrigin;
    CSize m_sizeScale;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CTranslateAnimation class
//

class CTranslateAnimation : public IAnimation
{
// Constructors
public:
    explicit CTranslateAnimation(int dx = 0, int dy = 0, UINT nFlags = 0);

// Operations
public:
    void SetOffset(int dx, int dy, UINT nFlags);

// Overrides
public:
    virtual void Start(HWND hWnd, UINT nFrameIndex, UINT nFrameCount);
    virtual void Play(HWND hWnd, UINT nFrameIndex, UINT nFrameCount);
    virtual void Stop(HWND hWnd, UINT nFrameIndex, UINT nFrameCount);

// Data members
protected:
    UINT m_nFlags;
    CPoint m_ptOrigin;
    CSize  m_sizeOffset;
};

}  // namespace atlutil

#include "animutil.inl"

#endif  // __ANIMUTIL_H__