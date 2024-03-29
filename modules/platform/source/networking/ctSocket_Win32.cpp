
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

#ifdef ctPLATFORM_WIN32
#include "ctAssert.h"
#include <WS2tcpip.h>
#include <WinSock2.h>
#pragma comment(lib, "Ws2_32.lib")

enum atSocketState
{
  atSS_Read = 1,
  atSS_Write = 1 << 1,
  atSS_Error = 1 << 2,
  atSS_Timeout = 1 << 3,
};

int64_t ctSocket::m_nSockets = 0;

static void _InitialiseSockets()
{
  WSAData wsaData;
  int wsaStartupResult = WSAStartup(MAKEWORD(atWSAMajorVer, atWSAMinorVer), &wsaData);
  ctAssert(wsaStartupResult == 0, "WSAStartup Failed()");
}

static void _DeInitSockets()
{
  int wsaCleanupResult = WSACleanup();
  ctAssert(wsaCleanupResult == 0, "WSACleanup Failed()");
}

static int64_t _GetBytesAvailable(atSocketHandle handle)
{
  unsigned long byteCount = 0;
  ioctlsocket((SOCKET)handle, FIONREAD, &byteCount);
  return (int64_t)byteCount;
}

static int64_t _Status(atSocketHandle handle, const int64_t timeout = -1)
{
  if (handle == INVALID_SOCKET)
    return 0;
  TIMEVAL tVal;
  tVal.tv_sec = timeout < 0 ? 0 : (long)timeout / 1000;
  tVal.tv_usec = timeout < 0 ? 0 : (long)timeout % 1000;

  FD_SET fdRead;
  FD_SET fdWrite;
  FD_SET fdExcept;

  FD_ZERO(&fdRead);
  FD_ZERO(&fdWrite);
  FD_ZERO(&fdExcept);

  FD_SET((SOCKET)handle, &fdRead);
  FD_SET((SOCKET)handle, &fdWrite);
  FD_SET((SOCKET)handle, &fdExcept);

  int64_t res = select((int)(handle + 1), &fdRead, &fdWrite, &fdExcept, &tVal);
  if (res == -1)
    return 0;

  int64_t state = 0;
  if (res == 0)
    state |= atSS_Timeout;
  else
  {
    if (FD_ISSET((SOCKET)handle, &fdRead)) state |= atSS_Read;
    if (FD_ISSET((SOCKET)handle, &fdWrite)) state |= atSS_Write;
    if (FD_ISSET((SOCKET)handle, &fdExcept)) state |= atSS_Error;
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
    return INVALID_SOCKET;

  SOCKET handle = INVALID_SOCKET;
  for(addrinfo *pTest = pInfo; pTest != nullptr; pTest = pTest->ai_next)
  {
    int connectResult = SOCKET_ERROR;
    handle = socket(pInfo->ai_family, pInfo->ai_socktype, pInfo->ai_protocol);
    if (host)
      connectResult = bind(handle, pTest->ai_addr, (int)pTest->ai_addrlen);
    else
      connectResult = connect(handle, pTest->ai_addr, (int)pTest->ai_addrlen);
    if (connectResult == SOCKET_ERROR)
    {
      closesocket(handle);
      handle = INVALID_SOCKET;
      continue;
    }
    break;
  }

  if (host)
    listen(handle, SOMAXCONN);

  unsigned long mode = 1; // non-blocking
  ioctlsocket(handle, FIONBIO, &mode);

  freeaddrinfo(pInfo);
  return (atSocketHandle)handle;
}

static void _CloseSocket(SOCKET handle)
{
  shutdown(handle, SD_SEND);
  closesocket(handle);
}

ctSocket::~ctSocket()
{
  if (m_handle != INVALID_SOCKET)
    _CloseSocket(m_handle);
  m_handle = INVALID_SOCKET;
  --m_nSockets;
  if (m_nSockets == 0)
    _DeInitSockets();
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
  int addrlen = sizeof(sockaddr);
  sock.m_handle = accept(Handle(), &addr, &addrlen);
  sock.m_isHost = false;
  sock.m_port = m_port;

  static const int64_t buflen = max(INET_ADDRSTRLEN, INET6_ADDRSTRLEN);
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
  move.m_handle = INVALID_SOCKET; 
}

ctSocket::ctSocket() : m_handle(INVALID_SOCKET) { if(++m_nSockets == 1) _InitialiseSockets(); }

const ctSocket& ctSocket::operator=(ctSocket &&move)
{
  m_addr = move.m_addr;
  m_port = move.m_port;
  m_isHost = move.m_isHost;
  m_handle = move.m_handle;
  move.m_handle = INVALID_SOCKET;
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
    printf("WSA error %d\n", WSAGetLastError());
  }
  return res;
}

const ctString& ctSocket::Port() const { return m_port; }
const ctString& ctSocket::Address() const { return m_addr; }
const atSocketHandle& ctSocket::Handle() const { return m_handle; }

bool ctSocket::IsHost() const { return m_isHost; }
bool ctSocket::CanAccept() const { return IsHost() && CanRead(); }
bool ctSocket::IsValid() const { return m_handle != INVALID_SOCKET; }
bool ctSocket::CanWrite() const { return (_Status(m_handle, -1) & atSS_Write) > 0; }
int64_t ctSocket::Write(const uint8_t *pData, const int64_t len) const { return IsValid() ? send(Handle(), (char*)pData, (int)len, 0) : 0; }

#endif
