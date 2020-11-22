
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

#include "ctLimits.h"
#include <float.h>

template<> double ctLimitsMax<double>() { return DBL_MAX; }
template<> float ctLimitsMax<float>() { return FLT_MAX; }
template<> int8_t ctLimitsMax<int8_t>() { return UINT8_MAX; }
template<> int16_t ctLimitsMax<int16_t>() {return INT16_MAX;}
template<> int32_t ctLimitsMax<int32_t>() {return INT32_MAX;}
template<> int64_t ctLimitsMax<int64_t>() {return INT64_MAX;}
template<> uint8_t ctLimitsMax<uint8_t>() { return UINT8_MAX; }
template<> uint16_t ctLimitsMax<uint16_t>() { return UINT16_MAX; }
template<> uint32_t ctLimitsMax<uint32_t>() { return UINT32_MAX; }
template<> uint64_t ctLimitsMax<uint64_t>() { return UINT64_MAX; }

template<> double ctLimitsMin<double>() { return -DBL_MAX; }
template<> float ctLimitsMin<float>() { return -FLT_MAX; }
template<> int8_t ctLimitsMin<int8_t>() { return INT8_MIN; }
template<> int16_t ctLimitsMin<int16_t>() { return INT16_MIN; }
template<> int32_t ctLimitsMin<int32_t>() { return INT32_MIN; }
template<> int64_t ctLimitsMin<int64_t>() { return INT64_MIN; }
template<> uint8_t ctLimitsMin<uint8_t>() { return 0; }
template<> uint16_t ctLimitsMin<uint16_t>() { return 0; }
template<> uint32_t ctLimitsMin<uint32_t>() { return 0; }
template<> uint64_t ctLimitsMin<uint64_t>() { return 0; }

template<> double ctLimitsSmallest<double>() { return DBL_EPSILON; }
template<> float ctLimitsSmallest<float>() { return FLT_EPSILON; }
