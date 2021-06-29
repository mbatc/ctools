
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

#ifndef ctReadStream_h__
#define ctReadStream_h__

#include "ctStreamSeekable.h"

#define ctTrivialStreamRead(type) inline int64_t ctStreamRead(ctReadStream *pStream, type *pData, const int64_t count) { return ctStreamRead(pStream, (void*)pData, sizeof(type) * count); }

class ctReadStream : public ctStreamSeekable
{
public:
  // Read data into pBuffer. 
  // Returns the number of bytes read
  virtual int64_t Peek(void *pBuffer, const int64_t size);
  virtual int64_t Read(void *pBuffer, const int64_t size) = 0;
  template<typename T> int64_t Read(T *pBuffer, const int64_t count = 1);
};

int64_t ctStreamRead(ctReadStream *pStream, void *pData, const int64_t count);

ctTrivialStreamRead(int64_t)
ctTrivialStreamRead(int32_t)
ctTrivialStreamRead(int16_t)
ctTrivialStreamRead(int8_t)
ctTrivialStreamRead(uint64_t)
ctTrivialStreamRead(uint32_t)
ctTrivialStreamRead(uint16_t)
ctTrivialStreamRead(uint8_t)
ctTrivialStreamRead(bool)
ctTrivialStreamRead(wchar_t)
ctTrivialStreamRead(char)
ctTrivialStreamRead(double)
ctTrivialStreamRead(float)

template<typename T> int64_t ctReadStream::Read(T *pBuffer, const int64_t count) { return ctStreamRead(this, pBuffer, count); }

#endif // ctReadStream_h__
