///////////////////////////////////////////////////////////////////////////////
// syncutil.h
//
// Author : Garfield
// Creation Date : 2020/4/20

#ifndef __SYNCUTIL_H__
#define __SYNCUTIL_H__

#include "platform.h"
#include <mutex>
#include <atomic>
#include <chrono>
#include <thread>
#include <poll.h>
#include <sys/un.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/eventfd.h>

///////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// spin_mutex
// spin_lock
// Epoll
// EventFd
// LocalSocket
//
// Global functions in this file:
//
// startMethodTracing()
// stopMethodTracing()
// stopMethodTracing2()

namespace stdutil {

#ifndef NDEBUG
///////////////////////////////////////////////////////////////////////////////
// Interface of the _Trace class
//

class _Trace final
{
// Constructors
private:
    _Trace() = default;

// Operations
public:
    static void start_method_tracing();
    static void stop_method_tracing(const char* _Prefix, char _TimeUnit);

// Implementation
private:
    using time_point = std::chrono::steady_clock::time_point;

// Data members
private:
    time_point _Mystart;
    std::thread::id _Myowner;
    static thread_local _Trace _Mytrace;
};
#endif  // NDEBUG


///////////////////////////////////////////////////////////////////////////////
// Interface of the spin_mutex class
//

class spin_mutex final
{
// Constructors
public:
    constexpr spin_mutex() = default;
    spin_mutex(const spin_mutex&) = delete;
    spin_mutex& operator=(const spin_mutex&) = delete;

// Operations
public:
    /**
     * Blocks the calling thread until the thread obtains ownership of this mutex.
     */
    void lock();

    /**
     * Releases ownership of this mutex.
     */
    void unlock();

// Data members
private:
#ifndef NDEBUG
    std::thread::id _Myowner = {};
#endif  // NDEBUG

    std::atomic_flag _Myflag = ATOMIC_FLAG_INIT;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the spin_lock class
//

using spin_lock = std::lock_guard<spin_mutex>;


///////////////////////////////////////////////////////////////////////////////
// Interface of the FileDescriptor class
//

class FileDescriptor
{
// Constructors/Destructor
protected:
    explicit constexpr FileDescriptor(int fd = -1);
    ~FileDescriptor();

    FileDescriptor(const FileDescriptor&) = delete;
    FileDescriptor& operator=(const FileDescriptor&) = delete;

// Operations
public:
    /**
     * Returns the file descriptor of this object.
     */
    operator int() const;

    /**
     * Closes the file descriptor of this object.
     */
    void close();

    /**
     * Attachs a file descriptor to this object.
     * @param fd A file descriptor to attach.
     */
    void attach(int fd);

    /**
     * Reads the count bytes from the file descriptor into the buffer.
     * @param buf A pointer to the buffer to receive the data.
     * @param count The number of bytes of the buffer.
     * @return The number of bytes to read, -1 otherwise.
     */
    ssize_t read(void* buf, size_t count) const;

    /**
     * Writes the count bytes from the buffer to the file descriptor.
     * @param buf A pointer to a buffer containing the data.
     * @param count The number of bytes of the buffer.
     * @return The number of bytes to write, -1 otherwise.
     */
    ssize_t write(const void* buf, size_t count) const;

// Attributes
public:
    /**
     * Tests if this object is empty.
     * @return true if this object is empty, false otherwise.
     */
    bool isEmpty() const;

// Data members
protected:
    int mFd;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the Epoll class
//

class Epoll final : public FileDescriptor
{
// Constructors
public:
    explicit constexpr Epoll(int epollFd = -1);

// Operations
public:
    /**
     * Creates a new epoll file descriptor.
     * @param size The number of file descriptors that the caller
     * expected to add to this epoll.
     * @return returns a new epoll file descriptor, -1 otherwise.
     */
    int create(int size = 1);

    /**
     * Adds a file descriptor to this epoll.
     * @param fd The file descriptor to add.
     * @param events The events to add.
     * @return returns 0 if adds successful, -1 otherwise.
     */
    int addFd(int fd, uint32_t events = EPOLLIN) const;

    /**
     * Removes a file descriptor from this epoll.
     * @param fd The file descriptor to remove.
     * @return returns 0 if removes successful, -1 otherwise.
     */
    int removeFd(int fd) const;

    /**
     * Waits for events on this epoll.
     * @param events A pointer to the epoll_event is used to return
     * information from the ready list about file descriptors.
     * @param count The number of epoll_events.
     * @param timeout The waiting timeout in milliseconds, -1 causes
     * wait to indefinitely.
     * @return The number of file descriptors ready for the requested
     * I/O, or 0 if no file descriptor became ready during the requested
     * timeout milliseconds, -1 otherwise.
     */
    int wait(struct epoll_event* events, int count = 1, int timeout = -1) const;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the EventFd class
//

class EventFd final : public FileDescriptor
{
// Constructors
public:
    explicit constexpr EventFd(int eventFd = -1);

// Operations
public:
    /**
     * Creates a new eventfd object.
     * @param initval The initial counter of this eventfd.
     * @param flags The flags to create.
     * @return returns a new file descriptor of this object, -1 otherwise.
     */
    int create(uint32_t initval = 0, int flags = EFD_NONBLOCK);

    /**
     * Blocks the thread, and sets a timeout after the thread unblocks.
     * @param timeout The waiting timeout in milliseconds, -1 causes wait
     * to indefinitely.
     */
    void poll(int timeout = -1);

    /**
     * Reads an 8-byte unsigned integer from this eventfd.
     * @param value The value to store the read result.
     * @return The number of bytes to read, -1 otherwise.
     */
    ssize_t read(uint64_t& value) const;

    /**
     * Writes an 8-byte unsigned integer to this eventfd.
     * @param value The value to write.
     * @return The number of bytes to write, -1 otherwise.
     */
    ssize_t write(uint64_t value = 1) const;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the LocalSocket class
//

class LocalSocket final : public FileDescriptor
{
// Constructors
public:
    explicit constexpr LocalSocket(int sockFd = -1);

// Operations
public:
    /**
     * The namespace that the UNIX-domain socket exists in.
     */
    enum class Namespace : int32_t
    {
        /**
         * A socket in the Linux abstract namespace.
         */
        abstract,

        /**
         * A socket named with a normal filesystem path.
         */
        filesystem,
    };

    /**
     * Places this socket in a state in which it is listening for an 
     * incoming connection.
     * @param name The name of the address.
     * @param ns The namespace of the UNIX-domain socket.
     * @return returns 0 if connects successful, -1 otherwise.
     */
    int listen(const char* name, Namespace ns = Namespace::abstract);

    /**
     * Permits an incoming connection attempt on this socket.
     * @return returns a file descriptor for the accepted socket, -1 otherwise.
     */
    int accept() const;

    /**
     * Establishes a connection to a UNIX-domain socket.
     * @param name The name of the address.
     * @param ns The namespace of the UNIX-domain socket.
     * @return returns 0 if connects successful, -1 otherwise.
     */
    int connect(const char* name, Namespace ns = Namespace::abstract);

// Implementation
private:
    static socklen_t buildSockAddr(struct sockaddr_un& addr, const char* name, Namespace ns);
};

}  // namespace stdutil

#include "syncutil.inl"

#endif  // __SYNCUTIL_H__