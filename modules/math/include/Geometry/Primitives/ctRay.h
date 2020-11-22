
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

#ifndef ctRay_h__
#define ctRay_h__

#include "../../LinearAlgebra/ctVector3.h"
#include "../ctIntersects.h"

template<typename T> class ctRay
{
public:
  using Vec3 = ctVector3<T>;
  
  ctRay(const Vec3 &pos, const Vec3 &dir);

  Vec3 At(const T time) const;
  Vec3 GetTime(const Vec3 &point) const;
  bool IsOnRay(const Vec3 &point, T *pTime) const;
  bool ClosestPoint(const ctRay<T> &ray, Vec3 *pPoint, T *pTime = nullptr) const;
  bool ClosestPoint(const ctRay<T> &ray, Vec3 *pPoint, T* pTime, T* pDist) const;
  bool ClosestPoint(const Vec3 &point, Vec3 *pPoint, T *pTime = nullptr, T* pDist = nullptr) const;

  T TimeX(const T val) const;
  T TimeY(const T val) const;
  T TimeZ(const T val) const;

  Vec3 m_pos;
  Vec3 m_dir;
};

typedef ctRay<int32_t> ctRayI;
typedef ctRay<int64_t> ctRayI64;
typedef ctRay<float> ctRayF;
typedef ctRay<double> ctRayD;

#include "ctRay.inl"
#endif // ctRay_h__
