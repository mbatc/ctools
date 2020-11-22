#ifndef tmlQuaternion_h__
#define tmlQuaternion_h__

#include "ctVector3.h"

template<typename T>
class ctQuaternion
{
public:
  ctQuaternion();
  ctQuaternion(const T &x, const T &y, const T &z, const T &w);
  ctQuaternion(const ctVector3<T> &axis, const T &angle);
  ctQuaternion(const T &eulerX, const T &eulerY, const T &eulerZ);

  void Set(const T &x, const T &y, const T &z, const T &w);
  void Set(const ctVector3<T> &axis, const T &angle);
  void Set(const T &eulerX, const T &eulerY, const T &eulerZ);

  static ctQuaternion<T> zero();
  static ctQuaternion<T> Identity();

  static ctQuaternion<T> Normalize(const ctQuaternion<T> &quat);
  static ctQuaternion<T> Inverse(const ctQuaternion<T> &quat);
  static ctQuaternion<T> Mul(const ctQuaternion<T> &lhs, const ctQuaternion<T> &rhs);
  static ctQuaternion<T> Mul(const ctQuaternion<T> &lhs, const T &rhs);
  static ctQuaternion<T> Add(const ctQuaternion<T> &lhs, const ctQuaternion<T> &rhs);
  static ctQuaternion<T> Sub(const ctQuaternion<T> &lhs, const ctQuaternion<T> &rhs);
  static ctQuaternion<T> Div(const ctQuaternion<T> &lhs, const T &rhs);

  static T AngleTo(const ctQuaternion<T> &from, const ctQuaternion<T> &to);
  static T Angle(const ctQuaternion<T> &quat);
  static T Length(const ctQuaternion<T> &quat);
  static T Mag(const ctQuaternion<T> &quat);
  static T Dot(const ctQuaternion<T> &lhs, const ctQuaternion<T> &rhs);

  static ctVector3<T> Rotate(const ctQuaternion<T> &quat, const ctVector3<T> &vec);
  static ctVector3<T> Axis(const ctQuaternion<T> &quat);
  static ctVector3<T> EulerAngles(const ctQuaternion<T> &quat);

  ctQuaternion<T> Slerp(const ctQuaternion &to, const T &factor) const;
  ctQuaternion<T> Normalize() const;
  ctQuaternion<T> Inverse() const;
  ctQuaternion<T> Mul(const ctQuaternion<T> &rhs) const;
  ctQuaternion<T> Mul(const T &rhs) const;
  ctQuaternion<T> Add(const ctQuaternion<T> &rhs) const;
  ctQuaternion<T> Sub(const ctQuaternion<T> &rhs) const;
  ctQuaternion<T> Div(const T &rhs) const;

  T AngleTo(const ctQuaternion<T> &to) const;
  T Angle() const;
  T Length() const;
  T Mag() const;
  T Dot(const ctQuaternion<T> &rhs) const;

  ctVector3<T> EulerAngles() const;
  ctVector3<T> Axis() const;
  ctVector3<T> Rotate(const ctVector3<T> &vec) const;

  bool operator==(const ctQuaternion<T> &rhs) const;
  bool operator!=(const ctQuaternion<T> &rhs) const;

  ctQuaternion<T> operator*(const ctQuaternion<T> &rhs) const;
  ctQuaternion<T> operator-(const ctQuaternion<T> &rhs) const;
  ctQuaternion<T> operator+(const ctQuaternion<T> &rhs) const;
  ctQuaternion<T> operator*(const T &rhs) const;
  ctQuaternion<T> operator/(const T &rhs) const;
  ctQuaternion<T> operator-() const;

  const ctQuaternion<T>& operator=(const ctQuaternion<T> &rhs);
  const ctQuaternion<T>& operator*=(const ctQuaternion<T> &rhs);
  const ctQuaternion<T>& operator*=(const T &rhs);

  const T& at(int64_t index) const;
  const T& operator[](int64_t index) const;
  T& at(int64_t index);
  T& operator[](int64_t index);

  const T* begin() const;
  const T* end() const;
  T* begin();
  T* end();

  T x;
  T y;
  T z;
  T w;
};

template<typename T> ctQuaternion<T> operator*(const ctVector3<T> &lhs, const ctQuaternion<T> &rhs);
template<typename T> ctQuaternion<T> operator*(const ctQuaternion<T> &lhs, const ctVector3<T> &rhs);

#include "ctQuaternion.inl"

#endif // tmlQuaternion_h__
