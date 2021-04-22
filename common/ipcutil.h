///////////////////////////////////////////////////////////////////////////////
// ipcutil.h
//
// Author : Garfield
// Creation Date : 2020/4/20

#ifndef __IPCUTIL_H__
#define __IPCUTIL_H__

#include "platform.h"
#include <unistd.h>
#include <sys/un.h>
#include <sys/socket.h>

///////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// LocalSocket

namespace stdutil {

///////////////////////////////////////////////////////////////////////////////
// Interface of the LocalSocket class
//

class LocalSocket final
{
// Constructors/Destructor
public:
    LocalSocket();
    ~LocalSocket();

    LocalSocket(const LocalSocket&) = delete;
    LocalSocket& operator=(const LocalSocket&) = delete;

// Operations
public:
    /**
     * The namespace that the UNIX-domain socket exists in.
     */
    enum class Namespace
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
     * Establishes a connection to a UNIX-domain socket.
     * @param name The name of the address.
     * @param ns The namespace of the UNIX-domain socket.
     * @return returns 0 if connects successful, -1 otherwise.
     */
    int connect(const char* name, Namespace ns = Namespace::abstract);

    /**
     * Sends data to a connected socket.
     * @param buf A pointer to a buffer containing the data.
     * @param size The number of bytes of the buffer.
     * @return The total number of bytes sent, -1 otherwise.
     */
    ssize_t send(const void* buf, size_t size) const;

    /**
     * Receives data from a connected socket.
     * @param buf A pointer to the buffer to receive the incoming data.
     * @param size The number of bytes of the buffer.
     * @return The number of bytes received, -1 otherwise.
     */
    ssize_t recv(void* buf, size_t size) const;

    /**
     * Permits an incoming connection attempt on this socket.
     * @return returns a file descriptor for the accepted socket, -1 otherwise.
     */
    int accept();

    /**
     * Closes this socket file descriptor.
     */
    void close();

    /**
     * Attachs a file descriptor of the socket.
     * @param sockFd A file descriptor of the socket.
     */
    void attach(int sockFd);

// Attributes
public:
    /**
     * Tests if this socket is empty.
     * @return true if this socket is empty, false otherwise.
     */
    bool isEmpty() const;

// Implementation
private:
    static socklen_t buildSockAddr(struct sockaddr_un& addr, const char* name, Namespace ns);

// Data members
private:
    int mSockFd;
};

}  // namespace stdutil

#include "ipcutil.inl"

#endif  // __IPCUTIL_H__