#include "ctRect.h"

template<typename T>
template<typename T2>
inline ctRect<T>::ctRect(const ctRect<T2> &o)
  : m_min(o.m_min)
  , m_max(o.m_max)
{}

template<typename T> inline ctRect<T>::ctRect(const T &left, const T &top, const T &right, const T &bottom)
  : m_min(left, top)
  , m_max(right, bottom)
{}

template<typename T> inline ctRect<T>::ctRect(const ctVector4<T> &rect)
  : m_min(rect.xy())
  , m_max(rect.zw())
{}

template<typename T>
inline ctRect<T> ctRect<T>::Largest()
{
    return ctRect<T>(ctLimitsMin<T>(), ctLimitsMax<T>());
}

template<typename T>
inline ctRect<T> ctRect<T>::Smallest()
{
  return ctRect<T>(ctLimitsMax<T>(), ctLimitsMin<T>());
}

template<typename T> inline ctRect<T>::ctRect(const ctVector2<T> &min, const ctVector2<T> &max)
  : m_min(min)
  , m_max(max)
{}

template<typename T> inline T ctRect<T>::Area() const
{
  ctVector2<T> dims = Dimensions();
  return dims.x * dims.y;
}

template<typename T> inline ctVector2<T> ctRect<T>::Dimensions() const { return m_max - m_min; }
template<typename T> inline ctVector4<T> ctRect<T>::ToVector4() const { return ctVector4<T>(m_min, m_max); }

template<typename T> inline T ctRect<T>::Width() const { return m_max.x - m_min.x; }
template<typename T> inline T ctRect<T>::Height() const { return m_max.y - m_min.y; }

template<typename T> inline bool ctRect<T>::Contains(const ctRect &rect) { return Contains(rect.m_min) && Contains(rect.m_max); }
template<typename T> inline bool ctRect<T>::Contains(const ctVector2<T> &point) { return point.Clamp(m_min, m_max) == point; }

template<typename T> inline void ctRect<T>::GrowToContain(const ctRect<T> &point)
{
  GrowToContain(point.m_min);
  GrowToContain(point.m_max);
}

template<typename T> inline void ctRect<T>::GrowToContain(const ctVector2<T> &point)
{
  m_min = m_min.Min(point);
  m_max = m_max.Max(point);
}

template<typename T> inline bool ctRect<T>::operator==(const ctRect<T> &rhs) const { return m_max == rhs.m_max && m_min == rhs.m_min; }
template<typename T> inline bool ctRect<T>::operator!=(const ctRect<T> &rhs) const { return !(*this == rhs); }
