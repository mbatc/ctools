#ifndef tmlVector2_h__
#define tmlVector2_h__

#include "ctMathHelpers.h"

template<typename T> class ctVector2
{
public:
  enum { ElementCount = 2 };

  ctVector2();
  ctVector2(T val);
  ctVector2(T _x, T _y);
  ctVector2(const ctVector2<T> &copy);
  ctVector2(ctVector2<T> &&move);

  template<typename T2> explicit ctVector2(T2 val);
  template<typename T2> ctVector2(T2 _x, T2 _y);
  template<typename T2> ctVector2(const ctVector2<T2> &copy);

  static ctVector2<T> zero();
  static ctVector2<T> one();

  const ctVector2<T>& assign(T x, T y);
  const ctVector2<T>& assign(const ctVector2<T> &rhs);

  template<typename T2> const ctVector2<T>& assign(T2 x, T2 y);
  template<typename T2> const ctVector2<T>& assign(const ctVector2<T2> &rhs);

  T* data();
  const T* data() const;
  T& operator[](int64_t index);
  const T& operator[](int64_t index) const;

  ctVector2<T> operator-() const;
  ctVector2<T> operator+(const ctVector2<T> &rhs) const;
  ctVector2<T> operator-(const ctVector2<T> &rhs) const;
  ctVector2<T> operator*(const ctVector2<T> &rhs) const;
  ctVector2<T> operator/(const ctVector2<T> &rhs) const;
  ctVector2<T> operator%(const ctVector2<T> &rhs) const;

  const ctVector2<T>& operator+=(const ctVector2<T> &rhs);
  const ctVector2<T>& operator-=(const ctVector2<T> &rhs);
  const ctVector2<T>& operator*=(const ctVector2<T> &rhs);
  const ctVector2<T>& operator/=(const ctVector2<T> &rhs);
  const ctVector2<T>& operator%=(const ctVector2<T> &rhs);
  const ctVector2<T>& operator=(const ctVector2<T> &rhs);

  bool operator==(const ctVector2<T> &rhs) const;
  bool operator!=(const ctVector2<T> &rhs) const;

  ctVector2<T> Add(const ctVector2<T> &rhs) const;
  ctVector2<T> Sub(const ctVector2<T> &rhs) const;
  ctVector2<T> Mul(const ctVector2<T> &rhs) const;
  ctVector2<T> Div(const ctVector2<T> &rhs) const;

  T Mag() const;
  T Length() const;
  T Dot(const ctVector2<T> &rhs) const;
  T Angle(const ctVector2<T> &rhs) const;

  ctVector2<T> Normalize() const;
  ctVector2<T> Reflect(const ctVector2<T> &norm) const;
  ctVector2<T> Project(const ctVector2<T> &to) const;

  // Returns a vector with where each component is the max of this and the input vectors components.
  ctVector2<T> Max(const ctVector2<T> &b) const;

  // Returns a vector with where each component is the min of this and the input vectors components.
  ctVector2<T> Min(const ctVector2<T> &b) const;

  // Returns a vector with where each component is clamped to the min/max of this and the input vectors components
  ctVector2<T> Clamp(const ctVector2<T> &min, const ctVector2<T> &max) const;

  static T Mag(const ctVector2<T> &rhs);
  static T Length(const ctVector2<T> &rhs);
  static T Dot(const ctVector2<T> &lhs, const ctVector2<T> &rhs);
  static T Angle(const ctVector2<T> &lhs, const ctVector2<T> &rhs);

  static ctVector2<T> Project(const ctVector2<T> &vec, const ctVector2<T> &to);
  static ctVector2<T> Reflect(const ctVector2<T> &dir, const ctVector2<T> &norm);
  static ctVector2<T> Normalize(const ctVector2<T> &rhs);

  // Returns a vector with where each component is the max of the input vectors components.
  static ctVector2<T> Max(const ctVector2<T> &a, const ctVector2<T> &b);

  // Returns a vector with where each component is the min of the input vectors components.
  static ctVector2<T> Min(const ctVector2<T> &a, const ctVector2<T> &b);

  // Returns a vector with where each component is clamped to the min/max of the input vectors components
  static ctVector2<T> Clamp(const ctVector2<T> &vec, const ctVector2<T> &min, const ctVector2<T> &max);

  T* begin();
  T* end();
  const T* begin() const;
  const T* end() const;

  union
  {
    struct
    {
      T x;
      T y;
    };

    T m[ElementCount];
  };
};

template<typename T> inline T ctATan2(const ctVector2<T> &pos);
template<typename T> inline ctVector2<T> tmlSolveQuadratic(const T &a, const T &b, const T &c);

template<typename T> inline ctVector2<T> operator*(const T &lhs, const ctVector2<T>& rhs);
template<typename T> inline ctVector2<T> operator/(const T &lhs, const ctVector2<T>& rhs);

#include "ctVector2.inl"

#endif // tmlVector2_h__
