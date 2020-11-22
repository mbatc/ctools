
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

#ifndef atTriangle_h__
#define atTriangle_h__

#include "ctPlane.h"
#include "../ctAABB.h"

template<typename T> class ctTriangle
{
public:
  ctTriangle();
  ctTriangle(const ctVector3<T> &a, const ctVector3<T> &b, const ctVector3<T> &c);

  // Returns the area of the triangle
  T Area() const;

  // Returns the point at the center of the triangle
  ctVector3<T> Center() const;

  // Returns the normalized cross product of the edges ab and ac
  ctVector3<T> Normal() const;

  // Returns the cross product of the edges ab and ac
  ctVector3<T> Normal2() const;
  
  ctVector3<T> BarycentricCoords(const ctVector3<T> &point) const;

  ctVector3<T> m_a;
  ctVector3<T> m_b;
  ctVector3<T> m_c;
};

template<typename T> ctAABB<T> ctBounds(const ctTriangle<T> &tri);

typedef ctTriangle<int32_t> ctTriangleI;
typedef ctTriangle<int64_t> ctTriangleI64;
typedef ctTriangle<float> ctTriangleF;
typedef ctTriangle<double> ctTriangleD;

#include "ctTriangle.inl"
#endif // atTriangle_h__
