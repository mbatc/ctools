#ifndef atHashSet_h__
#define atHashSet_h__

#include "ctVector.h"

template <typename T> class ctHashSet
{
  const double _grow_rate = 1.61803399; // PHI
  const int64_t m_itemCount = 16;

public:
  typedef ctVector<T> Bucket;

  class Iterator
  {
  public:
    Iterator(ctVector<Bucket> *pBuckets, const int64_t &bucketIdx, const int64_t &itemIdx);
    bool operator==(const Iterator &o) const;
    bool operator!=(const Iterator &o) const;
    Iterator& operator++();

    T* operator->();
    T& operator*();

  protected:
    ctVector<Bucket> *m_pBuckets;
    int64_t m_bucketIdx;
    int64_t m_itemIdx;
  };

  class ConstIterator : public Iterator
  {
  public:
    ConstIterator(const ctVector<Bucket> *pBuckets, const int64_t &bucketIdx, const int64_t &itemIdx);
    bool operator==(const ConstIterator &o) const;
    bool operator!=(const ConstIterator &o) const;
    ConstIterator& operator++();

    const T* operator->();
    const T& operator*();
  };

  ctHashSet(const int64_t &bucketCount = 1);
  ctHashSet(const std::initializer_list<T> &values);
  ctHashSet(const ctVector<T> &values);
  ctHashSet(std::initializer_list<T> &&values);
  ctHashSet(ctVector<T> &&values);

  ctHashSet(const ctHashSet<T> &o);
  ctHashSet(ctHashSet<T> &&o);

  void Clear();
  void Add(const T &value);
  void Add(T &&value);
  bool TryAdd(const T &value);
  bool TryAdd(T &&value);
  bool Remove(const T &value);
  bool Contains(const T &value) const;

  void Rehash(const int64_t &bucketCount);

  int64_t Size() const;

  ctVector<T> Values() const;
  ctHashSet<T> InclusiveSubset(const ctHashSet<T> &set);
  ctHashSet<T> ExclusiveSubset(const ctHashSet<T> &set);

  ctHashSet<T>& operator=(const ctHashSet<T> &o);
  ctHashSet<T>& operator=(ctHashSet<T> &&o);

  bool operator==(const ctHashSet<T> &rhs) const;
  bool operator!=(const ctHashSet<T> &rhs) const;

  template<typename T> friend int64_t ctStreamRead(ctReadStream *pStream, ctHashSet<T> *pTarget, const int64_t count);
  template<typename T> friend int64_t ctStreamWrite(ctWriteStream *pStream, ctHashSet<T> *pSource, const int64_t count);

  Iterator begin();
  Iterator end();

  ConstIterator begin() const;
  ConstIterator end() const;

protected:
  void TryRehash();

  ctVector<Bucket> m_buckets;
  int64_t m_size;

  bool BucketContainsValue(const Bucket &bucket, const T &value) const;

  int64_t BucketIndex(const T &value) const;
  Bucket& FindBucket(const T &value);
  const Bucket& FindBucket(const T &value) const;
};

#include "ctHashSet.inl"

#endif // atHashSet_h__
