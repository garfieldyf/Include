///////////////////////////////////////////////////////////////////////////////
// cemsgutil.inl
//
// Author : Garfield
// Creation Date : 2010/10/21

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __CEMSGUTIL_INL__
#define __CEMSGUTIL_INL__

#ifndef __CEMSGUTIL_H__
    #error cemsgutil.inl requires cemsgutil.h to be included first
#endif

namespace WCE {

///////////////////////////////////////////////////////////////////////////////
// Implementation of the CMessageQueueTraits class
//

inline BOOL WINAPI CMessageQueueTraits::Destroy(HANDLET_PTR hHandle)
{
    return ::CloseMsgQueue(hHandle);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CMessageQueue class
//

inline CMessageQueue::CMessageQueue()
{
}

inline HANDLE CMessageQueue::Create(DWORD cbMaxMessage, BOOL bReadAccess/* = TRUE*/, DWORD dwMaxMessages/* = 0*/, DWORD dwFlags/* = MSGQUEUE_NOPRECOMMIT*/, LPCWSTR pszName/* = NULL*/)
{
    ATLASSERT(m_hHandle == NULL);

    MSGQUEUEOPTIONS options = { sizeof(MSGQUEUEOPTIONS), dwFlags, dwMaxMessages, cbMaxMessage, bReadAccess };
    return (m_hHandle = ::CreateMsgQueue(pszName, &options));
}

inline HANDLE CMessageQueue::Open(HANDLE hProccess, HANDLE hMsgQueue, BOOL bReadAccess/* = TRUE*/)
{
    ATLASSERT(hMsgQueue);
    ATLASSERT(m_hHandle == NULL);

    MSGQUEUEOPTIONS options = { sizeof(MSGQUEUEOPTIONS), 0, 0, 0, bReadAccess };
    return (m_hHandle = ::OpenMsgQueue(hProccess, hMsgQueue, &options));
}

inline BOOL CMessageQueue::Read(LPVOID pBuffer, DWORD cbBufferSize, DWORD& dwFlags, DWORD& dwNumberOfBytesRead, DWORD dwTimeout/* = INFINITE*/) const
{
    ATLASSERT(pBuffer);
    ATLASSERT(m_hHandle);
    ATLASSERT(cbBufferSize != 0);

    return ::ReadMsgQueue(m_hHandle, pBuffer, cbBufferSize, &dwNumberOfBytesRead, dwTimeout, &dwFlags);
}

inline BOOL CMessageQueue::Write(LPCVOID pBuffer,DWORD cbBufferSize, DWORD dwFlags/* = 0*/, DWORD dwTimeout/* = INFINITE*/) const
{
    ATLASSERT(pBuffer);
    ATLASSERT(m_hHandle);
    ATLASSERT(cbBufferSize != 0);

    return ::WriteMsgQueue(m_hHandle, (LPVOID)pBuffer, cbBufferSize, dwTimeout, dwFlags);
}

inline DWORD CMessageQueue::GetMaxMessageSize() const
{
    ATLASSERT(m_hHandle);

    MSGQUEUEINFO msgQueueInfo;
    msgQueueInfo.dwSize = sizeof(MSGQUEUEINFO), msgQueueInfo.cbMaxMessage = 0;
    ATLVERIFY(::GetMsgQueueInfo(m_hHandle, &msgQueueInfo));

    return msgQueueInfo.cbMaxMessage;
}

inline BOOL CMessageQueue::GetQueueInfo(MSGQUEUEINFO& msgQueueInfo) const
{
    ATLASSERT(m_hHandle);
    ATLASSERT(msgQueueInfo.dwSize == sizeof(MSGQUEUEINFO));

    return ::GetMsgQueueInfo(m_hHandle, &msgQueueInfo);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CMessageLoop class
//

inline CMessageLoop::CMessageLoop()
{
}

inline BOOL CMessageLoop::Create(LPCWSTR pszMsgQueueName)
{
    ATLASSERT(pszMsgQueueName);
    return (m_msgQueue.Create(0, TRUE, 0, 0, pszMsgQueueName) != NULL);
}

inline BOOL CMessageLoop::Create(HANDLE hMsgQueue, HANDLE hProccess/* = ::GetCurrentProcess()*/)
{
    ATLASSERT(hMsgQueue);
    ATLASSERT(hProccess);

    return (m_msgQueue.Open(hProccess, hMsgQueue) != NULL);
}

inline void CMessageLoop::Destroy()
{
    m_msgQueue.Destroy();
}

inline void CMessageLoop::Run(IMessageHandler* pHandler)
{
    ATLASSERT(pHandler);
    ATLASSERT(!m_msgQueue.IsEmpty());

    const DWORD cbSize = m_msgQueue.GetMaxMessageSize();
    ATLASSERT(cbSize >= sizeof(MSGQUEUE_MESSAGE));

    ATL::CTempBuffer<MSGQUEUE_MESSAGE, 256> buffer;
    LPMSGQUEUE_MESSAGE pMsg = buffer.AllocateBytes(cbSize);
    ATLASSERT(pMsg);

    DWORD dwFlags = 0, cbRead = 0;
    while (m_msgQueue.Read(pMsg, cbSize, dwFlags, cbRead) && dwFlags != MSGQUEUE_MSGQUIT)
    {
        ATLASSERT(cbRead >= sizeof(MSGQUEUE_MESSAGE));
        pHandler->OnProcessMessage(pMsg, cbRead, dwFlags);
    }

    ATLTRACE(_T("CMessageLoop::Run() - exiting the message loop.\n"));
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the CMessageThread class
//

inline CMessageThread::CMessageThread()
    : m_hThread(NULL), m_pHandler(NULL)
{
}

inline CMessageThread::~CMessageThread()
{
    ATLASSERT(m_hThread == NULL);
    ATLASSERT(m_msgQueue.IsEmpty());
}

inline BOOL CMessageThread::Create(IMessageHandler* pHandler, DWORD cbMaxMessage, DWORD dwMaxMessages/* = 0*/, DWORD dwFlags/* = MSGQUEUE_NOPRECOMMIT*/, DWORD dwThreadFlags/* = 0*/)
{
    ATLASSERT(pHandler);
    ATLASSERT(m_hThread == NULL);
    ATLASSERT(m_msgQueue.IsEmpty());
    ATLASSERT(cbMaxMessage >= sizeof(MSGQUEUE_MESSAGE));

    m_pHandler = pHandler;
    if (m_msgQueue.Create(cbMaxMessage, FALSE, dwMaxMessages, dwFlags))
        m_hThread = _CreateThread(NULL, 0, ThreadProc, this, dwThreadFlags, NULL);

    return (m_hThread != NULL);
}

inline BOOL CMessageThread::Quit(DWORD dwTimeout/* = INFINITE*/)
{
    BOOL bSucceessful = TRUE;
    if (m_hThread != NULL)
    {
        ATLVERIFY(PostMessage(0U, MSGQUEUE_MSGQUIT));
        bSucceessful = (::WaitForSingleObject(m_hThread, dwTimeout) == WAIT_OBJECT_0);
        ATLVERIFY(::CloseHandle(m_hThread));

        m_msgQueue.Destroy(), m_hThread = NULL;
        ATLTRACE(_T("CMessageThread::Quit() - quitting the message loop.\n"));
    }

    return bSucceessful;
}

inline BOOL CMessageThread::PostMessage(UINT message, DWORD dwFlags/* = 0*/, DWORD dwTimeout/* = INFINITE*/) const
{
    ATLASSERT(m_hThread);
    ATLASSERT(!m_msgQueue.IsEmpty());

    return m_msgQueue.Write(&message, sizeof(UINT), dwFlags, dwTimeout);
}

inline BOOL CMessageThread::PostMessage(const MSGQUEUE_MESSAGE* pMsg, DWORD cbSize, DWORD dwFlags/* = 0*/, DWORD dwTimeout/* = INFINITE*/) const
{
    ATLASSERT(pMsg);
    ATLASSERT(m_hThread);
    ATLASSERT(!m_msgQueue.IsEmpty());
    ATLASSERT(cbSize >= sizeof(MSGQUEUE_MESSAGE));
    ATLASSERT(m_msgQueue.GetMaxMessageSize() >= cbSize);

    return m_msgQueue.Write(pMsg, cbSize, dwFlags, dwTimeout);
}

inline CMessageThread::operator HANDLE() const
{
    return m_hThread;
}

inline int CMessageThread::GetPriority() const
{
    ATLASSERT(m_hThread);
    return ::GetThreadPriority(m_hThread);
}

inline BOOL CMessageThread::SetPriority(int nPriority)
{
    ATLASSERT(m_hThread);
    return ::SetThreadPriority(m_hThread, nPriority);
}

inline DWORD WINAPI CMessageThread::ThreadProc(LPVOID pParameter)
{
    CMessageThread* pThis = reinterpret_cast<CMessageThread*>(pParameter);
    ATLASSERT(pThis);
    ATLASSERT(pThis->m_pHandler);
    ATLASSERT(!pThis->m_msgQueue.IsEmpty());

    CMessageLoop msgLoop;
    if (msgLoop.Create(pThis->m_msgQueue))
        msgLoop.Run(pThis->m_pHandler);

    ATLTRACE(_T("CMessageThread::ThreadProc() - exiting the message thread.\n"));
    return 0;
}

}  // namespace WCE

#endif  // __CEMSGUTIL_INL__