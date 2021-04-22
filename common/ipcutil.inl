///////////////////////////////////////////////////////////////////////////////
// ipcutil.inl
//
// Author : Garfield
// Creation Date : 2020/4/20

#ifndef __IPCUTIL_INL__
#define __IPCUTIL_INL__

#ifndef __IPCUTIL_H__
    #error ipcutil.inl requires ipcutil.h to be included first
#endif

namespace stdutil {

///////////////////////////////////////////////////////////////////////////////
// Global functions
//

#ifndef NDEBUG
__STATIC_INLINE__ void logError(const char* msg)
{
    const int errnum = errno;
    char error[256];
    ::strerror_r(errnum, error, sizeof(error));
    LOGE("%s - errno = %d, error = %s\n", msg, errnum, error);
}
#endif  // NDEBUG


///////////////////////////////////////////////////////////////////////////////
// Implementation of the LocalSocket class
//

__INLINE__ LocalSocket::LocalSocket()
    : mSockFd(-1)
{
}

__INLINE__ LocalSocket::~LocalSocket()
{
    close();
}

__INLINE__ int LocalSocket::listen(const char* name, Namespace ns/* = Namespace::abstract*/)
{
    assert(name);
    assert(isEmpty());

    int result = -1;
    if ((mSockFd = ::socket(AF_UNIX, SOCK_STREAM, 0)) != -1) {
        struct sockaddr_un addr;
        const socklen_t addrlen = buildSockAddr(addr, name, ns);

        result = ::bind(mSockFd, reinterpret_cast<sockaddr*>(&addr), addrlen);
        if (result == 0) {
            result = ::listen(mSockFd, 5);
        }
    }

#ifndef NDEBUG
    if (mSockFd == -1 || result != 0) {
        logError("The LocalSocket listen failed");
        assert(false);
    }
#endif  // NDEBUG

    return result;
}

__INLINE__ int LocalSocket::connect(const char* name, Namespace ns/* = Namespace::abstract*/)
{
    assert(name);
    assert(isEmpty());

    int result = -1;
    if ((mSockFd = ::socket(AF_UNIX, SOCK_STREAM, 0)) != -1) {
        struct sockaddr_un addr;
        const socklen_t addrlen = buildSockAddr(addr, name, ns);
        result = ::connect(mSockFd, reinterpret_cast<sockaddr*>(&addr), addrlen);
    }

#ifndef NDEBUG
    if (mSockFd == -1 || result != 0) {
        logError("The LocalSocket connect failed");
        assert(false);
    }
#endif // NDEBUG

    return result;
}

__INLINE__ ssize_t LocalSocket::send(const void* buf, size_t size) const
{
    assert(buf);
    assert(size > 0);
    assert(!isEmpty());

#ifndef NDEBUG
    const ssize_t sendBytes = ::send(mSockFd, buf, size, 0);
    if (sendBytes == -1) {
        logError("The LocalSocket send failed");
    }

    return sendBytes;
#else
    return ::send(mSockFd, buf, size, 0);
#endif  // NDEBUG
}

__INLINE__ ssize_t LocalSocket::recv(void* buf, size_t size) const
{
    assert(buf);
    assert(size > 0);
    assert(!isEmpty());

#ifndef NDEBUG
    const ssize_t recvBytes = ::recv(mSockFd, buf, size, 0);
    if (recvBytes == -1) {
        logError("The LocalSocket recv failed");
    }

    return recvBytes;
#else
    return ::recv(mSockFd, buf, size, 0);
#endif  // NDEBUG
}

__INLINE__ int LocalSocket::accept()
{
    assert(!isEmpty());
    struct sockaddr_un addr;
    socklen_t addrlen = sizeof(addr);

#ifndef NDEBUG
    const int sockFd = ::accept(mSockFd, reinterpret_cast<sockaddr*>(&addr), &addrlen);
    if (sockFd == -1) {
        logError("The LocalSocket accept failed");
        assert(false);
    }

    return sockFd;
#else
    return ::accept(mSockFd, reinterpret_cast<sockaddr*>(&addr), &addrlen);
#endif  // NDEBUG
}

__INLINE__ void LocalSocket::close()
{
    if (mSockFd != -1) {
        LOGD("The LocalSocket was closed [%d]\n", mSockFd);
        ::shutdown(mSockFd, SHUT_RDWR);
        ::close(mSockFd);
        mSockFd = -1;
    }
}

__INLINE__ void LocalSocket::attach(int sockFd)
{
    assert(isEmpty());
    mSockFd = sockFd;
}

__INLINE__ bool LocalSocket::isEmpty() const
{
    return (mSockFd == -1);
}

__INLINE__ socklen_t LocalSocket::buildSockAddr(struct sockaddr_un& addr, const char* name, Namespace ns)
{
    assert(name);

    size_t length;
    if (ns == Namespace::filesystem) {
        length = ::strlcpy(addr.sun_path, name, sizeof(addr.sun_path));
    } else {
        addr.sun_path[0] = '\0';
        length = ::strlcpy(addr.sun_path + 1, name, sizeof(addr.sun_path) - 1);
    }

    addr.sun_family = AF_UNIX;
    return offsetof(struct sockaddr_un, sun_path) + length + 1;
}

}  // namespace stdutil

#endif  // __IPCUTIL_INL__