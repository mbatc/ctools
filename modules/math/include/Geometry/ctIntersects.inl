
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

template<typename T, typename T2> bool ctIntersects(const ctRay<T2> &ray, const ctRay<T> &ray2, T2 *pTime)
{
  ctVector3<T> point;
  if (!ray2.ClosestPoint(ray, &point, pTime))
    return false;
  return ray.IsOnRay(point);
}

template<typename T, typename T2> bool ctIntersects(const ctRay<T2> &ray, const ctBVHNode<T> &bvhNode, T2 *pTime)
{
  bool res = false;
  T2 minDist = 0xFFFFFFFF;
  
  if (!ctIntersects(ray, bvhNode.bounds))
    return false;

  for (const ctBVHNode<T> &node : bvhNode.children)
  {
    T2 dist = 0xFFFFFFFF;
    bool hit = node.isLeaf ? ctIntersects(ray, node.primitive, &dist) : ctIntersects(ray, node, &dist);

    if (hit && dist < 0)
      printf("%f", minDist);

    if (hit && dist < minDist && dist >= 0)
    {
      minDist = dist;
      res = true;
    }
  }
  if (pTime) 
    *pTime = minDist;
  return res;
}

template<typename T> bool ctIntersects(const ctAABB<T> &boxA, const ctAABB<T> &boxB)
{
  if (boxA.m_max.x < boxB.m_min.x) return false;
  if (boxA.m_max.y < boxB.m_min.y) return false;
  if (boxA.m_max.z < boxB.m_min.z) return false;
  if (boxA.m_min.x > boxB.m_max.x) return false;
  if (boxA.m_min.y > boxB.m_max.y) return false;
  if (boxA.m_min.z > boxB.m_max.z) return false;
  return true;
}

template<typename T> inline bool ctIntersects(const ctSphere<T> &sphere, const ctSphere<T> &sphere2, ctVector3<T> *pPoint)
{
  ctVector3<T> aToB = sphere2.m_position - sphere.m_position;
  T distToB = aToB.Length();
  T maxDist = atSquare(sphere.m_radius + sphere2.m_radius);
  bool hit = distToB < maxDist;
  if (hit && pPoint)
  {
    distToB = atSqrt(distToB);
    maxDist = sphere.m_radius + sphere2.m_radius;
    T overlapDist = (maxDist - distToB) / 2;
    *pPoint = atLerp(sphere.m_position, sphere2.m_position, (sphere.m_radius + overlapDist) / maxDist);
  }

  return hit;
}

template<typename T> bool ctIntersects(const ctRect<T> &rect, const ctRect<T> &rect2)
{
  if (rect.m_max.x < rect2.m_min.x) return false;
  if (rect.m_max.y < rect2.m_min.y) return false;
  if (rect.m_min.x > rect2.m_max.x) return false;
  if (rect.m_min.y > rect2.m_max.y) return false;
  return true;
}

template<typename T> inline bool ctIntersects(const ctAABB<T> &box, const ctSphere<T> &sphere)
{
  return sphere.Contains(box.ClosestPoint(sphere.m_position));
}

template<typename T> inline bool ctIntersects(const ctOBB<T> &obb, const ctSphere<T> &sphere)
{
  return ctIntersects(obb.m_aabb, sphere);
}

template<typename T> inline bool ctIntersects(const ctAABB<T> &aabb, const ctOBB<T> &obb)
{
  return ctIntersects(ctOBB<T>(aabb.m_min, aabb.m_max), obb);
}

template<typename T> inline bool ctIntersects(const ctOBB<T> &obb, const ctOBB<T> &obb2)
{
  ctVector3<T> obbCenter = obb.Center();
  ctVector3<T> obb2Center = obb2.Center();

  { // Uses separating axis theorem to test the boxes are intersecting
    ctOBB<T> obb2To1 = obb2;
    atQuaternion<T> invRot = obb.m_orientation.Inverse();
    ctVector3<T> toCenter = obb2Center - obbCenter;
    toCenter = invRot.Rotate(toCenter) - toCenter;
    obb2To1.m_orientation *= invRot;
    obb2To1.m_aabb.m_max += toCenter;
    obb2To1.m_aabb.m_min += toCenter;
    if (!ctIntersects(obb.m_aabb, ctBounds(obb2To1)))
      return false;
  }

  {
    ctOBB<T> obb1To2 = obb;
    atQuaternion<T> invRot = obb2.m_orientation.Inverse();
    ctVector3<T> toCenter = obbCenter - obb2Center;
    toCenter = invRot.Rotate(toCenter) - toCenter;
    obb1To2.m_orientation *= invRot;
    obb1To2.m_aabb.m_max += toCenter;
    obb1To2.m_aabb.m_min += toCenter;
    if (!ctIntersects(obb2.m_aabb, ctBounds(obb1To2)))
      return false;
  }

  return true;
}

template<typename T> bool ctIntersects(const ctRay<T> &ray, const ctAABB<T> &box, T *pTime)
{
  ctVector3<T> invDir = (T)1 / ray.m_dir;
  ctVector3<T> tMin3 = (box.m_min - ray.m_pos) * invDir;
  ctVector3<T> tMax3 = (box.m_max - ray.m_pos) * invDir;
  T tmin = ctMax(ctMax(ctMin(tMin3.x, tMax3.x), ctMin(tMin3.y, tMax3.y)), ctMin(tMin3.z, tMax3.z));
  T tmax = ctMin(ctMin(ctMax(tMin3.x, tMax3.x), ctMax(tMin3.y, tMax3.y)), ctMax(tMin3.z, tMax3.z));
  bool intersects = !(tmax < 0 || tmin > tmax);
  if (pTime) 
    *pTime = intersects ? tmin : tmax;
  return intersects;
}

template<typename T> inline bool ctIntersects(const ctRay<T> &ray, const ctOBB<T> &obb, T *pTime)
{
  ctVector3<T> rotatedPos = obb.WorldToOBB(ray.m_pos);
  ctVector3<T> rotatedDir = obb.m_orientation.Inverse().Rotate(ray.m_dir);
  ctRay<T> rotatedRay(rotatedPos, rotatedDir);
  return ctIntersects(rotatedRay, obb.m_aabb, pTime);
}

template<typename T> bool ctIntersects(const ctRay<T> &ray, const ctTriangle<T> &tri, T *pTime)
{
  T time = 0;
  
  if (!ctIntersects(ray, ctPlane<T>(tri.m_a, tri.m_b, tri.m_c), &time))
    return false;

  ctVector3<T> point = ray.At(time);
  ctVector3<T> weights = tri.BarycentricCoords(point);
  if (pTime)
    *pTime = time;

  return weights.x + weights.y + weights.z <= 1.0 && time >= 0;
}

template<typename T> bool ctIntersects(const ctRay<T> &ray, const ctPlane<T> &plane, T *pTime) 
{
  const ctVector3<T> &rayDir = ray.m_dir;
  const ctVector4<T> &coeffs = plane.m_coeffs;
  const ctVector3<T> &rayPos = ray.m_pos;

  if (coeffs.xyz().Dot(rayDir) == 0) return false;
  if (pTime) *pTime = (coeffs.w - rayPos.x * coeffs.x - rayPos.y * coeffs.y - rayPos.z * coeffs.z) / (coeffs.x * rayDir.x + coeffs.y * rayDir.y + coeffs.z * rayDir.z);
  return !pTime || *pTime >= 0;
}

template<typename T> inline bool ctIntersects(const ctRay<T> &ray, const ctSphere<T> &sphere, T *pTime)
{
  ctVector3<T> point;
  T time = 0;
  T dist = 0;
  ray.ClosestPoint(sphere.m_position, &point, &time, &dist);
  if (dist < sphere.m_radius)
  {
    if (pTime)
      *pTime = time;
    return true;
  }

  return false;
}

template<typename T> bool ctIntersects(const ctTriangle<T> &tri, const ctTriangle<T> &tri2, ctVector3<T> *pPoint)
{ 
  T time = 0;
  bool result = false;
  if (ctIntersects(ctRay<T>(tri.m_a, tri.m_b - tri.m_a), tri2, &time))
  {
    if (pPoint) 
      *pPoint = ctRay<T>(tri.m_a, tri.m_b - tri.m_a).At(time);
  }
  else if (ctIntersects(ctRay<T>(tri.m_a, tri.m_c - tri.m_a), tri2, &time))
  {
    if (pPoint)
      *pPoint = ctRay<T>(tri.m_a, tri.m_c - tri.m_a).At(time);
  }
  else if (ctIntersects(ctRay<T>(tri.m_b, tri.m_c - tri.m_b), tri2, &time))
  {
    if (pPoint)
      *pPoint = ctRay<T>(tri.m_b, tri.m_c - tri.m_b).At(time);
  }
  else
    return false;
  return true;
}

template<typename T> bool ctIntersects(const ctRay<T> &ray, const ctRay<T> &ray2, T *pTime) { return atIntersects<T, T>(ray, ray2, pTime); }
template<typename T, typename T2> bool ctIntersects(const ctRay<T2> &ray, const ctBVH<T> &bvh, T2 *pTime) { return ctIntersects(ray, bvh.m_root, pTime); }
