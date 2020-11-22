
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

#ifndef atLimits_h__
#define atLimits_h__

#include "ctTypes.h"

template<typename T> T ctLimitsMax() { return{ 0 }; }

template<> double ctLimitsMax<double>();
template<> float ctLimitsMax<float>();
template<> int8_t ctLimitsMax<int8_t>();
template<> int16_t ctLimitsMax<int16_t>();
template<> int32_t ctLimitsMax<int32_t>();
template<> int64_t ctLimitsMax<int64_t>();
template<> uint8_t ctLimitsMax<uint8_t>();
template<> uint16_t ctLimitsMax<uint16_t>();
template<> uint32_t ctLimitsMax<uint32_t>();
template<> uint64_t ctLimitsMax<uint64_t>();

template<typename T> T ctLimitsMin() { return{ 0 }; }

template<> double ctLimitsMin<double>();
template<> float ctLimitsMin<float>();
template<> int8_t ctLimitsMin<int8_t>();
template<> int16_t ctLimitsMin<int16_t>();
template<> int32_t ctLimitsMin<int32_t>();
template<> int64_t ctLimitsMin<int64_t>();
template<> uint8_t ctLimitsMin<uint8_t>();
template<> uint16_t ctLimitsMin<uint16_t>();
template<> uint32_t ctLimitsMin<uint32_t>();
template<> uint64_t ctLimitsMin<uint64_t>();

template<typename T> T ctLimitsSmallest() { return T(0); }

template<> double ctLimitsSmallest<double>();
template<> float ctLimitsSmallest<float>();

#endif // atLimits_h__
