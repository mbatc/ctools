
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

#include "ctMemoryWriter.h"

ctMemoryWriter::ctMemoryWriter() {}
ctMemoryWriter::ctMemoryWriter(ctMemoryWriter &&move) : m_data(std::move(move.m_data)) {}
ctMemoryWriter::ctMemoryWriter(const ctMemoryWriter &copy) : m_data(copy.m_data) {}
ctMemoryWriter::~ctMemoryWriter() {}

int64_t ctMemoryWriter::Write(const void *pData, const int64_t len)
{
  m_data.resize(ctMax(m_pos + len, m_data.size()));
  memcpy(m_data.data() + m_pos, pData, (size_t)len);
  m_pos += len;
  return len;
}

void ctMemoryWriter::Clear()
{
  m_data.clear();
  m_pos = 0;
}

bool ctMemoryWriter::Seek(const int64_t offset, const ctSeekOrigin origin)
{
  switch (origin)
  {
  case atSO_Current: m_pos = m_pos + offset; break;
  case atSO_End: m_pos = m_data.size() + offset; break;
  case atSO_Start: m_pos = 0 + offset; break;
  default: return false;
  }

  m_pos = ctMax(0, ctMin(m_pos, m_data.size()));
  return true;
}

int64_t ctMemoryWriter::Length() const
{
  return m_data.size();
}

bool ctMemoryWriter::operator!=(const ctMemoryWriter &rhs) { return !(*this == rhs); }
bool ctMemoryWriter::operator==(const ctMemoryWriter &rhs) { return rhs.m_data.size() == m_data.size() && memcmp(rhs.m_data.data(), m_data.data(), (size_t)m_data.size()) == 0; }
