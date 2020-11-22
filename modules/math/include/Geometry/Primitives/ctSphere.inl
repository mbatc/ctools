
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

template<typename T> inline T ctSphere<T>::Diameter() const { return m_radius * 2; }

template<typename T> inline ctSphere<T>::ctSphere(const T &_radius, const ctVector3<T> &_position)
  : m_position(_position)
  , m_radius(_radius)
{}

template<typename T> inline T ctSphere<T>::Volume() const
{
  return T((4.0 / 3.0) * ctPi * m_radius * m_radius * m_radius);
}

template<typename T> inline T ctSphere<T>::SurfaceArea() const
{
  return T(4 * atPi * m_radius * m_radius);
}

template<typename T> inline void ctSphere<T>::GrowToContain(const ctVector3<T> &point)
{
  m_radius = ctMax(m_radius, (point - m_position).Mag());
}

template<typename T> inline bool ctSphere<T>::Contains(const ctVector3<T> &point) const
{
  return (m_position - point).Length() < m_radius * m_radius;
}

template<typename T> inline bool ctSphere<T>::Distance(const ctVector3<T> &point) const
{
  return ctSqrt(Distance2(point));
}

template<typename T> inline bool ctSphere<T>::Distance2(const ctVector3<T> &point) const
{
  return (m_position - point).Length();
}

template<typename T> inline ctVector3<T> ctSphere<T>::ClosestPoint(const ctVector3<T> &point) const
{
  ctVector3<T> fromCenter = point - m_pos;
  T dist = fromCenter.Mag();
  return ctMin(dist, m_radius) * fromCenter.Normalize();
}

template<typename T> inline ctVector3<T> ctSphere<T>::ClosestPointBounds(const ctVector3<T> &point) const
{
  return (point - m_position).Normalize() * m_radius + m_position;
}
