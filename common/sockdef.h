///////////////////////////////////////////////////////////////////////////////
// sockdef.h
//
// Author : Garfield
// Creation Date : 2008/10/11

#if (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef __SOCKDEF_H__
#define __SOCKDEF_H__

#include "platform.h"

#ifdef WINVER

///////////////////////////////////////////////////////////////////////////////
// Windows socket 2 header file.
//

#include <winsock2.h>

// The _closesocket macro
#define _closesocket        closesocket

#else

///////////////////////////////////////////////////////////////////////////////
// Linux socket header files.
//

#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>

// The socket handle
typedef int     SOCKET;

// Invalid socket value
#define INVALID_SOCKET      (-1)

// Socket error
#define SOCKET_ERROR        (-1)

// The _closesocket macro
#define _closesocket        close

#endif  // WINVER

#endif  // __SOCKDEF_H__