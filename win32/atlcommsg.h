///////////////////////////////////////////////////////////////////////////////
// atlcommsg.h
//
// Author : Garfield
// Creation Date : 2009/9/20

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __ATLCOMMSG_H__
#define __ATLCOMMSG_H__

#ifndef __ATLAPP_H__
    #error atlcommsg.h requires WTL - atlapp.h to be included first
#endif

///////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// IEventHandler
// CEventMessageLoop

namespace atlutil {

///////////////////////////////////////////////////////////////////////////////
// Interface of the IEventHandler class
//

interface ATL_NO_VTABLE IEventHandler
{
    virtual void OnEvent(HANDLE hObject, BOOL bAbandoned) = 0;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the CEventMessageLoop class
//

class CEventMessageLoop : public WTL::CMessageLoop
{
// Constructors
public:
    CEventMessageLoop();

// Operations
public:
    int RunEx();

    BOOL AddEventHandler(HANDLE hObject, IEventHandler* pEvent);
    BOOL RemoveEventHandler(HANDLE hObject);

// Implementation
private:
    CEventMessageLoop(const CEventMessageLoop& that);
    CEventMessageLoop& operator=(const CEventMessageLoop& that);

// Data members
public:
    ATL::CSimpleMap<HANDLE, IEventHandler*> m_mapHandlers;
};

}  // namespace atlutil

#include "atlcommsg.inl"

#endif  // __ATLCOMMSG_H__