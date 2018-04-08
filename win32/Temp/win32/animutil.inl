///////////////////////////////////////////////////////////////////////////////
// animutil.inl
//
// Copyright(c) 2012, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.5
// Finish Time : 2010/10/19

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __ANIMUTIL_INL__
#define __ANIMUTIL_INL__

#ifndef __ANIMUTIL_H__
    #error animutil.inl requires animutil.h to be included first
#endif

namespace atlutil {

///////////////////////////////////////////////////////////////////////////////
// Implementation of the CAnimationPlayerBase class
//

inline CAnimationPlayerBase::CAnimationPlayerBase()
    : m_hWnd(NULL), m_nFlags(0), m_pEvents(NULL), m_nFrameIndex(0), m_nFrameCount(0), m_pAnimation(NULL)
{
}

inline void CAnimationPlayerBase::InitAnimation(HWND hWnd, IAnimation* pAnimation, UINT nFrameCount, IAnimationEvents* pEvents, UINT nFlags)
{
    ATLASSERT(pAnimation);
    ATLASSERT(::IsWindow(hWnd));

    m_hWnd    = hWnd;
    m_nFlags  = nFlags;
    m_pEvents = pEvents;
    m_nFrameIndex = 0;
    m_pAnimation  = pAnimation;
    m_nFrameCount = nFrameCount;

    if (nFlags & APF_SHOWWINDOW)
        ::ShowWindow(hWnd, SW_SHOW);

    pAnimation->Start(hWnd, 0, nFrameCount);
    if (m_pEvents != NULL)
        m_pEvents->OnAnimationStart(pAnimation, hWnd, nFlags);
}

inline void CAnimationPlayerBase::StopAnimation()
{
    ATLASSERT(m_pAnimation);
    ATLASSERT(::IsWindow(m_hWnd));

    m_pAnimation->Stop(m_hWnd, m_nFrameIndex, m_nFrameCount);
    if (m_nFlags & APF_HIDEWINDOW)
        ::ShowWindow(m_hWnd, SW_HIDE);

    if (m_nFlags & APF_SYNCHRONIZED)
        ATLVERIFY(::PostMessage(m_hWnd, WM_USERFIRST, 0, 0L));

    if (m_pEvents != NULL)
        m_pEvents->OnAnimationEnd(m_pAnimation, m_hWnd, m_nFlags);
}

inline void CAnimationPlayerBase::RunMessageLoop()
{
    MSG msg;
    while (::GetMessage(&msg, NULL, 0, 0) > 0 && msg.message != WM_USERFIRST)
    {
        ::TranslateMessage(&msg);
        ::DispatchMessage(&msg);
    }

    ATLTRACE(_T("CAnimationPlayer::StartAnimation() - exit.\n"));
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CAnimationPlayer class
//

template <typename _TimerTask>
inline CAnimationPlayer<_TimerTask>::CAnimationPlayer()
{
}

template <typename _TimerTask>
inline BOOL CAnimationPlayer<_TimerTask>::StartAnimation(HWND hWnd, IAnimation* pAnimation, UINT nFrameCount, IAnimationEvents* pEvents/* = NULL*/, UINT nFlags/* = APF_SYNCHRONIZED | APF_SHOWWINDOW*/, UINT nFrameDelay/* = APF_FRAME_DELAY_MS*/)
{
    ATLASSERT(pAnimation);
    ATLASSERT(::IsWindow(hWnd));
    ATLASSERT(m_timerTask.IsStopped());

    InitAnimation(hWnd, pAnimation, nFrameCount, pEvents, nFlags);
    BOOL bSuccessful = (m_timerTask.Start(this, nFrameDelay) != 0);
    if (bSuccessful && (nFlags & APF_SYNCHRONIZED))
        RunMessageLoop();

    return bSuccessful;
}

template <typename _TimerTask>
inline void CAnimationPlayer<_TimerTask>::StopAnimation()
{
    ATLASSERT(m_pAnimation);
    ATLASSERT(::IsWindow(m_hWnd));

    if (!m_timerTask.IsStopped())
    {
        m_timerTask.Stop();
        CAnimationPlayerBase::StopAnimation();
    }
}

#ifdef _CRT_DEBUG_DUMP
template <typename _TimerTask> template <typename _Pr>
inline void CAnimationPlayer<_TimerTask>::Dump(const _Pr& _Printer)
{
    m_timerTask.Dump(_Printer);
}
#endif  // _CRT_DEBUG_DUMP

template <typename _TimerTask>
inline BOOL CAnimationPlayer<_TimerTask>::IsAnimationStopped() const
{
    return m_timerTask.IsStopped();
}

template <typename _TimerTask>
inline void CAnimationPlayer<_TimerTask>::OnTimer(UINT_PTR nIDEvent, DWORD /*dwTime*/)
{
    ATLASSERT(m_pAnimation);
    ATLASSERT(::IsWindow(m_hWnd));
    ATLASSERT(nIDEvent == m_timerTask.m_nIDEvent);
    UNUSED_PARAMETER(nIDEvent);

    if (m_nFrameIndex < m_nFrameCount)
        m_pAnimation->Play(m_hWnd, ++m_nFrameIndex, m_nFrameCount);
    else
        StopAnimation();

    if (m_nFlags & APF_UPDATEWINDOW)
    {
        ATLVERIFY(::InvalidateRect(m_hWnd, NULL, TRUE));
        ATLVERIFY(::UpdateWindow(m_hWnd));
    }
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CScaleAnimation class
//

inline CScaleAnimation::CScaleAnimation(int cx/* = -1*/, int cy/* = -1*/, UINT nFlags/* = SAF_TOPLEFT*/)
    : m_nFlags(nFlags), m_sizeScale(cx, cy), m_rcOrigin(0, 0, 0, 0)
{
}

inline void CScaleAnimation::SetScale(int cx, int cy, UINT nFlags)
{
    m_nFlags = nFlags;
    m_sizeScale.SetSize(cx, cy);
}

inline void CScaleAnimation::Start(HWND hWnd, UINT /*nFrameIndex*/, UINT /*nFrameCount*/)
{
    ATLASSERT(::IsWindow(hWnd));

    ATLVERIFY(::GetWindowRect(hWnd, &m_rcOrigin));
    if (::GetWindowLong(hWnd, GWL_STYLE) & WS_CHILD)
        ::MapWindowPoints(NULL, ::GetParent(hWnd), (LPPOINT)&m_rcOrigin, 2);

    if (m_sizeScale.cx == -1)
        m_sizeScale.cx = m_rcOrigin.Width();
    if (m_sizeScale.cy == -1)
        m_sizeScale.cy = m_rcOrigin.Height();
}

inline void CScaleAnimation::Play(HWND hWnd, UINT nFrameIndex, UINT nFrameCount)
{
    ATLASSERT(nFrameCount > 0);
    ATLASSERT(::IsWindow(hWnd));

    PlayAnimation(hWnd, ::MulDiv(nFrameIndex, m_sizeScale.cx - m_rcOrigin.Width(), nFrameCount), ::MulDiv(nFrameIndex, m_sizeScale.cy - m_rcOrigin.Height(), nFrameCount));
}

inline void CScaleAnimation::Stop(HWND hWnd, UINT /*nFrameIndex*/, UINT /*nFrameCount*/)
{
    ATLASSERT(::IsWindow(hWnd));

    if (m_nFlags & APF_FILLAFTER)
        PlayAnimation(hWnd, m_sizeScale.cx - m_rcOrigin.Width(), m_sizeScale.cy - m_rcOrigin.Height());
    else
        ATLVERIFY(::SetWindowPos(hWnd, NULL, m_rcOrigin.left, m_rcOrigin.top, m_rcOrigin.Width(), m_rcOrigin.Height(), SWP_NOZORDER | SWP_NOACTIVATE));
}

inline void CScaleAnimation::PlayAnimation(HWND hWnd, int dx, int dy)
{
    ATLASSERT(::IsWindow(hWnd));

    CRect rect(m_rcOrigin);
    switch (m_nFlags & SAF_MASK)
    {
    case SAF_TOPRIGHT:
        rect.left -= dx, rect.bottom += dy;
        break;

    case SAF_BOTTOMLEFT:
        rect.right += dx, rect.top -= dy;
        break;

    case SAF_BOTTOMRIGHT:
        rect.left -= dx, rect.top -= dy;
        break;

    case SAF_CENTERCORNER:
        dx /= 2, dy /= 2;
        rect.InflateRect(dx, dy, dx, dy);
        break;

    default:
        rect.right += dx, rect.bottom += dy;
    }

    ATLVERIFY(::SetWindowPos(hWnd, NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOZORDER | SWP_NOACTIVATE));
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CTranslateAnimation class
//

inline CTranslateAnimation::CTranslateAnimation(int dx/* = 0*/, int dy/* = 0*/, UINT nFlags/* = 0*/)
    : m_nFlags(nFlags), m_ptOrigin(0, 0), m_sizeOffset(dx, dy)
{
}

inline void CTranslateAnimation::SetOffset(int dx, int dy, UINT nFlags)
{
    m_nFlags = nFlags;
    m_sizeOffset.SetSize(dx, dy);
}

inline void CTranslateAnimation::Start(HWND hWnd, UINT /*nFrameIndex*/, UINT /*nFrameCount*/)
{
    ATLASSERT(::IsWindow(hWnd));

    sysutil::CWindowRect rcWindow(hWnd);
    m_ptOrigin.SetPoint(rcWindow.left, rcWindow.top);

    if (::GetWindowLong(hWnd, GWL_STYLE) & WS_CHILD)
        ::MapWindowPoints(NULL, ::GetParent(hWnd), &m_ptOrigin, 1);
}

inline void CTranslateAnimation::Play(HWND hWnd, UINT nFrameIndex, UINT nFrameCount)
{
    ATLASSERT(nFrameCount > 0);
    ATLASSERT(::IsWindow(hWnd));

    int x = m_ptOrigin.x + ::MulDiv(nFrameIndex, m_sizeOffset.cx, nFrameCount);
    int y = m_ptOrigin.y + ::MulDiv(nFrameIndex, m_sizeOffset.cy, nFrameCount);

    ATLVERIFY(::SetWindowPos(hWnd, NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE));
}

inline void CTranslateAnimation::Stop(HWND hWnd, UINT /*nFrameIndex*/, UINT /*nFrameCount*/)
{
    ATLASSERT(::IsWindow(hWnd));

    if (m_nFlags & APF_FILLAFTER)
        m_ptOrigin += m_sizeOffset;

    ATLVERIFY(::SetWindowPos(hWnd, NULL, m_ptOrigin.x, m_ptOrigin.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE));
}

}  // namespace atlutil

#endif  // __ANIMUTIL_INL__