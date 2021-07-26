
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

#ifndef atHashMap_h__
#define atHashMap_h__

#include "ctKeyValue.h"
#include "ctVector.h"
#include "ctHash.h"

template<typename Key, class Value> class ctHashMap
{
public:
  const double _grow_rate = 1.61803399; // PHI
  const int64_t m_itemCount = 16;

  typedef ctKeyValue<Key, Value> KVP;
  typedef ctVector<KVP> Bucket;

  class Iterator
  {
  public:
    Iterator(ctHashMap<Key, Value> *pMap, const int64_t bucketIndex, ctKeyValue<Key, Value> *pBucketIterator);
    Iterator(const Iterator &copy);
    Iterator(Iterator &&move);

    bool operator==(const Iterator &rhs) const;
    bool operator!=(const Iterator &rhs) const;

    ctKeyValue<Key, Value>* operator->();
    ctKeyValue<Key, Value>& operator*();
    const Iterator& operator++();

  protected:
    ctKeyValue<Key, Value> *m_pKVP;
    ctHashMap<Key, Value> *m_pMap;
    int64_t m_bucket;
  };

  class ConstIterator
  {
  public:
    ConstIterator(const ctHashMap<Key, Value> *pMap, const int64_t bucketIndex, const ctKeyValue<Key, Value> *pBucketIterator);
    ConstIterator(const ConstIterator &copy);
    ConstIterator(ConstIterator &&move);

    bool operator==(const ConstIterator &rhs) const;
    bool operator!=(const ConstIterator &rhs) const;

    const ctKeyValue<Key, Value>* operator->() const;
    const ctKeyValue<Key, Value>& operator*() const;
    const ConstIterator& operator++();

  protected:
    const ctKeyValue<Key, Value> *m_pKVP;
    const ctHashMap<Key, Value> *m_pMap;
    int64_t m_bucket;
  };

  ctHashMap(const int64_t bucketCount = 1);
  ctHashMap(const ctHashMap<Key, Value> &copy);
  ctHashMap(ctHashMap<Key, Value> &&move);

  void Clear();
  int64_t Size() const;

  void Add(const Key &key, Value &&val);
  void Add(const Key &key);
  void Add(const Key &key, const Value &val);
  void Add(const KVP &kvp);
  void Add(KVP &&kvp);

  void AddOrSet(const Key&key, const Value &value);

  bool TryAdd(const Key &key, Value &&val);
  bool TryAdd(const Key &key);
  bool TryAdd(const Key &key, const Value &val);
  bool TryAdd(const KVP &kvp);
  bool TryAdd(KVP &&kvp);

  bool Contains(const Key &key) const;

  bool Remove(const Key &key);

  // Try set an existing value in the hashmap
  bool TrySet(const Key &key, Value &&val);
  bool TrySet(const Key &key, const Value &val);

  Value& GetOrAdd(const Key &key);
  Value& Get(const Key &key);
  Value* TryGet(const Key &key);
  Value& operator[](const Key &key);

  const Value* TryGet(const Key &key) const;
  const Value& Get(const Key &key) const;
  const Value& operator[](const Key &key) const;

  Value GetOr(const Key &key, const Value &value) const;

  ctVector<Key> GetKeys() const;
  ctVector<Value> GetValues() const;
  
  Iterator begin();
  Iterator end();
  ConstIterator begin() const;
  ConstIterator end() const;

  const ctHashMap<Key, Value>& operator=(const ctHashMap<Key, Value> &rhs);

  static int64_t StreamWrite(ctWriteStream *pStream, const ctHashMap<Key, Value> *pData, const int64_t count);
  static int64_t StreamRead(ctReadStream *pStream, ctHashMap<Key, Value> *pData, const int64_t count);

protected:
  Bucket &GetBucket(const Key &key);
  const Bucket &GetBucket(const Key &key) const;

  bool Rehash(const int64_t bucketCount);
  int64_t FindBucket(const Key &key) const;

  ctVector<Bucket> m_buckets;
  int64_t m_size;
};

template<typename Key, typename Value> int64_t ctStreamWrite(ctWriteStream *pStream, const ctHashMap<Key, Value> *pData, const int64_t count)
{
  return ctHashMap<Key, Value>::StreamWrite(pStream, pData, count);
}

template<typename Key, typename Value> int64_t ctStreamRead(ctReadStream *pStream, ctHashMap<Key, Value> *pData, const int64_t count)
{
  return ctHashMap<Key, Value>::StreamRead(pStream, pData, count);
}

#include "ctHashMap.inl"
#endif // atHashMap_h__
