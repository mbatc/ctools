#ifndef tmlVector3_h__
#define tmlVector3_h__

#include "ctVector2.h"

template<typename T> class ctVector3
{
public:
  enum { ElementCount = 3 };

  ctVector3();
  ctVector3(T val);
  ctVector3(T _x, T _y, T _z);
  ctVector3(T _x, const ctVector2<T> &yz);
  ctVector3(const ctVector2<T> &xy, T _z);
  ctVector3(const ctVector3<T> &copy);
  ctVector3(ctVector3<T> &&move);

  template<typename T2> ctVector3(T2 _x, T2 _y, T2 _z);
  template<typename T2> ctVector3(T2 _x, const ctVector2<T2> &yz);
  template<typename T2> ctVector3(const ctVector2<T2> &xy, T2 _z);
  template<typename T2> ctVector3(ctVector3<T2> copy);
  template<typename T2> explicit ctVector3(T2 val);

  static ctVector3<T> zero();
  static ctVector3<T> one();

  template<typename T2> const ctVector3<T>& assign(T2 _x, T2 _y, T2 _z);
  template<typename T2> const ctVector3<T>& assign(const ctVector3<T2> &rhs);

  const ctVector3<T>& assign(T _x, T _y, T _z);
  const ctVector3<T>& assign(const ctVector3<T> &rhs);

  T& operator[](int64_t index);
  const T& operator[](int64_t index) const;

  ctVector3<T> operator-() const;
  ctVector3<T> operator+(const ctVector3<T> &rhs) const;
  ctVector3<T> operator-(const ctVector3<T> &rhs) const;
  ctVector3<T> operator*(const ctVector3<T> &rhs) const;
  ctVector3<T> operator/(const ctVector3<T> &rhs) const;
  ctVector3<T> operator%(const ctVector3<T> &rhs) const;

  const ctVector3<T>& operator+=(const ctVector3<T> &rhs);
  const ctVector3<T>& operator-=(const ctVector3<T> &rhs);
  const ctVector3<T>& operator*=(const ctVector3<T> &rhs);
  const ctVector3<T>& operator/=(const ctVector3<T> &rhs);
  const ctVector3<T>& operator%=(const ctVector3<T> &rhs);
  const ctVector3<T>& operator=(const ctVector3<T> &rhs);

  bool operator==(const ctVector3<T> &rhs) const;
  bool operator!=(const ctVector3<T> &rhs) const;

  ctVector2<T> xy() const;
  ctVector2<T> xz() const;
  ctVector2<T> yz() const;

  T Mag() const;
  T Length() const;
  T Dot(const ctVector3<T> &rhs) const;
  T Angle(const ctVector3<T> &rhs) const;

  ctVector3<T> Add(const ctVector3<T> &rhs) const;
  ctVector3<T> Sub(const ctVector3<T> &rhs) const;
  ctVector3<T> Mul(const ctVector3<T> &rhs) const;
  ctVector3<T> Div(const ctVector3<T> &rhs) const;

  ctVector3<T> Normalize() const;
  ctVector3<T> Reflect(const ctVector3<T> &norm) const;
  ctVector3<T> Cross(const ctVector3<T> &rhs) const;
  ctVector3<T> Project(const ctVector3<T> &to) const;

  // Returns a vector with where each component is the max of this and the input vectors components.
  ctVector3<T> Max(const ctVector3<T> &b) const;

  // Returns a vector with where each component is the min of this and the input vectors components.
  ctVector3<T> Min(const ctVector3<T> &b) const;

  // Returns a vector with where each component is clamped to the min/max of this and the input vectors components
  ctVector3<T> Clamp(const ctVector3<T> &min, const ctVector3<T> &max) const;

  static T Mag(const ctVector3<T> &rhs);
  static T Length(const ctVector3<T> &rhs);
  static T Dot(const ctVector3<T> &lhs, const ctVector3<T> &rhs);
  static T Angle(const ctVector3<T> &lhs, const ctVector3<T> &rhs);

  static ctVector3<T> Normalize(const ctVector3<T> &rhs);
  static ctVector3<T> Cross(const ctVector3<T> &lhs, const ctVector3<T> &rhs);
  static ctVector3<T> Reflect(const ctVector3<T> &dir, const ctVector3<T> &norm);
  static ctVector3<T> Project(const ctVector3<T> &vec, const ctVector3<T> &to);

  // Returns a vector with where each component is the max of the input vectors components.
  static ctVector3<T> Max(const ctVector3<T> &a, const ctVector3<T> &b);

  // Returns a vector with where each component is the min of the input vectors components.
  static ctVector3<T> Min(const ctVector3<T> &a, const ctVector3<T> &b);

  // Returns a vector with where each component is clamped to the min/max of the input vectors components
  static ctVector3<T> Clamp(const ctVector3<T> &vec, const ctVector3<T> &min, const ctVector3<T> &max);

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
    };

    T m[ElementCount];
  };
};

template<typename T> inline ctVector3<T> operator*(const T &lhs, const ctVector3<T>& rhs);
template<typename T> inline ctVector3<T> operator/(const T &lhs, const ctVector3<T>& rhs);

#include "ctVector3.inl"

#endif // tmlVector3_h__
