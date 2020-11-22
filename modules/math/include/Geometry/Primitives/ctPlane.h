
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

#ifndef atPlane_h__
#define atPlane_h__

#include "../../LinearAlgebra/ctVector4.h"
#include "ctRay.h"

template<typename T> class ctPlane
{
public:
  ctPlane(const ctVector4<T> &coeff);
  ctPlane(const ctVector3<T> &normal, const ctVector3<T> &point);
  ctPlane(const ctVector3<T> &a, const ctVector3<T> &b, const ctVector3<T> &c);

  ctVector3<T> Project(const ctVector3<T> &point);

  ctVector4<T> m_coeffs;
  ctVector4<T> m_point;
};

typedef ctPlane<int32_t> ctPlaneI;
typedef ctPlane<int64_t> ctPlaneI64;
typedef ctPlane<float> ctPlaneF;
typedef ctPlane<double> ctPlaneD;

#include "ctPlane.inl"
#endif // atPlane_h__
