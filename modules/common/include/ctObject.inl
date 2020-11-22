#include "ctObject.h"

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

template<typename T>
inline bool ctObject::Is() const
{
  return m_typeInfo == typeid(T);
}

template<typename T>
inline typename const std::enable_if<!std::is_void<T>::value, T>::type& ctObject::As() const
{
  return *(T*)m_data.data();
}

template<typename T>
inline typename std::enable_if<std::is_void<T>::value, T>::type ctObject::As() const {}

template<typename T>
inline typename std::enable_if<!std::is_void<T>::value, T>::type &ctObject::As()
{
  return *(T *)m_data.data();
}

template<typename T>
inline typename std::enable_if<std::is_void<T>::value, T>::type ctObject::As() {}

template<typename T>
inline T ctObject::AsOr(const T &defVal) const
{
  if (!Is<T>())
    return defVal;
  return As<T>();
}

template<typename T>
inline ctObject& ctObject::operator=(const T &val)
{
  Assign(val);
  return *this;
}

template<typename T>
inline ctObject& ctObject::operator=(T &&val)
{
  Assign(val);
  return *this;
}

template<typename T>
inline bool ctObject::operator==(const T &val) const
{
  return Is<T>() && (As<T>() == val);
}

template<typename T>
inline bool ctObject::operator!=(const T &val) const
{
  return !(*this == val);
}

template<typename T>
inline ctObject::ctObject(const T &val)
  : ctObject()
{
  Assign(val);
}

template<typename T>
inline ctObject::ctObject(T &&val)
  : ctObject()
{
  Assign(std::forward<T>(val));
}

template<typename T>
inline void ctObject::Assign(const T &value)
{
  SetType<T>();
  m_copyFunc(&m_data, &value);
}

template<typename T>
inline void ctObject::Assign(T &&value)
{
  SetType<T>();
  m_moveFunc(&m_data, &value);
}

template<typename T>
inline void ctObject::SetMember(const ctString &name, const T &value)
{
  SetMember(name, ctObject(value));
}

template<typename T>
inline void ctObject::SetMember(const ctString &name, T &&value)
{
  SetMember(name, ctObject(std::move(value)));
}

inline void ctObject::SetType(const ctObject &obj)
{
  if (obj.m_typeInfo == m_typeInfo)
    return;

  Destroy();
  m_typeInfo = obj.m_typeInfo;
  m_moveFunc = obj.m_moveFunc;
  m_copyFunc = obj.m_copyFunc;
  m_destructFunc = obj.m_destructFunc;
  m_data.reserve(obj.m_data.size());
}

template<typename T>
inline void __ctObjectDestructFunc(ctVector<uint8_t> *pMem)
{
  T* pObj = (T*)pMem->data();
  pObj->~T();
  pMem->clear();
}

template<typename T>
inline void __ctObjectCopyFunc(ctVector<uint8_t> *pMem, const void *pData)
{
  const T* pSrc = (const T*)pData;
  T* pDst = (T*)pMem->data();
  new (pDst)T(*pSrc);
}

template<typename T>
inline void __ctObjectMoveFunc(ctVector<uint8_t> *pMem, void *pData)
{
  T* pSrc = (T*)pData;
  T* pDst = (T*)pMem->data();
  new (pDst)T(std::move(*pSrc));
}

template<typename T>
T ctObject::GetMemberOr(const ctString &name, const T &defVal) const
{
  if (!HasMember(name))
    return defVal;
  return GetMember(name).AsOr<T>(defVal);
}

template<typename T>
inline void ctObjectWriteFunc(ctWriteStream *pStream, ctVector<uint8_t> *pData) { return ctStreamWrite(pStream, (T*)pData->data(), 1); }

template<typename T>
inline void ctObjectReadFunc(ctReadStream *pStream, ctVector<uint8_t> *pData) { return ctStreamRead(pStream, (T*)pData->data(), 1); }
template<typename T>
inline void ctObject::SetType()
{
  if (Is<T>())
    return;

  Destroy();
  m_destructFunc = __atObjectDestructFunc<std::decay_t<T>>;
  m_copyFunc = __atObjectCopyFunc<std::decay_t<T>>;
  m_moveFunc = __atObjectMoveFunc<std::decay_t<T>>;
  m_typeInfo = typeid(T);
  m_data.resize(sizeof(T));
}
