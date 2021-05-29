
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

#include "ctHashMap.h"

template<typename Key, class Value> ctHashMap<Key, Value>::ctHashMap(const ctHashMap<Key, Value> &copy) { m_buckets = copy.m_buckets; m_size = copy.m_size; }
template<typename Key, class Value> int64_t ctHashMap<Key, Value>::Size() const { return m_size; }
template<typename Key, class Value> void ctHashMap<Key, Value>::Clear() 
{
  for(Bucket &bucket : m_buckets)
    bucket.clear(); 
  m_size = 0; 
}

template<typename Key, class Value> ctHashMap<Key, Value>::ctHashMap(const int64_t bucketCount)
{
  m_buckets.resize(ctMax(bucketCount, 1));
  for (auto &bucket : m_buckets)
    bucket.reserve(m_itemCount);
  m_size = 0;
}

template<typename Key, class Value> ctHashMap<Key, Value>::ctHashMap(ctHashMap<Key, Value> &&move)
{
  m_buckets = std::move(move.m_buckets);
  m_size = move.m_size;
  move.m_size = 0;
  move.m_buckets.resize(1);
}

template<typename Key, class Value> bool ctHashMap<Key, Value>::TryAdd(const KVP &kvp)
{
  Bucket &bucket = GetBucket(kvp.m_key);
  for (KVP &item : bucket)
    if (item.m_key == kvp.m_key)
      return false;
  bucket.push_back(kvp);
  ++m_size;

  if (m_size > m_itemCount * m_buckets.size())
    Rehash((int64_t)(ctMax(m_buckets.size() * _grow_rate, 2)));
  return true;
}

template<typename Key, class Value> bool ctHashMap<Key, Value>::TryAdd(KVP &&kvp)
{
  Bucket &bucket = GetBucket(kvp.m_key);
  for (KVP &item : bucket)
    if (item.m_key == kvp.m_key)
      return false;
  bucket.push_back(kvp);
  ++m_size;

  if (m_size > m_itemCount * m_buckets.size())
    Rehash((int64_t)(ctMax(m_buckets.size() * _grow_rate, 2)));
  return true;
}

template<typename Key, class Value> bool ctHashMap<Key, Value>::Contains(const Key &key) const
{
  for (const KVP &kvp : GetBucket(key))
    if (kvp.m_key == key)
      return true;
  return false;
}

template<typename Key, class Value> bool ctHashMap<Key, Value>::Remove(const Key &key)
{
  Bucket &bucket = GetBucket(key);
  for (int64_t i = 0; i < bucket.size(); ++i)
    if (bucket[i].m_key == key)
    {
      bucket.swap_pop_back(i);
      --m_size;
      return true;
    }

  return false;
}

template<typename Key, class Value>
inline bool ctHashMap<Key, Value>::TrySet(const Key &key, Value &&val)
{
  Value *pValue = TryGet(key);
  if (!pValue)
    return false;
  *pValue = std::move(val);
  return true;
}

template<typename Key, class Value>
inline bool ctHashMap<Key, Value>::TrySet(const Key &key, const Value &val)
{
  return TrySet(key, Value(val));
}

template<typename Key, class Value> Value& ctHashMap<Key, Value>::GetOrAdd(const Key &key)
{
  Value *pVal = TryGet(key);
  if (!pVal)
  {
    Add(key, Value());
    pVal = TryGet(key);
  }
  return *pVal;
}

template<typename Key, class Value> Value& ctHashMap<Key, Value>::Get(const Key &key)
{
  for (KVP &kvp : GetBucket(key))
    if (kvp.m_key == key)
      return kvp.m_val;
  ctAssert(false, "[Key] does not exists");
  return (*m_buckets[0].data()).m_val;
}

template<typename Key, class Value> Value* ctHashMap<Key, Value>::TryGet(const Key &key)
{
  for (KVP &kvp : GetBucket(key))
    if (kvp.m_key == key)
      return &kvp.m_val;
  return nullptr;
}

template<typename Key, class Value> const Value& ctHashMap<Key, Value>::Get(const Key &key) const
{
  for (const KVP &kvp : GetBucket(key))
    if (kvp.m_key == key)
      return kvp.m_val;
  ctAssert(false, "[Key] does not exists");
  return (*m_buckets[0].data()).m_val;
}

template<typename Key, class Value> const Value* ctHashMap<Key, Value>::TryGet(const Key &key) const
{
  for (const KVP &kvp : GetBucket(key))
    if (kvp.m_key == key)
      return &kvp.m_val;
  return nullptr;
}

template<typename Key, class Value> Value ctHashMap<Key, Value>::GetOr(const Key &key, const Value &defaultVal) const
{
  const Value *pVal = TryGet(key);
  return pVal ? *pVal : defaultVal;
}

template<typename Key, class Value> ctVector<Key> ctHashMap<Key, Value>::GetKeys() const
{
  ctVector<Key> ret;
  for (const Bucket &bucket : m_buckets)
    for (const KVP &kvp : bucket)
      ret.push_back(kvp.m_key);
  return ret;
}

template<typename Key, class Value> ctVector<Value> ctHashMap<Key, Value>::GetValues() const
{
  ctVector<Value> ret;
  for (const Bucket &bucket : m_buckets)
    for (KVP &kvp : bucket)
      ret.push_back(kvp.m_val);
  return ret;
}

template<typename Key, class Value> const ctHashMap<Key, Value>& ctHashMap<Key, Value>::operator=(const ctHashMap<Key, Value>& rhs)
{
  m_buckets = rhs.m_buckets;
  m_size = rhs.m_size;
  return *this;
}

template<typename Key, class Value> int64_t ctHashMap<Key, Value>::StreamWrite(ctWriteStream *pStream, const ctHashMap<Key, Value> *pData, const int64_t count)
{
  int64_t ret = 0;
  for (const ctHashMap<Key, Value> &map : ctIterate(pData, count))
  {
    ret += ctStreamWrite(pStream, &map.m_size, 1);
    ret += ctStreamWrite(pStream, &map.m_buckets, 1);
  }
  return ret;
}

template<typename Key, class Value> int64_t ctHashMap<Key, Value>::StreamRead(ctReadStream *pStream, ctHashMap<Key, Value> *pData, const int64_t count)
{
  int64_t ret = 0;
  for (ctHashMap<Key, Value> &map : ctIterate(pData, count))
  {
    ret += ctStreamRead(pStream, &map.m_size, 1);
    ret += ctStreamRead(pStream, &map.m_buckets, 1);
  }
  return ret;
}

template<typename Key, class Value> bool ctHashMap<Key, Value>::Rehash(const int64_t bucketCount) 
{ 
  ctHashMap newMap(bucketCount);
  for (auto &kvp : *this)
    newMap.TryAdd(std::move(kvp));
  *this = std::move(newMap);
  return true;
}

template<typename Key, class Value> int64_t ctHashMap<Key, Value>::FindBucket(const Key &key) const { return abs(ctHash(key) % m_buckets.size()); }
template<typename Key, class Value> const typename ctHashMap<Key, Value>::Bucket& ctHashMap<Key, Value>::GetBucket(const Key & key) const { return m_buckets[m_buckets.size() > 1 ? FindBucket(key) : 0]; }
template<typename Key, class Value> typename ctHashMap<Key, Value>::ConstIterator ctHashMap<Key, Value>::begin() const { return ConstIterator(this, 0, m_buckets[0].data()); }
template<typename Key, class Value> typename ctHashMap<Key, Value>::ConstIterator ctHashMap<Key, Value>::end() const { return ConstIterator(this, m_buckets.size() - 1, m_buckets[m_buckets.size() - 1].end()); }
template<typename Key, class Value> typename ctHashMap<Key, Value>::Iterator ctHashMap<Key, Value>::begin() { return Iterator(this, 0, m_buckets[0].data()); }
template<typename Key, class Value> typename ctHashMap<Key, Value>::Iterator ctHashMap<Key, Value>::end() { return Iterator(this, m_buckets.size() - 1, m_buckets[m_buckets.size() - 1].end()); }
template<typename Key, class Value> typename ctHashMap<Key, Value>::Bucket& ctHashMap<Key, Value>::GetBucket(const Key &key) { return m_buckets[m_buckets.size() > 1 ? FindBucket(key) : 0]; }

template<typename Key, class Value> void ctHashMap<Key, Value>::Add(const Key &key, Value &&val)
{
  Add(KVP(key, std::move(val)));
}

template<typename Key, class Value> void ctHashMap<Key, Value>::Add(const Key &key)
{
  Add(key, Value());
}

template<typename Key, class Value> void ctHashMap<Key, Value>::Add(const Key &key, const Value &val)
{
  Add(KVP(key, val));
}

template<typename Key, class Value> void ctHashMap<Key, Value>::Add(const KVP &kvp) { Add(KVP(kvp)); }

template<typename Key, class Value> void ctHashMap<Key, Value>::Add(KVP &&kvp)
{
  bool addSuccess = TryAdd(std::move(kvp));
  ctAssert(addSuccess, "Duplicate Key!");
}

template<typename Key, class Value>
void ctHashMap<Key, Value>::AddOrSet(const KVP& key, const Value& value)
{
  Value * pValue = TryGet(key);
  if (pValue)
    *pValue = value;
  else
    Add(key, value);
}

template<typename Key, class Value> bool ctHashMap<Key, Value>::TryAdd(const Key &key, const Value &val) { return TryAdd(KVP(key, val)); }
template<typename Key, class Value> bool ctHashMap<Key, Value>::TryAdd(const Key &key, Value &&val) { return TryAdd(KVP(key, std::move(val))); }
template<typename Key, class Value> bool ctHashMap<Key, Value>::TryAdd(const Key &key) { return TryAdd(key, Value()); }

template<typename Key, class Value> const Value& ctHashMap<Key, Value>::operator[](const Key &key) const { return Get(key); }
template<typename Key, class Value> Value& ctHashMap<Key, Value>::operator[](const Key &key) { return Get(key); }

// -------------------------------------------------------
//                |** Hash Map Iterator **|

template<typename Key, class Value> ctHashMap<Key, Value>::Iterator::Iterator(ctHashMap<Key, Value> *pMap, const int64_t bucketIndex, ctKeyValue<Key, Value> *pBucketIterator)
{
  m_pMap = pMap;
  m_pKVP = pBucketIterator;
  m_bucket = bucketIndex;
}

template<typename Key, class Value> ctHashMap<Key, Value>::Iterator::Iterator(Iterator &&move)
  : Iterator(move.m_pMap, move.m_bucket, move.m_pKVP)
{
  move.m_pMap = nullptr;
  move.m_pKVP = nullptr;
  move.m_bucket = 0;
}

template<typename Key, class Value> const typename ctHashMap<Key, Value>::Iterator& ctHashMap<Key, Value>::Iterator::operator++()
{
  ++m_pKVP;
  while (m_pKVP >= m_pMap->m_buckets[m_bucket].end() && m_bucket < m_pMap->m_buckets.size() - 1)
    m_pKVP = m_pMap->m_buckets[++m_bucket].begin();
  return *this;
}

template<typename Key, class Value> ctHashMap<Key, Value>::ConstIterator::ConstIterator(const ctHashMap<Key, Value> *pMap, const int64_t bucketIndex, const ctKeyValue<Key, Value> *pBucketIterator)
{
  m_pMap = pMap;
  m_pKVP = pBucketIterator;
  m_bucket = bucketIndex;
}

template<typename Key, class Value> ctHashMap<Key, Value>::ConstIterator::ConstIterator(ConstIterator &&move)
  : ConstIterator(move.m_pMap, move.m_bucket, move.m_pKVP)
{
  move.m_pMap = nullptr;
  move.m_pKVP = nullptr;
  move.m_bucket = 0;
}

template<typename Key, class Value> const typename ctHashMap<Key, Value>::ConstIterator& ctHashMap<Key, Value>::ConstIterator::operator++()
{
  ++m_pKVP;
  while (m_pKVP >= m_pMap->m_buckets[m_bucket].end() && m_bucket < m_pMap->m_buckets.size() - 1)
    m_pKVP = m_pMap->m_buckets[++m_bucket].begin();
  return *this;
}

template<typename Key, class Value> ctHashMap<Key, Value>::Iterator::Iterator(const Iterator &copy) : Iterator(copy.m_pMap, copy.m_bucket, copy.m_pKVP) {}
template<typename Key, class Value> bool ctHashMap<Key, Value>::Iterator::operator==(const Iterator &rhs) const { return m_pKVP == rhs.m_pKVP; }
template<typename Key, class Value> bool ctHashMap<Key, Value>::Iterator::operator!=(const Iterator &rhs) const { return m_pKVP != rhs.m_pKVP; }
template<typename Key, class Value> ctKeyValue<Key, Value>* ctHashMap<Key, Value>::Iterator::operator->() { return m_pKVP; }
template<typename Key, class Value> ctKeyValue<Key, Value>& ctHashMap<Key, Value>::Iterator::operator*() { return *m_pKVP; }
template<typename Key, class Value> ctHashMap<Key, Value>::ConstIterator::ConstIterator(const ConstIterator &copy) : ConstIterator(copy.m_pMap, copy.m_bucket, copy.m_pKVP) {}
template<typename Key, class Value> bool ctHashMap<Key, Value>::ConstIterator::operator==(const ConstIterator &rhs) const { return m_pKVP == rhs.m_pKVP; }
template<typename Key, class Value> bool ctHashMap<Key, Value>::ConstIterator::operator!=(const ConstIterator &rhs) const { return m_pKVP != rhs.m_pKVP; }
template<typename Key, class Value> const ctKeyValue<Key, Value>* ctHashMap<Key, Value>::ConstIterator::operator->() const { return m_pKVP; }
template<typename Key, class Value> const ctKeyValue<Key, Value>& ctHashMap<Key, Value>::ConstIterator::operator*() const { return *m_pKVP; }
