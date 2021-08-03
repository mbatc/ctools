
// -----------------------------------------------------------------------------
// The MIT License
// 
// Copyright(c) 2020 Michael Batchelor, 
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
// -----------------------------------------------------------------------------

#include "networking/ctSocket.h"

#ifdef ctPLATFORM_LINUX
#include "ctAssert.h"

#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/select.h>
#include <unistd.h>
#include <netdb.h>
#include <errno.h>
#include <arpa/inet.h>

enum atSocketState
{
  atSS_Read = 1,
  atSS_Write = 1 << 1,
  atSS_Error = 1 << 2,
  atSS_Timeout = 1 << 3,
};

int64_t ctSocket::m_nSockets = 0;

static int64_t _GetBytesAvailable(atSocketHandle handle)
{
  unsigned long byteCount = 0;
  ioctl((int)handle, FIONREAD, &byteCount);
  return (int64_t)byteCount;
}

static int64_t _Status(atSocketHandle handle, const int64_t timeout = -1)
{
  if (handle == 0)
    return 0;

  timeval tVal;
  tVal.tv_sec = timeout < 0 ? 0 : (long)timeout / 1000;
  tVal.tv_usec = timeout < 0 ? 0 : (long)timeout % 1000;

  fd_set fdRead;
  fd_set fdWrite;
  fd_set fdExcept;

  FD_ZERO(&fdRead);
  FD_ZERO(&fdWrite);
  FD_ZERO(&fdExcept);

  FD_SET((int)handle, &fdRead);
  FD_SET((int)handle, &fdWrite);
  FD_SET((int)handle, &fdExcept);

  int64_t res = select((int)(handle + 1), &fdRead, &fdWrite, &fdExcept, &tVal);
  if (res == -1)
    return 0;

  int64_t state = 0;
  if (res == 0)
    state |= atSS_Timeout;
  else
  {
    if (FD_ISSET((int)handle, &fdRead)) state |= atSS_Read;
    if (FD_ISSET((int)handle, &fdWrite)) state |= atSS_Write;
    if (FD_ISSET((int)handle, &fdExcept)) state |= atSS_Error;
  }
  return state;
}

static atSocketHandle _CreateSocket(const char *addr, const char *port, const bool host)
{
  addrinfo *pInfo = nullptr;
  addrinfo hints = { 0 };

  hints.ai_flags = AI_PASSIVE;
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;

  if (getaddrinfo(addr, port, &hints, &pInfo) != 0)
    return 0;

  int handle = 0;
  for(addrinfo *pTest = pInfo; pTest != nullptr; pTest = pTest->ai_next)
  {
    int connectResult = -1;
    handle = socket(pInfo->ai_family, pInfo->ai_socktype, pInfo->ai_protocol);
    if (host)
      connectResult = bind(handle, pTest->ai_addr, (int)pTest->ai_addrlen);
    else
      connectResult = connect(handle, pTest->ai_addr, (int)pTest->ai_addrlen);
    if (connectResult == -1)
    {
      close(handle);
      handle = 0;
      continue;
    }
    break;
  }

  if (host)
    listen(handle, SOMAXCONN);

  unsigned long mode = 1; // non-blocking
  ioctl(handle, FIONBIO, &mode);

  freeaddrinfo(pInfo);
  return (atSocketHandle)handle;
}

static void _CloseSocket(int handle)
{
  shutdown(handle, SHUT_WR);
  close(handle);
}

ctSocket::~ctSocket()
{
  if (m_handle != 0)
    _CloseSocket(m_handle);
  m_handle = 0;
  --m_nSockets;
}

ctSocket ctSocket::Host(const ctString &port) 
{
  ctSocket sock;
  sock.m_handle = _CreateSocket(nullptr, port, true);
  sock.m_addr = "localhost";
  sock.m_port = port;
  sock.m_isHost = true;
  return sock;
}

ctSocket ctSocket::Connect(const ctString &addr, const ctString &port) 
{ 
  ctSocket sock;
  sock.m_handle = _CreateSocket(addr, port, false);
  sock.m_addr = addr;
  sock.m_port = port;
  sock.m_isHost = false;
  return sock;
}

ctSocket ctSocket::Accept() const
{
  ctSocket sock;
  sockaddr addr = { 0 };
  socklen_t len = 0;
  int addrlen = sizeof(sockaddr);
  sock.m_handle = accept(Handle(), &addr, &len);
  sock.m_isHost = false;
  sock.m_port = m_port;

  static const int64_t buflen = ctMax(INET_ADDRSTRLEN, INET6_ADDRSTRLEN);
  char buffer[buflen] = { 0 };

  // Get Connection Address
  sockaddr_in* ipv4Addr = addr.sa_family == AF_INET ? (sockaddr_in*)&addr : nullptr;
  sockaddr_in6* ipv6Addr = addr.sa_family == AF_INET6 ? (sockaddr_in6*)&addr : nullptr;
  if (ipv4Addr) inet_ntop(AF_INET, &ipv4Addr->sin_addr, buffer, buflen);
  if (ipv6Addr) inet_ntop(AF_INET6, &ipv6Addr->sin6_addr, buffer, buflen);
  sock.m_addr = buffer;

  return sock;
}

ctSocket::ctSocket(ctSocket &&move)
  : m_addr(move.m_addr)
  , m_port(move.m_port)
  , m_isHost(move.m_isHost)
  , m_handle(move.m_handle)
{
  ++m_nSockets;
  move.m_handle = 0; 
}

ctSocket::ctSocket() : m_handle(0) {}

const ctSocket& ctSocket::operator=(ctSocket &&move)
{
  m_addr = move.m_addr;
  m_port = move.m_port;
  m_isHost = move.m_isHost;
  m_handle = move.m_handle;
  move.m_handle = 0;
  move.m_port = "";
  move.m_addr = "";
  move.m_isHost = false;
  return *this;
}

bool ctSocket::CanRead(int64_t *pNumBytes) const
{
  if (!IsValid())
    return false;
  int64_t nBytes = 0;
  if ((_Status(m_handle, -1) & atSS_Read) > 0)
  {
    if (pNumBytes)
      *pNumBytes = _GetBytesAvailable(m_handle);
  }
  return nBytes > 0;
}

int64_t ctSocket::Read(uint8_t *pData, const int64_t maxLen) const
{
  if (!IsValid())
    return 0;
  int res = recv(Handle(), (char*)pData, (int)maxLen, 0);
  if (res < 0)
  {
    printf("WSA error: %s (%d)\n", strerror(errno), errno);
  }
  return res;
}

const ctString& ctSocket::Port() const { return m_port; }
const ctString& ctSocket::Address() const { return m_addr; }
const atSocketHandle& ctSocket::Handle() const { return m_handle; }

bool ctSocket::IsHost() const { return m_isHost; }
bool ctSocket::CanAccept() const { return IsHost() && CanRead(); }
bool ctSocket::IsValid() const { return m_handle != 0; }
bool ctSocket::CanWrite() const { return (_Status(m_handle, -1) & atSS_Write) > 0; }
int64_t ctSocket::Write(const uint8_t *pData, const int64_t len) const { return IsValid() ? send(Handle(), (char*)pData, (int)len, 0) : 0; }

#endif
