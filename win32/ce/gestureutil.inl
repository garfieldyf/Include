///////////////////////////////////////////////////////////////////////////////
// gestureutil.inl
//
// Copyright(c) 2019, Garfield. All rights reserved.
// Author : Garfield
// Version : 1.0
// Creation Date : 2010/10/27

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __GESTUREUTIL_INL__
#define __GESTUREUTIL_INL__

#ifndef __GESTUREUTIL_H__
    #error gestureutil.inl requires gestureutil.h to be included first
#endif

namespace WCE {

///////////////////////////////////////////////////////////////////////////////
// Implementation of the CPhysicsEngineTraits class
//

inline BOOL WINAPI CPhysicsEngineTraits::Destroy(HANDLET_PTR hHandle)
{
    return SUCCEEDED(::TKDestroyPhysicsEngine(hHandle));
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CPhysicsEngine class
//

inline CPhysicsEngine::CPhysicsEngine()
{
}

inline HRESULT CPhysicsEngine::Create(const PHYSICSENGINEINIT& init)
{
    ATLASSERT(m_hHandle == NULL);
    return ::TKCreatePhysicsEngine(&init, &m_hHandle);
}

inline HRESULT CPhysicsEngine::SetUserTime(DWORD dwTime)
{
    ATLASSERT(m_hHandle);
    return ::TKSetPhysicsEngineUserTime(m_hHandle, dwTime);
}

inline HRESULT CPhysicsEngine::Query(PHYSICSENGINESTATE& state)
{
    ATLASSERT(m_hHandle);
    return ::TKQueryPhysicsEngine(m_hHandle, &state);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CPhysicsEngineInit class
//

inline CPhysicsEngineInit::CPhysicsEngineInit(int left, int top, int right, int bottom, int xPos, int yPos, int cxView, int cyView, LONG lInitialVelocity, DWORD dwInitialAngle, DWORD dwFlags/* = 0*/, int cxItem/* = 1*/, int cyItem/* = 1*/, BYTE bXAxisBoundaryMode/* = PHYSICSENGINE_BOUNDARY_MODE_RUBBERBAND*/, BYTE bYAxisBoundaryMode/* = PHYSICSENGINE_BOUNDARY_MODE_RUBBERBAND*/)
{
    ATLASSERT(dwInitialAngle <= 65536);

    this->cbSize  = sizeof(PHYSICSENGINEINIT);
    this->dwFlags = dwFlags;
    this->dwEngineType = 0;
    this->sizeView.cx  = cxView;
    this->sizeView.cy  = cyView;
    this->sizeItem.cx  = cxItem;
    this->sizeItem.cy  = cyItem;
    this->ptInitialPosition.x = xPos;
    this->ptInitialPosition.y = yPos;
    this->dwInitialAngle      = dwInitialAngle;
    this->lInitialVelocity    = lInitialVelocity;
    this->bXAxisBoundaryMode  = bXAxisBoundaryMode;
    this->bYAxisBoundaryMode  = bYAxisBoundaryMode;
    this->bYAxisMovementMode  = this->bXAxisMovementMode = PHYSICSENGINE_MOVEMENT_MODE_DECELERATE;
    stdutil::SetRect(rcBoundary, left, top, right, bottom);
}

inline CPhysicsEngineInit::operator PHYSICSENGINEINIT*()
{
    return this;
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CPhysicsEngineState class
//

inline CPhysicsEngineState::CPhysicsEngineState()
{
    cbSize = sizeof(PHYSICSENGINESTATE);
}

inline CPhysicsEngineState::operator PHYSICSENGINESTATE*()
{
    return this;
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CGestureInfo class
//

inline CGestureInfo::CGestureInfo()
{
    cbSize = sizeof(GESTUREINFO);
}

inline CGestureInfo::operator PGESTUREINFO()
{
    return this;
}

inline BOOL CGestureInfo::GetGestureInfo(HGESTUREINFO hGestureInfo)
{
    ATLASSERT(hGestureInfo);
    return ::TKGetGestureInfo(hGestureInfo, this);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CAutoGestureInfo class
//

inline CAutoGestureInfo::CAutoGestureInfo(DWORD dwFlags/* = WAGIF_OWNERANIMATE | WAGIF_VSCROLLABLE | WAGIF_HSCROLLABLE*/, UINT nOwnerAnimateMessage/* = 0*/, UINT nAnimateStatusMessage/* = 0*/, UINT nItemHeight/* = 0*/, UINT nItemWidth/* = 0*/, BYTE nHorizontalExtent/* = 100*/, BYTE nVerticalExtent/* = 100*/, HBRUSH hExtentBrush/* = NULL*/)
{
    this->cbSize  = sizeof(WAGINFO);
    this->dwFlags = dwFlags;
    this->nItemWidth   = nItemWidth;
    this->nItemHeight  = nItemHeight;
    this->hExtentBrush = hExtentBrush;
    this->bVerticalExtent   = nVerticalExtent;
    this->bHorizontalExtent = nHorizontalExtent;
    this->nOwnerAnimateMessage  = nOwnerAnimateMessage;
    this->nAnimateStatusMessage = nAnimateStatusMessage;
}

inline CAutoGestureInfo::operator LPWAGINFO()
{
    return this;
}

inline BOOL CAutoGestureInfo::SetWindowAutoGesture(HWND hWnd)
{
    ATLASSERT(::IsWindow(hWnd));
    return ::TKSetWindowAutoGesture(hWnd, this);
}

inline BOOL CAutoGestureInfo::GetWindowAutoGesture(HWND hWnd)
{
    ATLASSERT(::IsWindow(hWnd));
    return ::TKGetWindowAutoGesture(hWnd, this);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CGestureMetrics class
//

inline CGestureMetrics::CGestureMetrics(DWORD uID)
{
    dwID = uID;
    cbSize = sizeof(GESTUREMETRICS);
}

inline CGestureMetrics::operator LPGESTUREMETRICS()
{
    return this;
}

inline BOOL CGestureMetrics::GetGestureMetrics()
{
    return ::TKGetGestureMetrics(this);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CAnimateMessageInfo class
//

inline CAnimateMessageInfo::CAnimateMessageInfo()
{
    cbSize = sizeof(ANIMATEMESSAGEINFO);
}

inline CAnimateMessageInfo::operator LPANIMATEMESSAGEINFO()
{
    return this;
}

inline BOOL CAnimateMessageInfo::GetAnimateMessageInfo(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
    ATLASSERT(::IsWindow(hWnd));
    return ::TKGetAnimateMessageInfo(hWnd, wParam, lParam, this);
}

} // namespace WCE

#endif // __GESTUREUTIL_INL__