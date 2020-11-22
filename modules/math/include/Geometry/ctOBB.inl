#include "ctOBB.h"

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

template<typename T> inline ctOBB<T>::ctOBB()
  : m_aabb()
  , m_orientation(ctQuaternion<T>::Identity())
{}

template<typename T> inline ctOBB<T>::ctOBB(const ctAABB<T> &aabb, const atQuaternion<T> &orientation)
  : m_aabb(aabb)
  , m_orientation(orientation)
{}

template<typename T> inline ctOBB<T>::ctOBB(const ctAABB<T> &aabb, const ctMatrix4<T> &rotation)
  : m_aabb(aabb)
  , m_orientation(rotation)
{}

template<typename T> inline ctOBB<T>::ctOBB(const ctAABB<T> &aabb, const ctVector3<T> &rotation)
  : m_aabb(aabb)
  , m_orientation(rotation)
{}

template<typename T> inline ctOBB<T>::ctOBB(const ctVector3<T> &min, const ctVector3<T> &max, const atQuaternion<T> &orientation)
  : m_aabb(min, max)
  , m_orientation(orientation)
{}

template<typename T> inline ctOBB<T>::ctOBB(const ctVector3<T> &min, const ctVector3<T> &max, const ctMatrix4<T> &rotation)
  : m_aabb(min, max)
  , m_orientation(rotation)
{}

template<typename T> inline ctOBB<T>::ctOBB(const ctVector3<T> &min, const ctVector3<T> &max, const ctVector3<T> &rotation)
  : m_aabb(min, max)
  , m_orientation(rotation.x, rotation.y, rotation.z)
{}

template<typename T> inline typename ctOBB<T>::Vec3 ctOBB<T>::ClosestPoint(const Vec3 &point) const
{
  return OBBToWorld(m_aabb.ClosestPoint(WorldToOBB(point)));
}

template<typename T> inline typename ctOBB<T>::Vec3 ctOBB<T>::ClosestPointBounds(const Vec3 &point) const
{
  return OBBToWorld(m_aabb.ClosestPointBounds(WorldToOBB(point)));
}

template<typename T> inline typename ctOBB<T>::Vec3 ctOBB<T>::ClosestFaceNormal(const Vec3 &point) const
{
  return m_orientation.Rotate(m_aabb.GetClosestFaceNormal(WorldToOBB(point)));
}

template<typename T> inline bool ctOBB<T>::Contains(const Vec3 &point) const
{
  return m_aabb.Contains(WorldToOBB(point));
}

template<typename T> inline typename ctOBB<T>::Vec3 ctOBB<T>::OBBToWorld(const Vec3 &point) const
{
  ctVector3<T> pos = Center();
  return m_orientation.Rotate(point - pos) + pos;
}

template<typename T> inline typename ctOBB<T>::Vec3 ctOBB<T>::WorldToOBB(const Vec3 &point) const
{
  ctVector3<T> pos = Center();
  return m_orientation.Inverse().Rotate(point - pos) + pos;
}

template<typename T> ctAABB<T> ctBounds(const ctOBB<T> &obb)
{
  ctVector3<T> dims = obb.Dimensions();
  ctAABB<T> aabb = ctAABB<T>();
  ctVector3<T> corner;

  for (int64_t i = 0; i < 8; ++i)
  {
    corner = obb.m_aabb.m_min + dims * ctVector3<T>((i & 1) == 0, (i & 2) == 0, (i & 4) == 0);
    aabb.GrowToContain(obb.OBBToWorld(corner));
  }

  return aabb;
}

template<typename T> inline typename ctOBB<T>::Vec3 ctOBB<T>::Center() const { return m_aabb.Center(); }

template<typename T> inline typename ctOBB<T>::Vec3 ctOBB<T>::Dimensions() const { return m_aabb.Dimensions(); }
