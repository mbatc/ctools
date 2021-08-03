
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

#ifndef _atMemory_h__
#define _atMemory_h__

#include "ctAlloc.h"
#include <type_traits>
#include <forward_list>
#include <memory>

//*********************
// Function Definitions

#define ctNew(...) new (_ctAllocTrace(sizeof(__VA_ARGS__), __LINE__, __FILE__, "")) __VA_ARGS__
#define ctNewArray(count, ...) new (_ctAllocTrace(sizeof(__VA_ARGS__) * count, __LINE__, __FILE__, "")) __VA_ARGS__[count]
#define ctDelete(pBlock) (delete pBlock, ctReleaseMemRef((void*)pBlock))

template<typename T, typename... Args>
inline void ctConstruct(T *pDst, Args&&... args)
{
  new (static_cast<void *>(&*pDst)) T(std::forward<Args>(args)...);
}

template<typename T>
inline void ctDestruct(T *pDst)
{
  pDst->~T();
}

// Fill an array with a value
template<typename T>
inline void ctUninitializedFillArray(T *pDst, int64_t count, const T &value)
{
  if (std::is_trivially_constructible<T>::value)
  {
    if (sizeof(T) == 1)
    {
      memset(pDst, *(uint8_t *)(&value), sizeof(T) * count);
    }
    else
    {
      for (; count--; ++pDst)
        memcpy(pDst, &value, sizeof(T));
    }
  }
  else
  {
    for (; count--; ++pDst)
      new (static_cast<void *>(&*pDst)) T(value);
  }
}

// Move construct [count] items from the source array to the destination array
template<typename T>
inline void ctUninitializedMoveArray(T *pDst, T *pSrc, int64_t count)
{
  if (std::is_trivially_move_constructible<T>::value)
  {
    memcpy(pDst, pSrc, sizeof(T) * count);
  }
  else
  {
    for (; count--; ++pDst, ++pSrc)
      new (static_cast<void *>(&*pDst)) T(std::move(*pSrc));
  }
}

// Copy construct [count] items from the source array to the destination array
template<typename T>
inline void ctUninitializedCopyArray(T *pDst, const T *pSrc, int64_t count)
{
  if (std::is_trivially_copy_constructible<T>::value)
  {
    memcpy(pDst, pSrc, count * sizeof(T));
  }
  else
  {
    for (; count--; ++pDst, ++pSrc)
      new (static_cast<void *>(&*pDst)) T(*pSrc);
  }
}

// Copy construct [count] items from the source array to the destination array
template<typename T, typename T2>
inline void ctUninitializedCopyArray(T *pDst, const T2 *pSrc, int64_t count)
{
  for (; count--; ++pDst, ++pSrc)
    new (static_cast<void *>(&*pDst)) T(*pSrc);
}

// Destruct [count] items in the destination array
template<typename T>
inline void ctDestructArray(T *pDst, int64_t count)
{
  if (std::is_scalar<T>::value)
    return;

  for (; count--; ++pDst)
    pDst->~T();
}

#endif
