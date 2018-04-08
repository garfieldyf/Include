///////////////////////////////////////////////////////////////////////////////
// gestureutil.h
//
// Copyright(c) 2018, Garfield. All rights reserved.
// Author : Garfield
// Version : 1.0
// Creation Date : 2010/10/27

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __GESTUREUTIL_H__
#define __GESTUREUTIL_H__

#ifndef _WIN32_WCE
    #error gestureutil.h compiles under Windows CE only
#endif

#include "stdutil.h"
#include <gesture.h>
#include <windowautogesture.h>
#include <gesturephysicsengine.h>

///////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// CPhysicsEngine
// CPhysicsEngineInit
// CPhysicsEngineState
// CGestureInfo
// CAutoGestureInfo
// CGestureMetrics
// CAnimateMessageInfo

namespace WCE {

///////////////////////////////////////////////////////////////////////////////
// Interface of the CPhysicsEngineTraits class
//

struct CPhysicsEngineTraits : public atlutil::CHandleTraits
{
    typedef HPHYSICSENGINE HANDLET_PTR;
    static BOOL WINAPI Destroy(HANDLET_PTR hHandle);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CPhysicsEngine class
//

class CPhysicsEngine : public atlutil::CHandle<CPhysicsEngineTraits>
{
// Constructors
public:
    CPhysicsEngine();

// Operations
public:
    HRESULT Create(const PHYSICSENGINEINIT& init);

    HRESULT SetUserTime(DWORD dwTime);
    HRESULT Query(PHYSICSENGINESTATE& state);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CPhysicsEngineInit class
//

struct CPhysicsEngineInit : public PHYSICSENGINEINIT
{
// Constructors
public:
    CPhysicsEngineInit(int left, int top, int right, int bottom, int xPos, int yPos, int cxView, int cyView, LONG lInitialVelocity, DWORD dwInitialAngle, DWORD dwFlags = 0, int cxItem = 1, int cyItem = 1, BYTE bXAxisBoundaryMode = PHYSICSENGINE_BOUNDARY_MODE_RUBBERBAND, BYTE bYAxisBoundaryMode = PHYSICSENGINE_BOUNDARY_MODE_RUBBERBAND);

// Operations
public:
    operator PHYSICSENGINEINIT*();
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CPhysicsEngineState class
//

struct CPhysicsEngineState : public PHYSICSENGINESTATE
{
// Constructors
public:
    CPhysicsEngineState();

// Operations
public:
    operator PHYSICSENGINESTATE*();
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CGestureInfo class
//

struct CGestureInfo : public GESTUREINFO
{
// Constructors
public:
    CGestureInfo();

// Operations
public:
    operator PGESTUREINFO();

// Implementation
public:
    BOOL GetGestureInfo(HGESTUREINFO hGestureInfo);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CAutoGestureInfo class
//

struct CAutoGestureInfo : public WAGINFO
{
// Constructors
public:
    explicit CAutoGestureInfo(DWORD dwFlags = WAGIF_OWNERANIMATE | WAGIF_VSCROLLABLE | WAGIF_HSCROLLABLE, UINT nOwnerAnimateMessage = 0, UINT nAnimateStatusMessage = 0, UINT nItemHeight = 0, UINT nItemWidth = 0, BYTE nHorizontalExtent = 100, BYTE nVerticalExtent = 100, HBRUSH hExtentBrush = NULL);

// Operations
public:
    operator LPWAGINFO();

// Implementation
public:
    BOOL SetWindowAutoGesture(HWND hWnd);
    BOOL GetWindowAutoGesture(HWND hWnd);
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CGestureMetrics class
//

struct CGestureMetrics : public GESTUREMETRICS
{
// Constructors
public:
    explicit CGestureMetrics(DWORD uID);

// Operations
public:
    operator LPGESTUREMETRICS();

// Implementation
public:
    BOOL GetGestureMetrics();
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CAnimateMessageInfo class
//

struct CAnimateMessageInfo : public ANIMATEMESSAGEINFO
{
// Constructors
public:
    CAnimateMessageInfo();

// Operations
public:
    operator LPANIMATEMESSAGEINFO();

// Implementation
public:
    BOOL GetAnimateMessageInfo(HWND hWnd, WPARAM wParam, LPARAM lParam);
};

} // namespace WCE

#include "gestureutil.inl"

#endif // __GESTUREUTIL_H__