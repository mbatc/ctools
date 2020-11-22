
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

template<typename T> ctPlane<T>::ctPlane(const ctVector4<T> &coeff) { m_coeffs = coeff; m_point = ctVector4<T>(0, 0, m_coeffs.w); }
template<typename T> ctPlane<T>::ctPlane(const ctVector3<T> &a, const ctVector3<T> &b, const ctVector3<T> &c) : ctPlane<T>((b - a).Cross(c - a), a) {}
template<typename T> ctPlane<T>::ctPlane(const ctVector3<T> &normal, const ctVector3<T>&point) { m_coeffs = { normal, normal.x * point.x + normal.y * point.y + normal.z * point.z }; }

template<typename T> inline ctVector3<T> ctPlane<T>::Project(const ctVector3<T> &point)
{
  double time = 0;
  ctRay<T> ray(point, m_coeffs.xyz());
  if (ctIntersects(ray, *this, &time))
    return ray.At(time);
  return point;
}
