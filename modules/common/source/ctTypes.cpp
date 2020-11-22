
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

#include "ctTypes.h"

template<> int64_t ctSize(const ctType &val)
{
  switch (val)
  {
  case ctType_Float32: return 4;
  case ctType_Float64: return 8;
  case ctType_Int8: return 1;
  case ctType_Int16: return 2;
  case ctType_Int32: return 4;
  case ctType_Int64: return 8;
  case ctType_Uint8: return 1;
  case ctType_Uint16: return 2;
  case ctType_Uint32: return 4;
  case ctType_Uint64: return 8;
  default: return 0;
  }
}

template<> ctType ctGetType<bool>() { return ctType_Uint8; }
template<> ctType ctGetType<char>() { return ctType_Uint8; }
template<> ctType ctGetType<wchar_t>() { return ctType_Uint16; }

template<> ctType ctGetType<int8_t>() { return ctType_Int8; }
template<> ctType ctGetType<int16_t>() { return ctType_Int16; }
template<> ctType ctGetType<int32_t>() { return ctType_Int32; }
template<> ctType ctGetType<int64_t>() { return ctType_Int64; }
template<> ctType ctGetType<uint8_t>() { return ctType_Uint8; }
template<> ctType ctGetType<uint16_t>() { return ctType_Uint16; }
template<> ctType ctGetType<uint32_t>() { return ctType_Uint32; }
template<> ctType ctGetType<uint64_t>() { return ctType_Uint64; }

template<> ctType ctGetType<float>() { return ctType_Float32; }
template<> ctType ctGetType<double>() { return ctType_Float64; }

bool ctTypeDesc::operator==(const ctTypeDesc &o) const { return type == o.type && size == o.size && count == o.count; }
bool ctTypeDesc::operator!=(const ctTypeDesc &o) const { return !(o == *this); }

bool ctTypeCast(void *pDst, const void *pSrc, const ctTypeDesc &dstType, const ctTypeDesc &srcType)
{
  int64_t dstCount = dstType.width * dstType.count;
  int64_t srcCount = srcType.width * srcType.count;
  int64_t count = dstCount < srcCount ? dstCount : srcCount;

  if (dstType.type == srcType.type)
  {
    memcpy(pDst, pSrc, count * srcType.size);
    return true;
  }

  switch (srcType.type)
  {
  case ctType_Float16: return false;
  case ctType_Float32: return __ctTypeCast(pDst, (float*)pSrc, dstType.type, count);
  case ctType_Float64: return __ctTypeCast(pDst, (double*)pSrc, dstType.type, count);
  case ctType_Int8: return __ctTypeCast(pDst, (int8_t*)pSrc, dstType.type, count);
  case ctType_Int16: return __ctTypeCast(pDst, (int16_t*)pSrc, dstType.type, count);
  case ctType_Int32: return __ctTypeCast(pDst, (int32_t*)pSrc, dstType.type, count);
  case ctType_Int64: return __ctTypeCast(pDst, (int64_t*)pSrc, dstType.type, count);
  case ctType_Uint8: return __ctTypeCast(pDst, (uint8_t*)pSrc, dstType.type, count);
  case ctType_Uint16: return __ctTypeCast(pDst, (uint16_t*)pSrc, dstType.type, count);
  case ctType_Uint32: return __ctTypeCast(pDst, (uint32_t*)pSrc, dstType.type, count);
  case ctType_Uint64: return __ctTypeCast(pDst, (uint64_t*)pSrc, dstType.type, count);
  }
  return false;
}
