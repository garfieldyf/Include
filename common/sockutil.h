///////////////////////////////////////////////////////////////////////////////
// sockutil.h
//
// Copyright(c) 2018, Garfield. All rights reserved.
// Author  : Garfield
// Version : 1.0
// Creation Date : 2008/10/11

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __SOCKUTIL_H__
#define __SOCKUTIL_H__

#include "sockdef.h"

///////////////////////////////////////////////////////////////////////////////
// Classes in this file:
//
// _sockaddr_t
// _socket_t
// _hostent_t
// _addrinfo_t

namespace stdutil {

///////////////////////////////////////////////////////////////////////////////
// Interface of the _sockaddr_t class
//

class _sockaddr_t : public sockaddr_in
{
// Constructors
public:
    _sockaddr_t();
    _sockaddr_t(u_long addr, u_short port);
    _sockaddr_t(in_addr addr, u_short port);
    _sockaddr_t(const char* addr, u_short port);

// Operations
public:
    operator sockaddr*();
    operator const sockaddr*() const;

    void set(u_long addr, u_short port);
    void set(in_addr addr, u_short port);
    void set(const char* addr, u_short port);

#ifdef _CRT_DEBUG_DUMP
    template <typename _Pr> void Dump(const _Pr& _Printer) const;
#endif
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the _socket_t class
//

class _socket_t
{
    DECLARE_NONCOPYABLE(_socket_t);

// Constructors/Destructor
public:
    explicit _socket_t(SOCKET _Sock = INVALID_SOCKET);
    ~_socket_t();

// Operations
public:
    bool create(int af = AF_INET, int type = SOCK_STREAM, int protocol = IPPROTO_TCP);
    void close();

    SOCKET detach();
    void attach(SOCKET _Sock);

    SOCKET accept(sockaddr* addr = NULL, socklen_t* addrlen = NULL) const;
    bool accept(_socket_t& client, sockaddr* addr = NULL, socklen_t* addrlen = NULL) const;

    int listen(int backlog = 10) const;
    int shutdown(int how) const;

#ifdef WINVER
    int ioctl(long cmd, u_long* param) const;
#endif

    int bind(const sockaddr* addr, int addrlen = sizeof(sockaddr_in)) const;
    int bind(const char* addr, u_short port) const;

    int connect(const sockaddr* addr, int addrlen = sizeof(sockaddr_in)) const;
    int connect(const char* addr, u_short port) const;

    int send(const void* buf, int size, int flags = 0) const;
    int recv(void* buf, int size, int flags = 0) const;

    int sendto(const void* buf, int size, const sockaddr* to, int tolen = sizeof(sockaddr_in), int flags = 0) const;
    int sendto(const void* buf, int size, const char* to, u_short port, int flags = 0) const;
    int recvfrom(void* buf, int size, sockaddr* from = NULL, socklen_t* fromlen = NULL, int flags = 0) const;

// Attributes
public:
    bool empty() const;

    int getsockname(sockaddr* name, socklen_t& namelen) const;
    int getpeername(sockaddr* name, socklen_t& namelen) const;

    template <typename _Ty>
    int getsockopt(int optname, _Ty& value, int level = SOL_SOCKET) const;

    template <typename _Ty>
    int setsockopt(int optname, const _Ty& value, int level = SOL_SOCKET) const;

// Data members
public:
    SOCKET sock;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the _hostent_t class
//

class _hostent_t
{
// Constructors
public:
    _hostent_t();

// Operations
public:
    hostent* get(const char* name = NULL);

    in_addr* addr(size_t index) const;
    const char* alias(size_t index) const;

#ifdef _CRT_DEBUG_DUMP
    template <typename _Pr> void Dump(const _Pr& _Printer) const;
#endif

// Attributes
public:
    bool empty() const;

// Data members
public:
    hostent* host;
};


///////////////////////////////////////////////////////////////////////////////
// Interface of the _addrinfo_t class
//

class _addrinfo_t : public addrinfo
{
    DECLARE_NONCOPYABLE(_addrinfo_t);

// Constructors/Destructor
public:
    explicit _addrinfo_t(int family = AF_INET, int socktype = SOCK_STREAM, int protocol = IPPROTO_TCP);
    ~_addrinfo_t();

// Operations
public:
    bool get(const char* hostname, const char* servicename = "80");

#ifdef _CRT_DEBUG_DUMP
    template <typename _Pr> void Dump(const _Pr& _Printer) const;
#endif

// Attributes
public:
    bool empty() const;

// Data members
public:
    addrinfo* ai_list;
};

}  // namespace stdutil

#include "sockutil.inl"

#endif  // __SOCKUTIL_H__