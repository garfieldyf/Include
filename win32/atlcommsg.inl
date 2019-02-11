///////////////////////////////////////////////////////////////////////////////
// atlcommsg.inl
//
// Copyright(c) 2019, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.0
// Creation Date : 2009/9/20

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __ATLCOMMSG_INL__
#define __ATLCOMMSG_INL__

#ifndef __ATLCOMMSG_H__
    #error atlcommsg.inl requires atlcommsg.h to be included first
#endif

namespace atlutil {

///////////////////////////////////////////////////////////////////////////////
// Implementation of the CEventMessageLoop class
//

inline CEventMessageLoop::CEventMessageLoop()
{
}

inline int CEventMessageLoop::RunEx()
{
    DWORD dwWaitObject = 0;
    for (; ;)
    {
        dwWaitObject = ::MsgWaitForMultipleObjects(m_mapHandlers.m_nSize, m_mapHandlers.m_aKey, FALSE, INFINITE, QS_ALLINPUT);
        if (dwWaitObject == WAIT_OBJECT_0 + (DWORD)m_mapHandlers.m_nSize)
        {
            // Receives new message.
            while (::PeekMessage(&m_msg, NULL, 0, 0, PM_REMOVE))
            {
                // Exit RunEx, if received WM_QUIT message.
                if (m_msg.message == WM_QUIT)
                    return (int)m_msg.wParam;

                // Processing message.
                if (!PreTranslateMessage(&m_msg))
                {
                    ::TranslateMessage(&m_msg);
                    ::DispatchMessage(&m_msg);
                }
            }

            // Processing idle.
            OnIdle(0);
        }
        else if (dwWaitObject >= WAIT_OBJECT_0 && dwWaitObject < WAIT_OBJECT_0 + (DWORD)m_mapHandlers.m_nSize)
        {
            // Waiting successful, the specified object is signaled.
            ATLASSERT(m_mapHandlers.m_aVal[dwWaitObject - WAIT_OBJECT_0]);
            m_mapHandlers.m_aVal[dwWaitObject - WAIT_OBJECT_0]->OnEvent(m_mapHandlers.m_aKey[dwWaitObject - WAIT_OBJECT_0], FALSE);
        }
        else if (dwWaitObject >= WAIT_ABANDONED_0 && dwWaitObject < WAIT_ABANDONED_0 + (DWORD)m_mapHandlers.m_nSize)
        {
            // Waiting abandoned, the specified mutex is abandoned.
            ATLASSERT(m_mapHandlers.m_aVal[dwWaitObject - WAIT_ABANDONED_0]);
            m_mapHandlers.m_aVal[dwWaitObject - WAIT_ABANDONED_0]->OnEvent(m_mapHandlers.m_aKey[dwWaitObject - WAIT_ABANDONED_0], TRUE);
        }
    #ifdef _DEBUG
        else
        {
            // Waiting failed.
            ATLTRACE(_T("CEventMessageLoop::RunEx - wait failed(error)\n"));
            ATLASSERT(FALSE);
        }
    #endif  // _DEBUG
    }
}

inline BOOL CEventMessageLoop::AddEventHandler(HANDLE hObject, IEventHandler* pEvent)
{
    ATLASSERT(pEvent);
    ATLASSERT(hObject);
    ATLASSERT(m_mapHandlers.GetSize() <= MAXIMUM_WAIT_OBJECTS);

    return m_mapHandlers.Add(hObject, pEvent);
}

inline BOOL CEventMessageLoop::RemoveEventHandler(HANDLE hObject)
{
    ATLASSERT(hObject);
    return m_mapHandlers.Remove(hObject);
}

}  // namespace atlutil

#endif  // __ATLCOMMSG_INL__