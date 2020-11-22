#ifndef atPtr_h__
#define atPtr_h__

#include <atomic>
#include <functional>

#include "ctHash.h"
#include "ctMemory.h"

template<typename T> class ctPtr
{
protected:
  struct Instance
  {
    T *pData = nullptr;                 // Raw pointer
    bool isForeign = false;             // Was this created by atPtr
    std::atomic<int64_t> refCount;      // How many references are there
    std::function<void(T *)> onDelete;  // What to do when deleting the pointer
    std::function<void(T *, int64_t)> onRelease; // What to do when releasing a reference to the pointer
    std::function<void(T *, int64_t)> onAcquire; // What to do when acquiring a reference to the pointer
  } *m_pInstance = nullptr;

public:
  ctPtr(T *pPtr = nullptr,
    std::function<void(T *)> onDelete = [](T *pPtr) { ctDelete(pPtr); },
    std::function<void(T *, int64_t)> onRelease = nullptr,
    std::function<void(T *, int64_t)> onAcquire = nullptr);

  ctPtr(std::nullptr_t);

  ctPtr(T &&o);
  ctPtr(const T &o);
  ctPtr(ctPtr<T> &&o);
  ctPtr(const ctPtr<T> &o);

  // Create a pointer to a derived type
  template<typename T2> explicit ctPtr(T2 &&o);
  template<typename T2> explicit ctPtr(const T2 &o);

  T* Get();
  T* operator->();
  T& operator*();

  const T* Get() const;
  const T* operator->() const;
  const T& operator*() const;

  ctPtr<T> &operator=(T *pPtr);
  ctPtr<T>& operator=(ctPtr<T> &&rhs);
  ctPtr<T>& operator=(const ctPtr<T> &rhs);

  operator bool() const;
  bool operator==(const T *pRhs) const;
  bool operator==(const std::nullptr_t &rhs) const;
  bool operator==(const ctPtr<T> &rhs) const;
  bool operator!=(const T *pRhs) const;
  bool operator!=(const std::nullptr_t &rhs) const;
  bool operator!=(const ctPtr<T> &rhs) const;
  bool operator<(const T *pRhs) const;
  bool operator<(const ctPtr<T> &rhs) const;
  bool operator>(const T *pRhs) const;
  bool operator>(const ctPtr<T> &rhs) const;
  bool operator<=(const T *pRhs) const;
  bool operator<=(const ctPtr<T> &rhs) const;
  bool operator>=(const T *pRhs) const;
  bool operator>=(const ctPtr<T> &rhs) const;

  // Get the number of references to this pointer
  int64_t GetReferenceCount() const;

  // Set a function to be called any time a reference is released
  // Can be null to clear a callback set previously.
  // The callback takes a T* (internal pointer) and an int64_t (refcount)
  // Returns false if this pointer is null
  // Returns true on success
  bool OnRelease(std::function<void(T*, int64_t)> onRelease);

  // Set a function to be called any time a reference is acquired
  // Can be null to clear a callback set previously.
  // The callback takes a T* (internal pointer) and an int64_t (refcount)
  // Returns false if this pointer is null
  // Returns true on success
  bool OnAqcuire(std::function<void(T*, int64_t)> onAcquire);

  // If the internal pointer is foreign, this can be used
  // to set how the pointer should be deleted.
  // Returns false if this pointer is null
  // Returns false if this pointer is not foreign
  // Returns true on success
  bool OnDelete(std::function<void(T *)>onDelete);

  bool IsForeign() const;
  bool IsNull() const;

  // Take ownership of the internal pointer.
  // This will set all atPtr's with this reference to null.
  T* TakePtr();

  template<typename U> friend int64_t atHash(const ctPtr<U> &o);

protected:
  void Release();
  void Acquire(Instance *pInstance);
  void Create(T *pData, bool isForeign,
    std::function<void(T *)> onDelete,
    std::function<void(T *, int64_t)> onRelease,
    std::function<void(T *, int64_t)> onAcquire);
};

#include "ctPtr.inl"
#endif // atPtr_h__
