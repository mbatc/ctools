\
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

#ifndef _ctKeyValue_h__
#define _ctKeyValue_h__

#include "ctReadStream.h"
#include "ctWriteStream.h"

template<typename Key, class Val> class ctKeyValue
{
public:
  ctKeyValue() {}

  ctKeyValue(const Key &key, const Val &val)
    : m_key(key)
    , m_val(val)
  {}

  ctKeyValue(const Key &key, Val &&val)
    : m_key(key)
    , m_val(std::move(val))
  {}

  ctKeyValue(Key &&key, const Val &val)
    : m_key(std::move(key))
    , m_val(val)
  {}

  ctKeyValue(Key &&key, Val &&val)
    : m_key(std::move(key))
    , m_val(std::move(val))
  {}

  ctKeyValue(const ctKeyValue<Key, Val> &copy)
    : m_key(copy.m_key)
    , m_val(copy.m_val)
  {}

  ctKeyValue(ctKeyValue<Key, Val> &&move)
    : m_key(std::move(move.m_key))
    , m_val(std::move(move.m_val))
  {}

  bool compare(const ctKeyValue<Key, Val> &rhs) { return rhs.m_key == m_key && rhs.m_val == m_val; }
  bool operator==(const ctKeyValue<Key, Val> &rhs) { return compare(rhs); }

  const ctKeyValue<Key, Val>& operator=(const ctKeyValue<Key, Val> &rhs)
  {
    m_val = rhs.m_val;
    m_key = rhs.m_key;
    return *this;
  }

  const ctKeyValue<Key, Val>& operator=(ctKeyValue<Key, Val> &&rhs)
  {
    m_val = std::move(rhs.m_val);
    m_key = std::move(rhs.m_key);
    return *this;
  }

  Key m_key;
  Val m_val;
};

template<typename Key, class Val> int64_t ctStreamWrite(ctWriteStream *pStream, const ctKeyValue<Key, Val> *pData, const int64_t count)
{
  int64_t ret = 0;
  for (const ctKeyValue<Key, Val> &vec : ctIterate(pData, count))
  {
    ret += ctStreamWrite(pStream, &pData->m_key, 1);
    ret += ctStreamWrite(pStream, &pData->m_val, 1);
  }
  return ret;
}

template<typename Key, class Val> int64_t ctStreamRead(ctReadStream *pStream, ctKeyValue<Key, Val> *pData, const int64_t count)
{
  int64_t ret = 0;
  for (ctKeyValue<Key, Val> &vec : ctIterate(pData, count))
  {
    ret += ctStreamRead(pStream, &pData->m_key, 1);
    ret += ctStreamRead(pStream, &pData->m_val, 1);
  }
  return ret;
}

#endif // _ctKeyValue_h__
