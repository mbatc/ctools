
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

#ifndef atIntersects_h__
#define atIntersects_h__

#include "atMath.h"

template<typename T> class ctRay;
template<typename T> class ctBVH;
template<typename T> class ctOBB;
template<typename T> class ctAABB;
template<typename T> class ctRect;
template<typename T> class ctPlane;
template<typename T> class ctSphere;
template<typename T> class ctTriangle;
template<typename T> struct ctBVHNode;

// Geometry intersections
template<typename T> bool ctIntersects(const ctAABB<T> &aabb, const ctAABB<T> &aabb2);
template<typename T> bool ctIntersects(const ctSphere<T> &sphere, const ctSphere<T> &sphere2, ctVector3<T> *pPoint = nullptr);
template<typename T> bool ctIntersects(const ctTriangle<T> &tri, const ctTriangle<T> &tri2, ctVector3<T> *pPoint);
template<typename T> bool ctIntersects(const ctRect<T> &rect, const ctRect<T> &ray2);
template<typename T> bool ctIntersects(const ctAABB<T> &aabb, const ctSphere<T> &sphere);
template<typename T> bool ctIntersects(const ctOBB<T> &obb, const ctSphere<T> &sphere);
template<typename T> bool ctIntersects(const ctOBB<T> &obb, const ctOBB<T> &obb2);
template<typename T> bool ctIntersects(const ctAABB<T> &aabb, const ctOBB<T> &obb);

// Ray intersections
template<typename T> bool ctIntersects(const ctRay<T> &ray, const ctRay<T> &ray2, T *pTime);
template<typename T> bool ctIntersects(const ctRay<T> &ray, const ctPlane<T> &tri, T* pTime = nullptr);
template<typename T> bool ctIntersects(const ctRay<T> &ray, const ctSphere<T> &sphere, T* pTime = nullptr);
template<typename T> bool ctIntersects(const ctRay<T> &ray, const ctAABB<T> &box, T* pTime = nullptr);
template<typename T> bool ctIntersects(const ctRay<T> &ray, const ctOBB<T> &obb, T* pTime = nullptr);
template<typename T> bool ctIntersects(const ctRay<T> &ray, const ctTriangle<T> &tri, T* pTime = nullptr);
template<typename T, typename T2> bool ctIntersects(const ctRay<T2> &ray, const ctRay<T> &ray2, T2* pTime);
template<typename T, typename T2> bool ctIntersects(const ctRay<T2> &ray, const ctBVH<T> &bvh, T2* pTime);
template<typename T, typename T2> bool ctIntersects(const ctRay<T2> &ray, const ctBVHNode<T> &bvhNode, T2* pTime);

#include "ctIntersects.inl"
#endif // atIntersects_h__

