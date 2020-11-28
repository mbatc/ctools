#include "ctVector.h"

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

template<typename T> ctVector<T>::ctVector() {}
template<typename T> ctVector<T>::ctVector(const int64_t _reserve) { reserve(_reserve); }
template<typename T> ctVector<T>::ctVector(const std::initializer_list<T> &list) { assign(list.begin(), list.end()); }
template<typename T> ctVector<T>::ctVector(const T *pData, int64_t len) { assign(pData, pData + len); }
template<typename T> ctVector<T>::ctVector(const int64_t size, const T &initial) { assign(initial, size); }
template<typename T> ctVector<T>::ctVector(const ctVector<T> &copy) { assign(copy); }
template<typename T> ctVector<T>::ctVector(const std::vector<T> &copy) { assign(copy); }

template<typename T>
template<typename T2>
inline ctVector<T>::ctVector(const ctVector<T2> &vec)
{
  reserve(vec.size());
  for (const T2 &i : vec)
    push_back(T2(i));
}

template<typename T>
inline ctVector<T>::ctVector(ctVector<T> &&move)
{
  swap(move);
  move.make_empty();
}

template<typename T>
inline T& ctVector<T>::at(const int64_t index)
{
  ctAssert(index >= 0 && index < m_size, "Index out of Range");
  return m_pData[index];
}

template<typename T>
inline void ctVector<T>::set_data(T* pBuffer, const int64_t size, const int64_t capacity)
{
  make_empty();
  m_pData = pBuffer;
  m_size = size;
  m_capacity = capacity;
}

template<typename T>
inline T* ctVector<T>::take_data()
{
  T* ret = m_pData;
  m_pData = 0;
  m_size = 0;
  m_capacity = 0;
  return ret;
}

template<typename T>
inline void ctVector<T>::insert(const int64_t index, vector_const_iterator start, vector_const_iterator end)
{
  if (start >= end)
    return;

  int64_t startIndex = m_size;
  int64_t count = end - start;
  grow_reserve(m_size + count);
  ctUninitializedCopyArray(m_pData + m_size, start, count);
  m_size += count;
  move_to_index(startIndex, index, count);
}

template<typename T>
inline void ctVector<T>::insert_move(const int64_t index, vector_iterator start, vector_iterator end)
{
  if (start >= end)
    return;

  int64_t startIndex = m_size;
  int64_t count = end - start;
  grow_reserve(m_size + count);
  ctUninitializedMoveArray(m_pData + m_size, start, count);
  m_size += count;
  move_to_index(startIndex, index, count);
}


template<typename T>
template<typename... Args>
inline void ctVector<T>::emplace_back_array(const int64_t count, Args&&... args)
{
  grow_reserve(m_size + count);
  ctUninitializedFillArray(m_pData + m_size, count, T(std::forward<Args>(args)...));
  m_size += count;
}

template<typename T>
template<typename... Args>
inline void ctVector<T>::emplace_array(const int64_t index, const int64_t count, Args&&... args)
{
  emplace_back_array(count, std::forward<Args>(args)...);
  move_to_index(size() - count, index, count);
}

template<typename T>
template<typename... Args>
inline void ctVector<T>::emplace(const int64_t index, Args&&... args)
{
  emplace_back(std::forward<Args>(args)...);
  move_item(size() - 1, index);
}

template<typename T>
inline void ctVector<T>::resize(const int64_t size, const T &initial)
{
  if (try_resize(size, initial))
    return;
  bool growSucceeded = try_grow(size);
  ctAssert(growSucceeded, "Could not resize, try_grow failed!");
  bool resizeSucceeded = try_resize(size, initial);
  ctAssert(resizeSucceeded, "Could not resize ctVector");
}

template<typename T>
inline void ctVector<T>::resize(const int64_t size)
{
  if (try_resize(size))
    return;
  bool growSucceeded = try_grow(size);
  ctAssert(growSucceeded, "Could not resize, try_grow failed!");
  bool resizeSucceeded = try_resize(size);
  ctAssert(resizeSucceeded, "Could not resize ctVector");
}

template<typename T>
inline void ctVector<T>::reserve(const int64_t capacity)
{
  if (capacity == m_capacity)
    return; // no need to realloc
  if (try_realloc(capacity))
    return; // [capacity] is > [m_size] so realloc succeeded

  // [capacity] is < [m_size] so resize the vector before realloc
  bool shrinkSucceeded = shrink_by(m_size - capacity);
  ctAssert(shrinkSucceeded, "Could not resize, reserve failed!");
  try_realloc(capacity);
}

template<typename T>
inline void ctVector<T>::make_empty()
{
  clear();
  shrink_to_fit();
}

template<typename T>
inline void ctVector<T>::erase(const int64_t index)
{
  move_to_back(index, 1);
  pop_back();
}

template<typename T>
inline void ctVector<T>::erase(const int64_t start, const int64_t end)
{
  move_to_back(start, end - start);
  shrink_by(end - start);
}


template<typename T>
inline void ctVector<T>::assign(const T &item, const int64_t count)
{
  clear();
  reserve(count);
  emplace_back_array(count, item);
  m_size = count;
}

template<typename T>
inline void ctVector<T>::assign(vector_const_iterator start, vector_const_iterator end)
{
  const int64_t count = end - start;
  clear();
  reserve(count);
  ctUninitializedCopyArray(data(), start, count);
  m_size = count;
}

template<typename T>
template<typename T1>
inline void ctVector<T>::assign(const T1* start, const T1* end)
{
  const int64_t count = end - start;
  clear();
  reserve(count);
  ctUninitializedCopyArray(data(), start, count);
  m_size = count;
}

template<typename T>
template<typename T2>
inline ctVector<T>::operator std::vector<T2>()
{
  std::vector<T2> ret;
  ret.reserve(size());
  for (T &i : *this)
    ret.push_back(T2(i));
  return ret;
}

template<typename T> 
inline ctVector<T>::operator std::vector<T>()
{
  std::vector<T> ret;
  ret.reserve(size());
  for (T &i : *this)
    ret.push_back(i);
  return ret;
}

template<typename T>
inline const T& ctVector<T>::at(const int64_t index) const
{
  ctAssert(index >= 0 && index < m_size, "Index out of Range");
  return m_pData[index];
}

template<typename T>
inline const ctVector<T> &ctVector<T>::operator=(ctVector<T> &&rhs)
{
  swap(rhs);
  rhs.make_empty();
  return *this;
}

template<typename T>
inline const ctVector<T> &ctVector<T>::operator=(const ctVector<T> &rhs)
{
  assign(rhs);
  return *this;
}

template<typename T>
inline const ctVector<T> &ctVector<T>::operator+=(const ctVector<T> &rhs)
{
  insert(size(), rhs);
  return *this;
}

template<typename T>
inline const ctVector<T> &ctVector<T>::operator+(const ctVector<T> &rhs)
{
  ctVector<T> ret(size() + rhs.size());
  ret += *this;
  ret += rhs;
  return ret;
}

template<typename T>
inline void ctVector<T>::swap(ctVector<T> &with)
{
  std::swap(m_pData, with.m_pData);
  std::swap(m_size, with.m_size);
  std::swap(m_capacity, with.m_capacity);
}

template<typename T>
inline void ctVector<T>::move_item(const int64_t from, const int64_t to)
{
  const int64_t dir = from < to ? 1 : -1;
  for (int64_t i = from; i != to; i += dir)
    std::swap(at(i), at(i + dir));
}

template<typename T>
inline void ctVector<T>::move_to_index(const int64_t index, const int64_t to, const int64_t count) 
{
  if (index == to)
    return;
  for (int64_t i = 0; i < count; ++i)
    move_item(index + i, to + i);
}

template<typename T>
inline bool ctVector<T>::shrink_by(const int64_t count)
{
  if (count > m_size || count < 0)
    return false;
  ctDestructArray<T>(m_pData + m_size - count, count);
  m_size -= count;
  return true;
}

template<typename T>
inline void ctVector<T>::grow_reserve(const int64_t capacity)
{
  if (capacity <= m_capacity)
    return;
  try_grow(capacity);
}

template<typename T>
inline bool ctVector<T>::try_grow(const int64_t minSize)
{
  int64_t allocSize = (int64_t)((double)m_capacity * _grow_rate + 0.5);
  if (allocSize < minSize)
    allocSize = minSize;
  return try_realloc(allocSize);
}

template<typename T>
inline bool ctVector<T>::try_resize(const int64_t size, const T &initial)
{
  if (m_capacity < size)
    return false;
  if (m_size < size)
    emplace_back_array(size - m_size, initial); // emplace will call the default constructor of the new elements
  else if (m_size > size)
    shrink_by(m_size - size);
  m_size = size;
  return true;
}

template<typename T>
inline bool ctVector<T>::try_resize(const int64_t size)
{
  if (m_capacity < size)
    return false;
  if (m_size < size)
    emplace_back_array(size - m_size); // emplace will call the default constructor of the new elements
  else if (m_size > size)
    shrink_by(m_size - size);
  m_size = size;
  return true;
}

template<typename T>
inline bool ctVector<T>::try_realloc(const int64_t size)
{
  if (size < m_size)
    return false;
  realloc(size);
  return true;
}

template<typename T>
inline void ctVector<T>::realloc(const int64_t size)
{
  ctAssert(size >= m_size, "Realloc size not large enough to contain all items");
  T *pNew = nullptr;
  if (size > 0)
  {
    pNew = (T*)ctAlloc(size * sizeof(T));
    ctUninitializedMoveArray(pNew, m_pData, m_size);
    ctDestructArray(m_pData, m_size);
  }

  ctFree(m_pData);
  m_pData = pNew;
  m_capacity = size;
}

template<typename T> template<typename... Args> void ctVector<T>::emplace_back(Args&&... args)
{
  grow_reserve(m_size + 1);
  ctConstruct(m_pData + m_size, std::forward<Args>(args)...);
  m_size += 1;
}

template<typename T> void ctVector<T>::push_back(const ctVector<T> &item)
{
  insert(m_size, item.begin(), item.end());
}

template<typename T> void ctVector<T>::push_back(ctVector<T> &&item)
{
  insert_move(m_size, item.begin(), item.end());
  item.clear();
}

template<typename T> void ctVector<T>::move_to_back(const int64_t index, const int64_t count) { move_to_index(index, m_size - count, count); }
template<typename T> void ctVector<T>::move_to_front(const int64_t index, const int64_t count) { move_to_index(index, 0, count); }
template<typename T> typename ctVector<T>::vector_iterator ctVector<T>::begin() { return m_pData; }
template<typename T> typename ctVector<T>::vector_iterator ctVector<T>::end() { return m_pData + m_size; }
template<typename T> typename ctVector<T>::vector_const_iterator ctVector<T>::begin() const { return m_pData; }
template<typename T> typename ctVector<T>::vector_const_iterator ctVector<T>::end() const { return m_pData + m_size; }
template<typename T> const bool ctVector<T>::operator==(const ctVector<T>& rhs) const { return size() == rhs.size() && memcmp(rhs.data(), data(), size()) == 0; }
template<typename T> const bool ctVector<T>::operator!=(const ctVector<T>& rhs) const { return !(rhs == *this); }
template<typename T> const T& ctVector<T>::operator[](const int64_t index) const { return at(index); }
template<typename T> const T& ctVector<T>::operator[](const int32_t index) const { return at((int64_t)index); }
template<typename T> const T& ctVector<T>::operator[](const int16_t index) const { return at((int64_t)index); }
template<typename T> const T& ctVector<T>::operator[](const int8_t index) const { return at((int64_t)index); }
template<typename T> ctVector<T>::~ctVector() { make_empty(); }
template<typename T> T* ctVector<T>::data() { return m_pData; }
template<typename T> T& ctVector<T>::back() { return at(m_size - 1); }
template<typename T> T& ctVector<T>::front() { return at(0); }
template<typename T> void ctVector<T>::push_back(const T &item) { emplace_back(item); }
template<typename T> void ctVector<T>::push_back(T &&item) { emplace_back(std::move(item)); }
template<typename T> void ctVector<T>::pop_back() { shrink_by(1); }
template<typename T> void ctVector<T>::pop_front() { erase(0); }
template<typename T> void ctVector<T>::swap_pop_back(const int64_t index) { std::swap(at(index), back()); pop_back(); }
template<typename T> void ctVector<T>::swap_pop_front(const int64_t index) { std::swap(at(index), front()); pop_front(); }
template<typename T> void ctVector<T>::insert(const int64_t index, const T &item) { emplace(index, item); }
template<typename T> void ctVector<T>::insert(const int64_t index, const ctVector<T> &items) { insert(index, items.begin(), items.end()); }
template<typename T> void ctVector<T>::insert(const int64_t index, const std::vector<T> &items) { insert(index, items.begin(), items.end()); }
template<typename T> void ctVector<T>::shrink_to_fit() { reserve(m_size); }
template<typename T> void ctVector<T>::clear() { shrink_by(m_size); }
template<typename T> void ctVector<T>::erase(vector_iterator start, vector_iterator end) { erase(start - m_pData, end - m_pData); }
template<typename T> T& ctVector<T>::operator[](const int64_t index) { return at(index); }
template<typename T> T& ctVector<T>::operator[](const int32_t index) { return at((int64_t)index); }
template<typename T> T& ctVector<T>::operator[](const int16_t index) { return at((int64_t)index); }
template<typename T> T& ctVector<T>::operator[](const int8_t index) { return at((int64_t)index); }
template<typename T> const int64_t& ctVector<T>::size() const { return m_size; }
template<typename T> const T& ctVector<T>::front() const { return at(0); }
template<typename T> const T* ctVector<T>::data() const { return m_pData; }
template<typename T> bool ctVector<T>::empty() const { return m_size == 0; }
template<typename T> const int64_t& ctVector<T>::capacity() const { return m_capacity; }
template<typename T> const T& ctVector<T>::back() const { return at(m_size - 1); }
template<typename T> void ctVector<T>::assign(const ctVector<T> &copy) { assign(copy.begin(), copy.end()); }
template<typename T> void ctVector<T>::assign(const std::vector<T> &copy) { assign(copy.begin(), copy.end()); }
template<typename T> void ctVector<T>::assign(const std::initializer_list<T> &list) { assign(list.begin(), list.end()); }
