///////////////////////////////////////////////////////////////////////////////
// sockutil.inl
//
// Author : Garfield
// Creation Date : 2008/10/11

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __SOCKUTIL_INL__
#define __SOCKUTIL_INL__

#ifndef __SOCKUTIL_H__
    #error sockutil.inl requires sockutil.h to be included first
#endif

namespace stdutil {

///////////////////////////////////////////////////////////////////////////////
// Implementation of the sockaddr_t class
//

__INLINE__ sockaddr_t::sockaddr_t()
{
}

__INLINE__ sockaddr_t::sockaddr_t(u_long addr, u_short port)
{
    set(addr, port);
    sin_family = AF_INET;
}

__INLINE__ sockaddr_t::sockaddr_t(in_addr addr, u_short port)
{
    set(addr, port);
    sin_family = AF_INET;
}

__INLINE__ sockaddr_t::sockaddr_t(const char* addr, u_short port)
{
    assert(addr);

    set(addr, port);
    sin_family = AF_INET;
}

__INLINE__ sockaddr_t::operator sockaddr*()
{
    return reinterpret_cast<sockaddr*>(this);
}

__INLINE__ sockaddr_t::operator const sockaddr*() const
{
    return reinterpret_cast<const sockaddr*>(this);
}

__INLINE__ void sockaddr_t::set(u_long addr, u_short port)
{
    sin_addr.s_addr = addr;
    sin_port = port;
}

__INLINE__ void sockaddr_t::set(in_addr addr, u_short port)
{
    sin_addr = addr;
    sin_port = htons(port);
}

__INLINE__ void sockaddr_t::set(const char* addr, u_short port)
{
    assert(addr);

    sin_addr.s_addr = inet_addr(addr);
    sin_port = htons(port);
}

#ifdef _CRT_DEBUG_DUMP
template <typename _Pr>
__INLINE__ void sockaddr_t::Dump(const _Pr& _Printer) const
{
    _Printer("sockaddr_t [ address = %s : %u ]\n", inet_ntoa(sin_addr), ntohs(sin_port));
}
#endif  // _CRT_DEBUG_DUMP


///////////////////////////////////////////////////////////////////////////////
// Implementation of the socket_t class
//

__INLINE__ socket_t::socket_t(SOCKET _Sock/* = INVALID_SOCKET*/)
    : sock(_Sock)
{
}

__INLINE__ socket_t::~socket_t()
{
    close();
}

__INLINE__ bool socket_t::create(int af/* = AF_INET*/, int type/* = SOCK_STREAM*/, int protocol/* = IPPROTO_TCP*/)
{
    assert(empty());
    return ((sock = ::socket(af, type, protocol)) != INVALID_SOCKET);
}

__INLINE__ void socket_t::close()
{
    if (sock != INVALID_SOCKET && ::_closesocket(sock) == 0)
        sock = INVALID_SOCKET;
}

__INLINE__ SOCKET socket_t::detach()
{
    SOCKET oldSock = sock;
    sock = INVALID_SOCKET;

    return oldSock;
}

__INLINE__ void socket_t::attach(SOCKET _Sock)
{
    assert((sock == INVALID_SOCKET && _Sock == INVALID_SOCKET) || sock != _Sock);

    close();
    sock = _Sock;
}

__INLINE__ SOCKET socket_t::accept(sockaddr* addr/* = NULL*/, socklen_t* addrlen/* = NULL*/) const
{
    assert(!empty());
    return ::accept(sock, addr, addrlen);
}

__INLINE__ bool socket_t::accept(socket_t& client, sockaddr* addr/* = NULL*/, socklen_t* addrlen/* = NULL*/) const
{
    assert(!empty());
    assert(client.empty());

    client.attach(::accept(sock, addr, addrlen));
    return (client.sock != INVALID_SOCKET);
}

__INLINE__ int socket_t::listen(int backlog/* = 10*/) const
{
    assert(!empty());
    return ::listen(sock, backlog);
}

__INLINE__ int socket_t::shutdown(int how) const
{
    assert(!empty());
    return ::shutdown(sock, how);
}

#ifdef WINVER
__INLINE__ int socket_t::ioctl(long cmd, u_long* param) const
{
    assert(param);
    assert(!empty());

    return ::ioctlsocket(sock, cmd, param);
}
#endif  // WINVER

__INLINE__ int socket_t::bind(const sockaddr* addr, int addrlen/* = sizeof(sockaddr_in)*/) const
{
    assert(addr);
    assert(!empty());

    return ::bind(sock, addr, addrlen);
}

__INLINE__ int socket_t::bind(const char* addr, u_short port) const
{
    assert(addr);
    assert(!empty());

    return ::bind(sock, sockaddr_t(addr, port), sizeof(sockaddr_t));
}

__INLINE__ int socket_t::connect(const sockaddr* addr, int addrlen/* = sizeof(sockaddr_in)*/) const
{
    assert(addr);
    assert(!empty());

    return ::connect(sock, addr, addrlen);
}

__INLINE__ int socket_t::connect(const char* addr, u_short port) const
{
    assert(addr);
    assert(!empty());

    return ::connect(sock, sockaddr_t(addr, port), sizeof(sockaddr_t));
}

__INLINE__ int socket_t::send(const void* buf, int size, int flags/* = 0*/) const
{
    assert(buf);
    assert(!empty());

    return ::send(sock, (const char*)buf, size, flags);
}

__INLINE__ int socket_t::recv(void* buf, int size, int flags/* = 0*/) const
{
    assert(buf);
    assert(!empty());

    return ::recv(sock, (char*)buf, size, flags);
}

__INLINE__ int socket_t::sendto(const void* buf, int size, const sockaddr* to, int tolen/* = sizeof(sockaddr_in)*/, int flags/* = 0*/) const
{
    assert(to);
    assert(buf);
    assert(!empty());

    return ::sendto(sock, (const char*)buf, size, flags, to, tolen);
}

__INLINE__ int socket_t::sendto(const void* buf, int size, const char* to, u_short port, int flags/* = 0*/) const
{
    assert(to);
    assert(buf);
    assert(!empty());

    return ::sendto(sock, (const char*)buf, size, flags, sockaddr_t(to, port), sizeof(sockaddr_t));
}

__INLINE__ int socket_t::recvfrom(void* buf, int size, sockaddr* from/* = NULL*/, socklen_t* fromlen/* = NULL*/, int flags/* = 0*/) const
{
    assert(buf);
    assert(!empty());

    return ::recvfrom(sock, (char*)buf, size, flags, from, fromlen);
}

__INLINE__ bool socket_t::empty() const
{
    return (sock == INVALID_SOCKET);
}

__INLINE__ int socket_t::getsockname(sockaddr* name, socklen_t& namelen) const
{
    assert(name);
    assert(!empty());

    return ::getsockname(sock, name, &namelen);
}

__INLINE__ int socket_t::getpeername(sockaddr* name, socklen_t& namelen) const
{
    assert(name);
    assert(!empty());

    return ::getpeername(sock, name, &namelen);
}

template <typename _Ty>
__INLINE__ int socket_t::getsockopt(int optname, _Ty& value, int level/* = SOL_SOCKET*/) const
{
    assert(!empty());

    int size = sizeof(_Ty);
    return ::getsockopt(sock, level, optname, (char*)&value, &size);
}

template <typename _Ty>
__INLINE__ int socket_t::setsockopt(int optname, const _Ty& value, int level/* = SOL_SOCKET*/) const
{
    assert(!empty());
    return ::setsockopt(sock, level, optname, (const char*)&value, sizeof(_Ty));
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the hostent_t class
//

__INLINE__ hostent_t::hostent_t()
    : host(NULL)
{
}

__INLINE__ hostent* hostent_t::get(const char* name/* = NULL*/)
{
    return (host = ::gethostbyname(name));
}

__INLINE__ in_addr* hostent_t::addr(size_t index) const
{
    return (in_addr*)host->h_addr_list[index];
}

__INLINE__ const char* hostent_t::alias(size_t index) const
{
    assert(!empty());
    return host->h_aliases[index];
}

#ifdef _CRT_DEBUG_DUMP
template <typename _Pr>
__INLINE__ void hostent_t::Dump(const _Pr& _Printer) const
{
    if (!empty())
    {
        _Printer("[ hostent_t ]\n[\n    name    = %s, addr type = %d, addr length = %d\n", host->h_name, host->h_addrtype, host->h_length);
        if (const char* aliasname = alias(0))
        {
            _Printer("    aliases = %s\n", aliasname);
            for (size_t i = 1; (aliasname = alias(i)) != NULL; ++i)
                _Printer("              %s\n", aliasname);
        }

        in_addr* address = addr(0);
        _Printer("    address = %s\n", ::inet_ntoa(*address));
        for (size_t i = 1; (address = addr(i)) != NULL; ++i)
            _Printer("              %s\n", ::inet_ntoa(*address));

        _Printer("]\n");
    }
    else
    {
        _Printer("[ The hostent_t is empty! ]\n");
    }
}
#endif  // _CRT_DEBUG_DUMP

__INLINE__ bool hostent_t::empty() const
{
    return (host == NULL);
}


///////////////////////////////////////////////////////////////////////////////
// Implementation of the addrinfo_t class
//

__INLINE__ addrinfo_t::addrinfo_t(int family/* = AF_INET*/, int socktype/* = SOCK_STREAM*/, int protocol/* = IPPROTO_TCP*/)
{
    ::memset(this, 0, sizeof(addrinfo_t));
    ai_family   = family;
    ai_socktype = socktype;
    ai_protocol = protocol;
}

__INLINE__ addrinfo_t::~addrinfo_t()
{
    ::freeaddrinfo(ai_list);
}

__INLINE__ bool addrinfo_t::get(const char* hostname, const char* servicename/* = "80"*/)
{
    assert(empty());
    return (::getaddrinfo(hostname, servicename, this, &ai_list) == 0);
}

#ifdef _CRT_DEBUG_DUMP
template <typename _Pr>
__INLINE__ void addrinfo_t::Dump(const _Pr& _Printer) const
{
    if (!empty())
    {
        _Printer("[ addrinfo_t ]\n[\n");
        for (addrinfo* info = ai_list; info != NULL; info = info->ai_next)
            _Printer("    family = %2d, socket type = %d, protocol = %d, address = %s\n", info->ai_family, info->ai_socktype, info->ai_protocol, ::inet_ntoa(((sockaddr_in*)info->ai_addr)->sin_addr));

        _Printer("]\n");
    }
    else
    {
        _Printer("[ The addrinfo_t is empty! ]\n");
    }
}
#endif  // _CRT_DEBUG_DUMP

__INLINE__ bool addrinfo_t::empty() const
{
    return (ai_list == NULL);
}

}  // namespace stdutil

#endif  // __SOCKUTIL_INL__