#ifndef tmlVector4_h__
#define tmlVector4_h__

#include "ctVector3.h"

template<typename T> class ctVector4
{
public:
  enum { ElementCount = 4 };

  ctVector4();
  ctVector4(T val);
  ctVector4(T _x, T _y, T _z, T _w);
  ctVector4(const ctVector3<T> &xyz, T _w);
  ctVector4(T _x, const ctVector3<T> &yzw);
  ctVector4(T _x, T _y, const ctVector2<T> &zw);
  ctVector4(T _x, const ctVector2<T> &yz, T _w);
  ctVector4(const ctVector2<T> &xy, T _z, T _w);
  ctVector4(const ctVector2<T> &xy, const ctVector2<T> &yw);

  template<typename T2> explicit ctVector4(T2 val);
  template<typename T2> ctVector4(T2 _x, T2 _y, T2 _z, T2 _w);
  template<typename T2> ctVector4(const ctVector3<T2> &xyz, T2 _w);
  template<typename T2> ctVector4(T2 _x, const ctVector3<T2> &yzw);
  template<typename T2> ctVector4(T2 _x, T2 _y, const ctVector2<T2> &zw);
  template<typename T2> ctVector4(T2 _x, const ctVector2<T2> &yz, T2 _w);
  template<typename T2> ctVector4(const ctVector2<T2> &xy, T2 _z, T2 _w);
  template<typename T2> ctVector4(const ctVector2<T2> &xy, const ctVector2<T2> &yw);
  template<typename T2> ctVector4(ctVector4<T2> copy);

  ctVector4(const ctVector4<T> &copy);
  ctVector4(ctVector4<T> &&move);

  static ctVector4<T> zero();
  static ctVector4<T> one();

  template<typename T2> const ctVector4<T>& assign(T2 _x, T2 _y, T2 _z, T2 _w);
  template<typename T2> const ctVector4<T>& assign(const ctVector4<T2> &rhs);
  const ctVector4<T>& assign(T x, T y, T z, T w);
  const ctVector4<T>& assign(const ctVector4<T> &rhs);

  T& operator[](const int64_t index);
  const T& operator[](const int64_t index) const;

  ctVector4<T> operator-() const;
  ctVector4<T> operator+(const ctVector4<T> &rhs) const;
  ctVector4<T> operator-(const ctVector4<T> &rhs) const;
  ctVector4<T> operator*(const ctVector4<T> &rhs) const;
  ctVector4<T> operator/(const ctVector4<T> &rhs) const;
  ctVector4<T> operator%(const ctVector4<T> &rhs) const;
  const ctVector4<T>& operator+=(const ctVector4<T> &rhs);
  const ctVector4<T>& operator-=(const ctVector4<T> &rhs);
  const ctVector4<T>& operator*=(const ctVector4<T> &rhs);
  const ctVector4<T>& operator/=(const ctVector4<T> &rhs);
  const ctVector4<T>& operator%=(const ctVector4<T> &rhs);
  const ctVector4<T>& operator=(const ctVector4<T> &rhs);
  bool operator==(const ctVector4<T> &rhs) const;
  bool operator!=(const ctVector4<T> &rhs) const;

  ctVector2<T> xy() const;
  ctVector2<T> xz() const;
  ctVector2<T> xw() const;
  ctVector2<T> yz() const;
  ctVector2<T> yw() const;
  ctVector2<T> zw() const;
  ctVector3<T> xyz() const;
  ctVector3<T> xzw() const;
  ctVector3<T> xyw() const;
  ctVector3<T> yzw() const;

  ctVector4<T> Add(const ctVector4<T> &rhs) const;
  ctVector4<T> Sub(const ctVector4<T> &rhs) const;
  ctVector4<T> Mul(const ctVector4<T> &rhs) const;
  ctVector4<T> Div(const ctVector4<T> &rhs) const;

  T Mag() const;
  T Length() const;
  T Dot(const ctVector4<T> &rhs) const;
  T Angle(const ctVector4<T> &rhs) const;

  ctVector4<T> Normalize() const;
  ctVector4<T> Reflect(const ctVector4<T> &norm) const;
  ctVector4<T> Project(const ctVector4<T> &to) const;

  // Returns a vector with where each component is the max of this and the input vectors components.
  ctVector4<T> Max(const ctVector4<T> &b) const;

  // Returns a vector with where each component is the min of this and the input vectors components.
  ctVector4<T> Min(const ctVector4<T> &b) const;

  // Returns a vector with where each component is clamped to the min/max of this and the input vectors components
  ctVector4<T> Clamp(const ctVector4<T> &min, const ctVector4<T> &max) const;

  static T Mag(const ctVector4<T> &rhs);
  static T Length(const ctVector4<T> &rhs);
  static T Dot(const ctVector4<T> &lhs, const ctVector4<T> &rhs);
  static T Angle(const ctVector4<T> &lhs, const ctVector4<T> &rhs);

  static ctVector4<T> Reflect(const ctVector4<T> &dir, const ctVector4<T> &norm);
  static ctVector4<T> Normalize(const ctVector4<T> &rhs);
  static ctVector4<T> Project(const ctVector4<T> &vec, const ctVector4<T> &to);

  // Returns a vector with where each component is the max of the input vectors components.
  static ctVector4<T> Max(const ctVector4<T> &a, const ctVector4<T> &b);

  // Returns a vector with where each component is the min of the input vectors components.
  static ctVector4<T> Min(const ctVector4<T> &a, const ctVector4<T> &b);

  // Returns a vector with where each component is clamped to the min/max of the input vectors components
  static ctVector4<T> Clamp(const ctVector4<T> &vec, const ctVector4<T> &min, const ctVector4<T> &max);

  T* begin();
  T* end();
  const T* begin() const;
  const T* end() const;

  T* data();
  const T* data() const;

  union
  {
    struct
    {
      T x;
      T y;
      T z;
      T w;
    };

    T m[ElementCount];
  };
};

template<typename T> inline ctVector4<T> operator*(const T &lhs, const ctVector4<T> &rhs);
template<typename T> inline ctVector4<T> operator/(const T &lhs, const ctVector4<T> &rhs);

#include "ctVector4.inl"

#endif // tmlVector4_h__
