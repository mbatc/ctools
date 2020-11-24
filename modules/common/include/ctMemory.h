
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

//*********************
// Function Definitions

#define ctNew(...) new (_ctAllocTrace(sizeof(__VA_ARGS__), __LINE__, __FILE__, "")) __VA_ARGS__
#define ctNewArray(count, ...) new (_ctAllocTrace(sizeof(__VA_ARGS__) * count, __LINE__, __FILE__, "")) __VA_ARGS__[count]
#define ctDelete(pBlock) (delete pBlock, ctReleaseMemRef((void*)pBlock))

// Will Construct/Destruct object of they are non primitive types
template<typename T, typename... Args> void ctConstruct(T *pVal, Args&&... args);
template<typename T, typename... Args> void ctConstructArray(T *pVal, const int64_t count, Args&&... args);

template<typename T> typename std::enable_if<std::is_destructible<T>::value>::type ctDestruct(T *pVal);
template<typename T> typename std::enable_if<!std::is_destructible<T>::value>::type ctDestruct(T *pVal) { ctUnused(pVal); }
template<typename T> typename std::enable_if<std::is_destructible<T>::value>::type ctDestructArray(T *pVal, const int64_t count);
template<typename T> typename std::enable_if<!std::is_destructible<T>::value>::type ctDestructArray(T *pVal, const int64_t count) { ctUnused(pVal, count); }

//*************************
// Function Implementations

template<typename T> typename std::enable_if<std::is_destructible<T>::value>::type ctDestructArray(T *pVal, int64_t count)
{
  if (!std::is_trivially_destructible<T>::value)
    while (count-- > 0)
      ctDestruct(pVal++);
}

template<typename T, typename... Args> void ctConstructArray(T *pVal, int64_t count, Args&&... args)
{
  if (std::is_trivially_constructible<T>::value)
  {
    if (count > 1)
      new (pVal) T[count];
    else
      ctConstruct(pVal, std::forward<Args>(args)...);
  }
  else
  {
    T value(std::forward<Args>(args)...);
    while (count-- > 0)
      memcpy(pVal, &value, count * sizeof(T));
  }
}

template<typename T> inline void ctConstructArray(T *pVal, int64_t count)
{
  if (std::is_integral<T>::value)
    return;
  while (count-- > 0)
    ctConstruct(pVal++);
}

template<typename T> inline typename std::enable_if<std::is_destructible<T>::value>::type ctDestruct(T *pVal) { pVal->~T(); }

template<typename T, typename... Args> inline void ctConstruct(T *pVal, Args&&... args) { new (pVal) T(std::forward<Args>(args)...); }

#endif
