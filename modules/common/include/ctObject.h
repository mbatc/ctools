
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

#ifndef atObject_h__
#define atObject_h__

#include <typeindex>
#include <type_traits>
#include "ctHashMap.h"
#include "ctString.h"

class ctObject
{
public:
  ctObject();
  ctObject(const ctObject &copy);
  ctObject(ctObject &&move);
  ~ctObject();

  void Assign(const ctObject &value);
  void Assign(ctObject &&value);
  void SetMember(const ctString &name, const ctObject &value);
  void SetMember(const ctString &name, ctObject &&value);
  void Destroy();

  bool HasMember(const ctString &name) const;

  ctObject& GetMember(const ctString &name);
  const ctObject& GetMember(const ctString &name) const;

  ctObject& operator=(const ctObject &rhs);
  ctObject& operator=(ctObject &&rhs);

  ctObject& operator[](const ctString &name);
  const ctObject& operator[](const ctString &name) const;

  template<typename T>
  bool Is() const;

  template<typename T>
  typename std::enable_if<!std::is_void<T>::value, T>::type &As();

  template<typename T>
  typename std::enable_if<std::is_void<T>::value, T>::type As();

  template<typename T>
  const typename std::enable_if<!std::is_void<T>::value, T>::type& As() const;

  template<typename T>
  typename std::enable_if<std::is_void<T>::value, T>::type As() const;

  template<typename T>
  T AsOr(const T &defVal) const;

  template<typename T>
  ctObject& operator=(const T &val);

  template<typename T>
  ctObject& operator=(T &&val);

  template<typename T>
  bool operator==(const T &val) const;

  template<typename T>
  bool operator!=(const T &val) const;

  template<typename T>
  ctObject(const T &val);

  template<typename T>
  ctObject(T &&val);

  template<typename T>
  void Assign(const T &value);

  template<typename T>
  void Assign(T &&value);

  template<typename T>
  void SetMember(const ctString &name, const T &value);

  template<typename T>
  void SetMember(const ctString &name, T &&value);

  template<typename T>
  T& GetMember(const ctString &name);

  template<typename T>
  const T& GetMember(const ctString &name) const;

  template<typename T> T GetMemberOr(const ctString &name, const T &defVal) const;

  bool Empty() const;

  ctString Typename() const;

  ctVector<ctString> GetMemberNames() const;

  friend int64_t ctStreamRead(ctReadStream *pStream, ctObject *pData, const int64_t count);
  friend int64_t ctStreamWrite(ctWriteStream *pStream, ctObject *pData, const int64_t count);

protected:
  void SetType(const ctObject &obj);
  
  template<typename T>
  void SetType();

  std::type_index m_typeInfo;
  ctVector<uint8_t> m_data;
  ctHashMap<ctString, ctObject> m_members;

  void(*m_copyFunc) (ctVector<uint8_t> *pMem, const void *) = nullptr;
  void(*m_moveFunc) (ctVector<uint8_t> *pMem, void *) = nullptr;
  void(*m_destructFunc) (ctVector<uint8_t> *pMem) = nullptr;
  int64_t (*m_writeFunc) (ctWriteStream *pStream, const ctVector<uint8_t> *pSrc);
  int64_t (*m_readFunc) (ctReadStream *pStream, ctVector<uint8_t> *pDst);
};

template<typename T> void __ctObjectDestructFunc(ctVector<uint8_t> *pMem);
template<typename T> void __ctObjectCopyFunc(ctVector<uint8_t> *pMem, const void *pData);
template<typename T> void __ctObjectMoveFunc(ctVector<uint8_t> *pMem, void *pData);
template<typename T> int64_t ctObjectWriteFunc(ctWriteStream *pStream, ctVector<uint8_t> *pData);
template<typename T> int64_t ctObjectReadFunc(ctReadStream *pRead, ctVector<uint8_t> *pData);

#include "ctObject.inl"
#endif // atObject_h__
