
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

#ifndef atSocket_h__
#define atSocket_h__

#include "ctString.h"

#define atWSAMajorVer 2
#define atWSAMinorVer 2

typedef int64_t atSocketHandle;

class ctSocket
{
public:
  ctSocket();
  ctSocket(ctSocket &&move);
  ~ctSocket();

  static ctSocket Host(const ctString &port);
  static ctSocket Connect(const ctString &addr, const ctString &port);

  bool IsHost() const;
  bool IsValid() const;

  bool CanAccept() const;
  bool CanRead(int64_t *pNumBytes = nullptr) const;
  bool CanWrite() const;
  
  ctSocket Accept() const;
  int64_t Read(uint8_t *pData, const int64_t maxLen) const;
  int64_t Write(const uint8_t *pData, const int64_t len) const;

  const atSocketHandle &Handle() const;
  const ctString &Port() const;
  const ctString &Address() const;

  const ctSocket& operator=(ctSocket &&move);

protected:
  atSocketHandle m_handle;
  ctString m_addr;
  ctString m_port;
  bool m_isHost;

  static int64_t m_nSockets;
};

#endif // atSocket_h__
