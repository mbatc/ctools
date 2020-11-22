
template<typename T>
inline ctPtr<T>::ctPtr(T *pPtr, std::function<void(T *)> onDelete,
  std::function<void(T *, int64_t)> onRelease,
  std::function<void(T *, int64_t)> onAcquire)
{
  Create(pPtr, true, onDelete, onRelease, onAcquire);
}

template<typename T> inline ctPtr<T>::ctPtr(std::nullptr_t) : ctPtr((T*)nullptr) {}

template<typename T>
inline ctPtr<T>::ctPtr(T &&o)
{
  Create(ctNew(T)(std::move(o)), false, [](T *pPtr) { ctDelete(pPtr); }, nullptr, nullptr);
}

template<typename T>
inline ctPtr<T>::ctPtr(const T &o)
  : ctPtr(std::move(T(o)))
{}

template<typename T> inline ctPtr<T>::ctPtr(ctPtr<T> &&o) { *this = std::move(o); }
template<typename T> inline ctPtr<T>::ctPtr(const ctPtr<T> &o) { *this = o; }

template<typename T>
template<typename T2>
inline ctPtr<T>::ctPtr(T2 &&o)
{
  Create((T*)ctNew(T2)(std::move(o)), false, [](T *pPtr) { ctDelete((T2 *)pPtr); }, nullptr, nullptr);
}

template<typename T>
template<typename T2>
inline ctPtr<T>::ctPtr(const T2 &o)
  : ctPtr(T2(o))
{}

template<typename T> inline T *ctPtr<T>::Get() { return m_pInstance ? m_pInstance->pData : nullptr; }
template<typename T> inline T *ctPtr<T>::operator->() { return Get(); }

template<typename T>
inline T &ctPtr<T>::operator*()
{
  ctAssert(!IsNull(), "Dereferencing a null ptr");
  return *Get();
}

template<typename T> inline const T *ctPtr<T>::Get() const { return m_pInstance ? m_pInstance->pData : nullptr; }
template<typename T> inline const T *ctPtr<T>::operator->() const { return Get(); }

template<typename T>
inline const T &ctPtr<T>::operator*() const
{
  ctAssert(!IsNull(), "Dereferencing a null ptr");
  return *Get();
}

template<typename T>
inline ctPtr<T>& ctPtr<T>::operator=(T *pPtr)
{
  if (pPtr == Get())
    return *this;
  Create(pPtr, true, nullptr, nullptr, nullptr);
  return *this;
}

template<typename T>
inline ctPtr<T> &ctPtr<T>::operator=(ctPtr<T> &&rhs)
{
  Release();
  std::swap(m_pInstance, rhs.m_pInstance);
  return *this;
}

template<typename T>
inline ctPtr<T> &ctPtr<T>::operator=(const ctPtr<T> &rhs)
{
  Acquire(rhs.m_pInstance);
  return *this;
}

template<typename T> inline int64_t ctPtr<T>::GetReferenceCount() const { return m_pInstance ? m_pInstance->refCount.load() : 1; }

template<typename T>
inline bool ctPtr<T>::OnRelease(std::function<void(T *, int64_t)> onRelease)
{
  if (!m_pInstance)
    return false;
  m_pInstance->onRelease = onRelease;
  return true;
}

template<typename T>
inline bool ctPtr<T>::OnAqcuire(std::function<void(T *, int64_t)> onAcquire)
{
  if (!m_pInstance)
    return false;
  m_pInstance->onAcquire = onAcquire;
  return true;
}

template<typename T>
inline bool ctPtr<T>::OnDelete(std::function<void(T *)> onDelete)
{
  if (!m_pInstance || !m_pInstance->isForeign)
    return false;
  m_pInstance->onDelete = onDelete;
  return true;
}

template<typename T> inline bool ctPtr<T>::IsForeign() const { return m_pInstance && m_pInstance->isForeign; }
template<typename T> inline bool ctPtr<T>::IsNull() const { return Get() == nullptr; }

template<typename T> inline T *ctPtr<T>::TakePtr()
{
  T *pPtr = m_pInstance->pData;
  m_pInstance->pData = nullptr;
  return pPtr;
}

template<typename T>
inline void ctPtr<T>::Release()
{
  if (!m_pInstance)
    return;
  Instance *pInstance = m_pInstance;
  m_pInstance = nullptr;
  int64_t refCount = --pInstance->refCount;

  // Try call the released callback
  if (pInstance->onRelease)
    pInstance->onRelease(pInstance->pData, refCount);

  // If there are no more references delete the ptr and
  // the instance struct
  if (refCount == 0)
  {
    if (pInstance->pData && pInstance->onDelete)
      pInstance->onDelete(pInstance->pData);
    ctDelete(pInstance);
  }
}

template<typename T>
inline void ctPtr<T>::Acquire(typename ctPtr<T>::Instance *pInstance)
{
  if (m_pInstance == pInstance)
    return; // Same ptr

  Release(); // Release the current ptr
  if (!pInstance)
    return; // Setting to null

  int64_t refCount = ++pInstance->refCount;
  m_pInstance = pInstance;

  // Try call acquire callback
  if (m_pInstance->onAcquire)
    m_pInstance->onAcquire(m_pInstance->pData, refCount);
}

template<typename T>
inline void ctPtr<T>::Create(T *pData, bool isForeign, std::function<void(T *)> onDelete, std::function<void(T *, int64_t)> onRelease, std::function<void(T *, int64_t)> onAcquire)
{
  Release();
  m_pInstance = ctNew(Instance);
  m_pInstance->pData = pData;
  m_pInstance->isForeign = true;
  m_pInstance->refCount = 1;
  m_pInstance->onDelete = onDelete;
  m_pInstance->onRelease = onRelease;
  m_pInstance->onAcquire = onAcquire;
}

template<typename T> inline ctPtr<T>::operator bool() const { return !IsNull(); }
template<typename T> inline bool ctPtr<T>::operator==(const T *pRhs) const { return Get() == pRhs; }
template<typename T> inline bool ctPtr<T>::operator==(const std::nullptr_t &) const { return IsNull(); }
template<typename T> inline bool ctPtr<T>::operator==(const ctPtr<T> &rhs) const { return Get() == rhs.Get(); }
template<typename T> inline bool ctPtr<T>::operator!=(const T *pRhs) const { return !(*this == pRhs); }
template<typename T> inline bool ctPtr<T>::operator!=(const std::nullptr_t &) const { return !IsNull(); }
template<typename T> inline bool ctPtr<T>::operator!=(const ctPtr<T> &rhs) const { return !(*this == rhs); }
template<typename T> inline bool ctPtr<T>::operator<(const T *pRhs) const { return Get() < pRhs; }
template<typename T> inline bool ctPtr<T>::operator<(const ctPtr<T> &rhs) const { return *this < rhs.Get(); }
template<typename T> inline bool ctPtr<T>::operator>(const T *pRhs) const { return Get > pRhs; }
template<typename T> inline bool ctPtr<T>::operator>(const ctPtr<T> &rhs) const { return *this > rhs.Get(); }
template<typename T> inline bool ctPtr<T>::operator<=(const T *pRhs) const { return Get() <= pRhs; }
template<typename T> inline bool ctPtr<T>::operator<=(const ctPtr<T> &rhs) const { return *this <= rhs.Get(); }
template<typename T> inline bool ctPtr<T>::operator>=(const T *pRhs) const { return Get >= pRhs; }
template<typename T> inline bool ctPtr<T>::operator>=(const ctPtr<T> &rhs) const { return *this >= rhs.Get(); }
template<typename U> inline int64_t ctHash(const ctPtr<U> &o) { return ctHash(o.Get()) ;}
