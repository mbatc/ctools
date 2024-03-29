
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

#ifndef atMemoryWriter_h__
#define atMemoryWriter_h__

#include "ctVector.h"
#include "ctWriteStream.h"

class ctMemoryWriter : public ctWriteStream
{
public:
  ctMemoryWriter();
  ctMemoryWriter(ctMemoryWriter &&move);
  ctMemoryWriter(const ctMemoryWriter &move);
  ~ctMemoryWriter();

  void Clear();
  bool Seek(const int64_t offset, const ctSeekOrigin origin = atSO_Start) override;

  int64_t Write(const void *pData, const int64_t len) override;
  template<typename T> int64_t Write(const T &data);
  template<typename T> int64_t Write(const T *pData, const int64_t count);

  int64_t Length() const override;

  bool operator!=(const ctMemoryWriter &rhs);
  bool operator==(const ctMemoryWriter &rhs);

  ctVector<uint8_t> m_data;
  
protected:
  int64_t m_pos = 0;
};

template<typename T> int64_t ctMemoryWriter::Write(const T *pData, const int64_t count) { return ctStreamWrite(this, pData, count); }
template<typename T> int64_t ctMemoryWriter::Write(const T &data) { return Write(&data, 1); }

#endif // atMemoryWriter_h__
