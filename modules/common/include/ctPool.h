#ifndef atPool_h__
#define atPool_h__

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

#include "ctVector.h"

template<typename T> class ctPool
{
public:
  class Iterator
  {
    friend ctPool;
    Iterator(ctPool *pPool, int64_t start)
    {
      m_pPool = pPool;
      m_index = start;
      EnsureValid();
    }

  public:
    Iterator(const Iterator &it)
    {
      m_pPool = it.m_pPool;
      m_index = it.m_index;
    }

    bool operator==(const Iterator &rhs) const { return rhs.m_index == m_index && rhs.m_pPool == m_pPool; }
    bool operator!=(const Iterator &rhs) const { return !(*this == rhs); }

    Iterator& operator++()
    {
      m_index = ctMin(m_index + 1, m_pPool->capacity());
      EnsureValid();
      return *this;
    }

    T *operator->() const { return &m_pPool->at(m_index); }
    T &operator*() const { return m_pPool->at(m_index); }

  protected:
    void EnsureValid()
    {
      while (m_index < m_pPool->capacity() && !m_pPool->m_usedFlags[m_index])
        ++m_index;
    }

    ctPool *m_pPool = nullptr;
    int64_t m_index = 0;
  };

  class ConstIterator : public Iterator
  {
    friend ctPool;
    ConstIterator(ctPool *pPool, int64_t start) : Iterator(pPool, start) {}

  public:
    ConstIterator(const ConstIterator &it) : Iterator((Iterator const &)it) {}

    bool operator==(const ConstIterator &rhs) const{ return Iterator::operator==((Iterator const &)rhs); }
    bool operator!=(const ConstIterator &rhs) const { return Iterator::operator!=((Iterator const &)rhs); }
    ConstIterator& operator++() { return (ConstIterator&)Iterator::operator++(); }
    const T* operator->() const { return Iterator::operator->(); }
    const T &operator*() const { return Iterator::operator*(); }
  };

  // Construct a pool with the specified initial capacity
  ctPool(const int64_t &capacity = 1);

  // Get the number of the items in the pool
  const int64_t& size() const;

  // Get the capacity of the pool
  const int64_t& capacity() const;

  // Get the item at the specified index
  T& at(const int64_t &index);
  const T& at(const int64_t &index) const;

  // Add an item in the next available slot
  // Returns the index of the slot it was added to
  int64_t Add(const T &value);
  int64_t Add(T &&value);

  // Check if an index is valid
  bool Contains(const int64_t &index) const;

  // Erase the item at the specified index
  bool erase(const int64_t &index);

  // Construct an item in place in the next available slot
  template<typename... Args> int64_t emplace(Args&&... args);

  // Accessor operators
  T& operator[](const int64_t &index);
  const T& operator[](const int64_t &index) const;

  Iterator begin();
  Iterator end();

  ConstIterator begin() const;
  ConstIterator end() const;

protected:
  bool TryGrow(const int64_t &requiredCapacity);
  bool Grow(const int64_t &capacity);
  int64_t GetNextAvailSlot();
  
  T *m_pData;
  int64_t m_capacity;
  int64_t m_size;

  ctVector<bool> m_usedFlags;
  ctVector<int64_t> m_freeSlots;
};

#include "ctPool.inl"

#endif // atPool_h__
