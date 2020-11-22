#include "ctHash.h"

template<typename T>
inline ctHashSet<T>::ctHashSet(const int64_t &bucketCount)
{
  m_buckets.resize(ctMax(1LL, bucketCount));
  m_size = 0;
}

template<typename T>
inline ctHashSet<T>::ctHashSet(const std::initializer_list<T> &values)
  : ctHashSet(values.size() / m_itemCount + 1)
{
  for (const T &value : values)
    TryAdd(value);
}

template<typename T>
inline ctHashSet<T>::ctHashSet(const ctVector<T> &values)
  : ctHashSet(values.size() / m_itemCount + 1)
{
  for (const T &value : values)
    TryAdd(value);
}

template<typename T>
inline ctHashSet<T>::ctHashSet(std::initializer_list<T> &&values)
  : ctHashSet(values.size() / m_itemCount + 1)
{
  for (T &value : values)
    TryAdd(std::move(value));
  values.clear();
}

template<typename T>
inline ctHashSet<T>::ctHashSet(ctVector<T> &&values)
  : ctHashSet(values.size() / m_itemCount + 1)
{
  for (T &value : values)
    TryAdd(std::move(value));
  values.clear();
}

template<typename T>
inline ctHashSet<T>::ctHashSet(const ctHashSet<T> &o)
{
  m_buckets = o.m_buckets;
}

template<typename T>
inline ctHashSet<T>::ctHashSet(ctHashSet<T> &&o)
{
  m_buckets = std::move(o.m_buckets);
}

template<typename T>
inline void ctHashSet<T>::Clear()
{
  m_buckets.resize(1);
  m_buckets[0].clear();
  m_size = 0;
}

template<typename T>
inline void ctHashSet<T>::Add(const T &value)
{
  ctRelAssert(TryAdd(value), "Duplicate Key!");
}

template<typename T>
inline void ctHashSet<T>::Add(T &&value)
{
  ctRelAssert(TryAdd(std::forward<T>(value)), "Duplicate Key!");
}

template<typename T>
inline bool ctHashSet<T>::TryAdd(const T &value)
{
  Bucket &bucket = FindBucket(value);
  if (BucketContainsValue(bucket, value))
    return false;
  bucket.emplace_back(value);
  ++m_size;
  TryRehash();
  return true;
}

template<typename T>
inline bool ctHashSet<T>::TryAdd(T &&value)
{
  Bucket &bucket = FindBucket(value);
  if (BucketContainsValue(bucket, value))
    return false;
  bucket.emplace_back(std::move(value));
  ++m_size;
  TryRehash();
  return true;
}

template<typename T>
inline bool ctHashSet<T>::Remove(const T &value)
{
  Bucket &bucket = FindBucket(value);
  for (int64_t i = 0; i < bucket.size(); ++i)
    if (bucket[i] == value)
    {
      bucket.erase(i);
      --m_size;
      return true;
    }

  return false;
}

template<typename T>
inline bool ctHashSet<T>::Contains(const T &value) const
{
  return BucketContainsValue(FindBucket(value), value);
}

template<typename T>
inline void ctHashSet<T>::Rehash(const int64_t &bucketCount)
{
  ctHashSet<T> newMap(ctMax(1LL, bucketCount));
  for (T &value : *this)
    newMap.Add(std::move(value));
  *this = std::move(newMap);
}

template<typename T> inline int64_t ctHashSet<T>::Size() const { return m_size; }

template<typename T>
inline ctVector<T> ctHashSet<T>::Values() const
{
  ctVector<T> values;
  values.reserve(Size());
  for (const T &value : *this)
    values.push_back(value);
  return values;
}

template<typename T>
inline ctHashSet<T> ctHashSet<T>::InclusiveSubset(const ctHashSet<T> &set)
{
  ctHashSet<T> subset;
  for (const T &value : set)
    if (Contains(value))
      subset.Add(value);
  return subset;
}

template<typename T>
inline ctHashSet<T> ctHashSet<T>::ExclusiveSubset(const ctHashSet<T> &set)
{
  ctHashSet<T> subset;
  for (const T &value : set)
    if (!Contains(value))
      subset.Add(value);
  return subset;
}

template<typename T>
inline ctHashSet<T>& ctHashSet<T>::operator=(const ctHashSet<T> &o)
{
  m_buckets = o.m_buckets;
  m_size = o.m_size;
  return *this;
}

template<typename T>
inline ctHashSet<T>& ctHashSet<T>::operator=(ctHashSet<T> &&o)
{
  m_buckets = std::move(o.m_buckets);
  m_size = o.m_size;
  o.m_size = 0;
  return *this;
}

template<typename T>
inline bool ctHashSet<T>::operator==(const ctHashSet<T> &rhs) const
{
  if (rhs.Size() != Size())
    return false;
  for (const T &value : rhs)
    if (!Contains(value))
      return false;
  return true;
}

template<typename T>
inline bool ctHashSet<T>::operator!=(const ctHashSet<T> &rhs) const
{
  return !(*this == rhs);
}

template<typename T>
inline typename ctHashSet<T>::Iterator ctHashSet<T>::begin()
{
  return Iterator(&m_buckets, 0, 0);
}

template<typename T>
inline typename ctHashSet<T>::Iterator ctHashSet<T>::end()
{
  return Iterator(&m_buckets, m_buckets.size(), 0);
}

template<typename T>
inline typename ctHashSet<T>::ConstIterator ctHashSet<T>::begin() const
{
  return ConstIterator(&m_buckets, 0, 0);
}

template<typename T>
inline typename ctHashSet<T>::ConstIterator ctHashSet<T>::end() const
{
  return ConstIterator(&m_buckets, m_buckets.size(), 0);
}

template<typename T>
inline void ctHashSet<T>::TryRehash()
{
  if ((Size() + 1) / m_buckets.size() > m_itemCount)
    Rehash(ctMax(m_buckets.size() + 1, m_buckets.size() * _grow_rate));
}

template<typename T>
inline bool ctHashSet<T>::BucketContainsValue(const Bucket &bucket, const T &value) const
{
  for (const T &item : bucket)
    if (item == value)
      return true;
  return false;
}

template<typename T>
inline int64_t ctHashSet<T>::BucketIndex(const T &value) const
{
  return ctHash(value) % m_buckets.size();
}

template<typename T>
inline typename ctHashSet<T>::Bucket& ctHashSet<T>::FindBucket(const T &value)
{
  return m_buckets[BucketIndex(value)];
}

template<typename T>
inline const typename ctHashSet<T>::Bucket& ctHashSet<T>::FindBucket(const T &value) const
{
  return m_buckets[BucketIndex(value)];
}

template<typename T>
inline int64_t ctStreamRead(ctReadStream *pStream, ctHashSet<T> *pTarget, const int64_t count)
{
  int64_t len = 0;
  for (int64_t i = 0; i < count; ++i)
  {
    len += ctStreamRead(pStream, &pTarget[i].m_size, 1);
    len += ctStreamRead(pStream, &pTarget[i].m_buckets, 1);
  }
  return len;
}


template<typename T>
inline int64_t ctStreamWrite(ctWriteStream *pStream, ctHashSet<T> *pSource, const int64_t count)
{
  int64_t len = 0;
  for (int64_t i = 0; i < count; ++i)
  {
    len += ctStreamWrite(pStream, &pTarget[i].m_size, 1);
    len += ctStreamWrite(pStream, &pTarget[i].m_buckets, 1);
  }
  return len;
}

template<typename T> inline ctHashSet<T>::Iterator::Iterator(ctVector<Bucket> *pBuckets, const int64_t &bucketIdx, const int64_t &itemIdx)
  : m_pBuckets(pBuckets)
  , m_bucketIdx(bucketIdx)
  , m_itemIdx(itemIdx)
{}

template<typename T> inline bool ctHashSet<T>::Iterator::operator==(const typename Iterator &o) const
{
  return o.m_pBuckets == m_pBuckets && o.m_bucketIdx == m_bucketIdx && o.m_itemIdx == o.m_itemIdx;
}

template<typename T> inline bool ctHashSet<T>::Iterator::operator!=(const typename Iterator &o) const { return !(*this == o); }

template<typename T> inline typename ctHashSet<T>::Iterator& ctHashSet<T>::Iterator::operator++()
{
  m_itemIdx++;
  if (m_itemIdx >= m_pBuckets->at(m_bucketIdx).size())
  {
    m_itemIdx = 0;
    m_bucketIdx++;
  }
  return *this;
}

template<typename T> inline T* ctHashSet<T>::Iterator::operator->()
{
  return &(*this);
}

template<typename T> inline T& ctHashSet<T>::Iterator::operator*()
{
  return m_pBuckets->at(m_bucketIdx)[m_itemIdx];
}

template<typename T> inline ctHashSet<T>::ConstIterator::ConstIterator(const ctVector<Bucket> *pBuckets, const int64_t &bucketIdx, const int64_t &itemIdx) : Iterator(pBuckets, bucketIdx, itemIdx) {}
template<typename T> inline bool ctHashSet<T>::ConstIterator::operator==(const typename ConstIterator &o) const { return (ConstIterator&)Iterator::operator==((Iterator&)o); }
template<typename T> inline bool ctHashSet<T>::ConstIterator::operator!=(const typename ConstIterator &o) const { return (ConstIterator&)Iterator::operator++((Iterator&)o); }
template<typename T> inline typename ctHashSet<T>::ConstIterator& ctHashSet<T>::ConstIterator::operator++() { return (ConstIterator&)Iterator::operator++(); }
template<typename T> const T* ctHashSet<T>::ConstIterator::operator->() { return &(*((Iterator*)this)); }
template<typename T> const T& ctHashSet<T>::ConstIterator::operator*() { return *((Iterator*)this); }
