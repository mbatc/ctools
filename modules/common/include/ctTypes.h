
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

#ifndef _atTypes_h__
#define _atTypes_h__

#include <stdbool.h>
#include <stdint.h>
#include <stdarg.h>
#include <tuple>

#include "ctUtility.h"

#define CT_INVALID_ID (int64_t)-1
#define CT_INVALID_INDEX (int64_t)-1
#define CT_INVALID_LOC (int64_t)-1

#define ctUnused(...) __VA_ARGS__

enum ctType : int64_t
{
  ctType_Float16,
  ctType_Float32,
  ctType_Float64,

  ctType_Int8,
  ctType_Int16,
  ctType_Int32,
  ctType_Int64,

  ctType_Uint8,
  ctType_Uint16,
  ctType_Uint32,
  ctType_Uint64,

  ctType_Count,

  ctType_Unknown
};

// Return the size in bytes of a specific type
template<typename T> int64_t ctSize() { return sizeof(T); };
template<typename T> int64_t ctSize(const T& val) { return sizeof(val); }; 
template<> int64_t ctSize(const ctType &val);

struct ctTypeDesc
{
  ctTypeDesc(const ctType type = ctType_Unknown, const int64_t width = 1, const int64_t count = 1)
    : type(type)
    , count(count)
    , width(width)
    , size(ctSize(type))
  {}

  ctType type = ctType_Unknown;

  int64_t width = 1;
  int64_t count = 1;
  int64_t size = 1;

  bool operator==(const ctTypeDesc &o) const;
  bool operator!=(const ctTypeDesc &o) const;
};

// Return an enum representing the type
template<typename T> ctType ctGetType() { return ctType_Unknown; }
template<> ctType ctGetType<bool>();
template<> ctType ctGetType<char>();
template<> ctType ctGetType<wchar_t>();
template<> ctType ctGetType<int8_t>();
template<> ctType ctGetType<int16_t>();
template<> ctType ctGetType<int32_t>();
template<> ctType ctGetType<int64_t>();
template<> ctType ctGetType<uint8_t>();
template<> ctType ctGetType<uint16_t>();
template<> ctType ctGetType<uint32_t>();
template<> ctType ctGetType<uint64_t>();
template<> ctType ctGetType<float>();
template<> ctType ctGetType<double>();

// Return a struct containing the atType enum and count of that type
// Specialized Templates implemented in atMath.h for ctVector2/3/4 and atMatrix

template<typename T> ctTypeDesc ctGetTypeDesc() { return ctTypeDesc(ctGetType<T>()); }
template<typename T> ctTypeDesc ctGetTypeDesc(const T &unused) { ctUnused(unused); return ctGetTypeDesc<T>(); }

// Type case helpers
template<typename T1, typename T2> bool __ctTypeCast(T1 *pDst, const T2 *pSrc, const int64_t &count);
template<typename T> bool __ctTypeCast(T *pDst, const T *pSrc, const int64_t &count);
template<typename T> bool __ctTypeCast(void *pDst, const T *pSrc, const ctType &dstType, const int64_t &count);

// Cast types that can be expressed by an atTypeDesc
bool ctTypeCast(void *pDst, const void *pSrc, const ctTypeDesc &dstType, const ctTypeDesc &srcType);

#include "ctTypes.inl"

#endif
