///////////////////////////////////////////////////////////////////////////////
// syncutil.inl
//
// Author : Garfield
// Creation Date : 2020/4/20

#ifndef __SYNCUTIL_INL__
#define __SYNCUTIL_INL__

#ifndef __SYNCUTIL_H__
    #error syncutil.inl requires syncutil.h to be included first
#endif

namespace stdutil {

///////////////////////////////////////////////////////////////////////////////
// Global functions
//

#ifndef NDEBUG
#define startMethodTracing()                        stdutil::_Trace::start_method_tracing()
#define stopMethodTracing(_Prefix)                  stopMethodTracing2(_Prefix, 'm')
#define stopMethodTracing2(_Prefix, _TimeUnit)      stdutil::_Trace::stop_method_tracing(_Prefix, _TimeUnit)
#else
#define startMethodTracing()                        ((void)0)
#define stopMethodTracing(_Prefix)                  ((void)0)
#define stopMethodTracing2(_Prefix, _TimeUnit)      ((void)0)
#endif  // NDEBUG

#ifndef NDEBUG
__STATIC_INLINE__ void _LogError(const char* msg)
{
    const int errnum = errno;
    char error[256];
    ::strerror_r(errnum, error, sizeof(error));
    LOGE("%s - errno = %d, error = %s\n", msg, errnum, error);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the _Trace class
//

__INLINE__ _Trace::_Trace()
{
    LOGD("_Trace::_Trace()\n");
}

__INLINE__ _Trace& _Trace::get()
{
    static thread_local _Trace _Instance;
    return _Instance;
}

__INLINE__ void _Trace::start_method_tracing()
{
    get()._Mystart = std::chrono::steady_clock::now();
}

__INLINE__ void _Trace::stop_method_tracing(const char* _Prefix, char _TimeUnit)
{
    assert(_Prefix);

    using namespace std::chrono;
    const nanoseconds _Duration = steady_clock::now() - get()._Mystart;
    long long _RunningTime;
    switch (_TimeUnit)
    {
    // nanoseconds
    case 'n':
        _RunningTime = _Duration.count();
        break;

    // microseconds
    case 'u':
        _RunningTime = duration_cast<microseconds>(_Duration).count();
        break;

    // milliseconds
    default:
        _RunningTime = duration_cast<milliseconds>(_Duration).count();
        break;
    }

    LOGD("%s running time = %lld%cs\n", _Prefix, _RunningTime, _TimeUnit);
}
#endif  // NDEBUG


///////////////////////////////////////////////////////////////////////////////
// Implementation of the spin_mutex class
//

__INLINE__ void spin_mutex::lock()
{
#ifndef NDEBUG
    const std::thread::id _Id = std::this_thread::get_id();
    if (_Myowner == _Id) {
        LOGE("The spin_mutex deadlock would occur.\n");
        assert(false);
    }
#endif  // NDEBUG

    while (_Myflag.test_and_set(std::memory_order_acquire)) {
        // Empty loop
    }

#ifndef NDEBUG
    _Myowner = _Id;
#endif  // NDEBUG
}

__INLINE__ void spin_mutex::unlock()
{
    _Myflag.clear(std::memory_order_release);

#ifndef NDEBUG
    _Myowner = {};
#endif  // NDEBUG
}

__INLINE__ bool spin_mutex::try_lock()
{
    const bool _Result = !_Myflag.test_and_set(std::memory_order_acquire);

#ifndef NDEBUG
    if (_Result) {
        _Myowner = std::this_thread::get_id();
    }
#endif  // NDEBUG

    return _Result;
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the FileDescriptor class
//

__INLINE__ constexpr FileDescriptor::FileDescriptor(int fd/* = -1*/)
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
    assert(buf);
    assert(!isEmpty());

    const ssize_t readBytes = ::read(mFd, buf, count);

#ifndef NDEBUG
    if (readBytes == -1) {
        _LogError("The FileDescriptor read failed");
    }
#endif  // NDEBUG

    return readBytes;
}

__INLINE__ ssize_t FileDescriptor::write(const void* buf, size_t count) const
{
    assert(buf);
    assert(!isEmpty());

    const ssize_t writtenBytes = ::write(mFd, buf, count);

#ifndef NDEBUG
    if (writtenBytes == -1) {
        _LogError("The FileDescriptor write failed");
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

__INLINE__ constexpr Epoll::Epoll(int epollFd/* = -1*/)
    : FileDescriptor(epollFd)
{
}

__INLINE__ int Epoll::create(int size/* = 1*/)
{
    assert(isEmpty());
    mFd = ::epoll_create(size);

#ifndef NDEBUG
    if (mFd == -1) {
        _LogError("The Epoll create failed");
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
        _LogError("The Epoll add fd failed");
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
        _LogError("The Epoll remove fd failed");
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
        _LogError("The Epoll wait failed");
        assert(false);
    }
#endif  // NDEBUG

    return result;
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the EventFd class
//

__INLINE__ constexpr EventFd::EventFd(int eventFd/* = -1*/)
    : FileDescriptor(eventFd)
{
}

__INLINE__ int EventFd::create(uint32_t initval/* = 0*/, int flags/* = EFD_NONBLOCK*/)
{
    assert(isEmpty());
    mFd = ::eventfd(initval, flags);

#ifndef NDEBUG
    if (mFd == -1) {
        _LogError("The EventFd create failed");
        assert(false);
    }
#endif  // NDEBUG

    return mFd;
}

__INLINE__ void EventFd::poll(int timeout/* = -1*/) const
{
    assert(!isEmpty());

    struct pollfd pfd = { mFd, POLLIN };
    const int result = ::poll(&pfd, 1, timeout);

#ifndef NDEBUG
    if (result == -1) {
        _LogError("The EventFd poll failed");
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

__INLINE__ constexpr LocalSocket::LocalSocket(int sockFd/* = -1*/)
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
        _LogError("The LocalSocket listen failed");
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
        _LogError("The LocalSocket accept failed");
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
        _LogError("The LocalSocket connect failed");
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

#endif  // __SYNCUTIL_INL__