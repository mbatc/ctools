#ifndef ctRect_h__
#define ctRect_h__

#include "ctVector4.h"
#include "ctIntersects.h"

template<typename T> class ctRect
{
public:
  template<typename T2 = T> ctRect(const ctRect<T2> &o);

  ctRect(const ctVector2<T> &min = atLimitsMax<T>(), const ctVector2<T> &max = atLimitsMin<T>());
  ctRect(const T &left, const T &top, const T &right, const T &bottom);
  ctRect(const ctVector4<T> &rect);

  static ctRect<T> Largest();
  static ctRect<T> Smallest();

  T Area() const;
  ctVector2<T> Dimensions() const;
  ctVector4<T> ToVector4() const;

  T Width() const;
  T Height() const;

  bool Contains(const ctRect &rect);
  bool Contains(const ctVector2<T> &point);
  void GrowToContain(const ctRect &point);
  void GrowToContain(const ctVector2<T> &point);

  bool operator==(const ctRect<T> &rhs) const;
  bool operator!=(const ctRect<T> &rhs) const;

  ctVector2<T> m_min;
  ctVector2<T> m_max;
};

typedef ctRect<int32_t> atRectI;
typedef ctRect<int64_t> atRectI64;
typedef ctRect<float> atRectF;
typedef ctRect<double> atRectD;

#include "ctRect.inl"
#endif // ctRect_h__
