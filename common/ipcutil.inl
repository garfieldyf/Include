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
// Implementation of the FileDescriptor class
//

__INLINE__ FileDescriptor::FileDescriptor(int fd/* = -1*/)
    : mFd(fd)
{
}

__INLINE__ FileDescriptor::~FileDescriptor()
{
    close();
}

__INLINE__ FileDescriptor::operator int() const
{
    return mFd;
}

__INLINE__ void FileDescriptor::close()
{
    if (mFd != -1) {
        LOGD("The FileDescriptor was closed (fd = %d)\n", mFd);
        ::close(mFd);
        mFd = -1;
    }
}

__INLINE__ void FileDescriptor::attach(int fd)
{
    assert(isEmpty());
    mFd = fd;
}

__INLINE__ ssize_t FileDescriptor::read(void* buf, size_t count) const
{
    assert(!isEmpty());
    const ssize_t readBytes = ::read(mFd, buf, count);

#ifndef NDEBUG
    if (readBytes == -1) {
        logError("The FileDescriptor read failed");
    }
#endif  // NDEBUG

    return readBytes;
}

__INLINE__ ssize_t FileDescriptor::write(const void* buf, size_t count) const
{
    assert(!isEmpty());
    const ssize_t writtenBytes = ::write(mFd, buf, count);

#ifndef NDEBUG
    if (writtenBytes == -1) {
        logError("The FileDescriptor write failed");
    }
#endif  // NDEBUG

    return writtenBytes;
}

__INLINE__ bool FileDescriptor::isEmpty() const
{
    return (mFd == -1);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the Epoll class
//

__INLINE__ Epoll::Epoll(int epollFd/* = -1*/)
    : FileDescriptor(epollFd)
{
}

__INLINE__ int Epoll::create(int size/* = 1*/)
{
    assert(isEmpty());
    mFd = ::epoll_create(size);

#ifndef NDEBUG
    if (mFd == -1) {
        logError("The Epoll create failed");
        assert(false);
    }
#endif  // NDEBUG

    return mFd;
}

__INLINE__ int Epoll::addFd(int fd, uint32_t events/* = EPOLLIN*/) const
{
    assert(!isEmpty());

    struct epoll_event event;
    event.data.fd = fd;
    event.events  = events;

    const int result = ::epoll_ctl(mFd, EPOLL_CTL_ADD, fd, &event);

#ifndef NDEBUG
    if (result == -1) {
        logError("The Epoll add fd failed");
        assert(false);
    }
#endif  // NDEBUG

    return result;
}

__INLINE__ int Epoll::removeFd(int fd) const
{
    assert(!isEmpty());
    const int result = ::epoll_ctl(mFd, EPOLL_CTL_DEL, fd, nullptr);

#ifndef NDEBUG
    if (result == -1) {
        logError("The Epoll remove fd failed");
        assert(false);
    }
#endif  // NDEBUG

    return result;
}

__INLINE__ int Epoll::wait(struct epoll_event* events, int count/* = 1*/, int timeout/* = -1*/) const
{
    assert(events);
    assert(count > 0);
    assert(!isEmpty());

    const int result = ::epoll_wait(mFd, events, count, timeout);

#ifndef NDEBUG
    if (result == -1) {
        logError("The Epoll wait failed");
        assert(false);
    }
#endif  // NDEBUG

    return result;
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the EventFd class
//

__INLINE__ EventFd::EventFd(int eventFd/* = -1*/)
    : FileDescriptor(eventFd)
{
}

__INLINE__ int EventFd::create(uint32_t initval/* = 0*/, int flags/* = EFD_NONBLOCK*/)
{
    assert(isEmpty());
    mFd = ::eventfd(initval, flags);

#ifndef NDEBUG
    if (mFd == -1) {
        logError("The EventFd create failed");
        assert(false);
    }
#endif  // NDEBUG

    return mFd;
}

__INLINE__ void EventFd::poll(int timeout/* = -1*/)
{
    assert(!isEmpty());

    struct pollfd pfd = { mFd, POLLIN };
    const int result = ::poll(&pfd, 1, timeout);

#ifndef NDEBUG
    if (result == -1) {
        logError("The EventFd poll failed");
        assert(false);
    }
#endif  // NDEBUG

    if (result > 0 && pfd.fd == mFd && (pfd.revents & POLLIN)) {
        uint64_t value;
        FileDescriptor::read(&value, sizeof(uint64_t));
    }
}

__INLINE__ ssize_t EventFd::read(uint64_t& value) const
{
    assert(!isEmpty());
    return FileDescriptor::read(&value, sizeof(uint64_t));
}

__INLINE__ ssize_t EventFd::write(uint64_t value/* = 1*/) const
{
    assert(!isEmpty());
    return FileDescriptor::write(&value, sizeof(uint64_t));
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the LocalSocket class
//

__INLINE__ LocalSocket::LocalSocket(int sockFd/* = -1*/)
    : FileDescriptor(sockFd)
{
}

__INLINE__ int LocalSocket::listen(const char* name, Namespace ns/* = Namespace::abstract*/)
{
    assert(name);
    assert(isEmpty());

    int result = -1;
    if ((mFd = ::socket(AF_UNIX, SOCK_STREAM, 0)) != -1) {
        struct sockaddr_un addr;
        const socklen_t addrlen = buildSockAddr(addr, name, ns);

        result = ::bind(mFd, reinterpret_cast<sockaddr*>(&addr), addrlen);
        if (result == 0) {
            result = ::listen(mFd, 5);
        }
    }

#ifndef NDEBUG
    if (mFd == -1 || result != 0) {
        logError("The LocalSocket listen failed");
        assert(false);
    }
#endif  // NDEBUG

    return result;
}

__INLINE__ int LocalSocket::accept() const
{
    assert(!isEmpty());

    struct sockaddr_un addr;
    socklen_t addrlen = sizeof(addr);
    const int sockFd = ::accept(mFd, reinterpret_cast<sockaddr*>(&addr), &addrlen);

#ifndef NDEBUG
    if (sockFd == -1) {
        logError("The LocalSocket accept failed");
        assert(false);
    }
#endif  // NDEBUG

    return sockFd;
}

__INLINE__ int LocalSocket::connect(const char* name, Namespace ns/* = Namespace::abstract*/)
{
    assert(name);
    assert(isEmpty());

    int result = -1;
    if ((mFd = ::socket(AF_UNIX, SOCK_STREAM, 0)) != -1) {
        struct sockaddr_un addr;
        const socklen_t addrlen = buildSockAddr(addr, name, ns);
        result = ::connect(mFd, reinterpret_cast<sockaddr*>(&addr), addrlen);
    }

#ifndef NDEBUG
    if (mFd == -1 || result != 0) {
        logError("The LocalSocket connect failed");
        assert(false);
    }
#endif // NDEBUG

    return result;
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