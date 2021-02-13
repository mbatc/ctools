#include "ctPool.h"

template<typename T>
inline ctPool<T>::ctPool(const int64_t &capacity)
  : m_capacity(0)
  , m_size(0)
  , m_pData(0)
{
  TryGrow(capacity);
}

template<typename T>
inline const int64_t& ctPool<T>::size() const
{
  return m_size;
}

template<typename T>
inline const int64_t& ctPool<T>::capacity() const
{
  return m_capacity;
}

template<typename T>
inline T& ctPool<T>::at(const int64_t &index)
{
  return m_pData[index];
}

template<typename T>
inline const T& ctPool<T>::at(const int64_t &index) const
{
  return m_pData[index];
}

template<typename T>
inline int64_t ctPool<T>::Add(const T &value)
{
  return emplace(value);
}

template<typename T>
inline int64_t ctPool<T>::Add(T &&value)
{
  return emplace(std::move(value));
}

template<typename T>
inline bool ctPool<T>::Contains(const int64_t &index) const
{
  return index < 0 || index >= m_capacity ? false : m_usedFlags[index];
}

template<typename T>
inline bool ctPool<T>::erase(const int64_t &index)
{
  if (!m_usedFlags[index])
    return false;

  ctDestructArray(m_pData, 1);
  m_freeSlots.push_back(index);
  m_usedFlags[index] = false;
  return true;
}

template<typename T>
inline T& ctPool<T>::operator[](const int64_t &index)
{
  return at(index);
}

template<typename T>
inline const T& ctPool<T>::operator[](const int64_t &index) const
{
  return at(index);
}

template<typename T>
inline bool ctPool<T>::TryGrow(const int64_t &requiredCapacity)
{
  return m_capacity > requiredCapacity || Grow(ctMax(requiredCapacity, m_capacity * 2));
}

template<typename T>
inline bool ctPool<T>::Grow(const int64_t &capacity)
{
  T *pNewMem = (T*)ctAlloc(sizeof(T) * capacity);
  if (!pNewMem)
    return false;

  // Move existing items to new memory block
  for (int64_t i = 0; i < m_capacity && i < capacity; ++i)
    if (m_usedFlags[i])
    {
      ctUninitializedMoveArray(pNewMem + i, m_pData + i, 1);
      ctDestructArray(m_pData + i, 1);
    }

  ctFree(m_pData);

  m_capacity = capacity;
  m_usedFlags.resize(capacity, false);
  m_pData = pNewMem;
  return true;
}

template<typename T>
inline int64_t ctPool<T>::GetNextAvailSlot()
{
  if (m_freeSlots.size() > 0)
  {
    int64_t idx = m_freeSlots.back();
    m_freeSlots.pop_back();
    return idx;
  }

  return m_size;
}

template<typename T>
template<typename ...Args>
inline int64_t ctPool<T>::emplace(Args&&... args)
{
  int64_t slot = GetNextAvailSlot();
  if (!TryGrow(m_size + 1))
    return -1;

  ++m_size;
  ctUninitializedFillArray(m_pData + slot, 1, T(std::forward<Args>(args)...));
  m_usedFlags[slot] = true;
  return slot;
}
