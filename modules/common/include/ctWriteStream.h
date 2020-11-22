
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

#ifndef atWriteStream_h__
#define atWriteStream_h__

#include "ctStreamSeekable.h"
#include "ctTypes.h"

#define ctTrivialStreamWrite(type) inline int64_t ctStreamWrite(ctWriteStream *pStream, const type *pData, const int64_t count) { return ctStreamWrite(pStream, (const void*)pData, (int64_t)sizeof(type) * count); }

class ctWriteStream : public ctStreamSeekable
{
public:
  // Writes pData to the file
  virtual int64_t Write(const void *pData, const int64_t len) = 0;
  template<typename T> int64_t Write(const T *pData, const int64_t count = 1);
  template<typename T> int64_t Write(const T &data);
};

int64_t ctStreamWrite(ctWriteStream *pStream, const void *pData, const int64_t count);

template<typename T> int64_t ctWriteStream::Write(const T *pData, const int64_t count) { return ctStreamWrite(this, pData, count); }
template<typename T> int64_t ctWriteStream::Write(const T &data) { return Write(&data, 1); }

ctTrivialStreamWrite(int64_t);
ctTrivialStreamWrite(int32_t);
ctTrivialStreamWrite(int16_t);
ctTrivialStreamWrite(int8_t);

ctTrivialStreamWrite(uint64_t);
ctTrivialStreamWrite(uint32_t);
ctTrivialStreamWrite(uint16_t);
ctTrivialStreamWrite(uint8_t);
ctTrivialStreamWrite(bool)
ctTrivialStreamWrite(wchar_t);
ctTrivialStreamWrite(char);
ctTrivialStreamWrite(double);
ctTrivialStreamWrite(float);

#endif // atWriteStream_h__
