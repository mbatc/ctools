#ifndef tmlMatrix4_h__
#define tmlMatrix4_h__

#include "ctVector4.h"
#include "ctQuaternion.h"

template<typename T> inline T atMatrixDet2x2(T a, T b, T c, T d);
template<typename T> inline T atMatrixDet3x3(T a, T b, T c, T d, T e, T f, T g, T h, T i);

template<typename T> class ctMatrix4
{
public:
  ctMatrix4(
    T _00 = 1, T _01 = 0, T _02 = 0, T _03 = 0,
    T _10 = 0, T _11 = 1, T _12 = 0, T _13 = 0,
    T _20 = 0, T _21 = 0, T _22 = 1, T _23 = 0,
    T _30 = 0, T _31 = 0, T _32 = 0, T _33 = 1);

  ctMatrix4(ctMatrix4<T> &&move);
  ctMatrix4(const ctMatrix4<T> &copy);

  template<typename T2>
  ctMatrix4(const ctMatrix4<T2> &copy);

  ctMatrix4<T> Transpose() const;
  ctMatrix4<T> Cofactors() const;
  ctMatrix4<T> Inverse() const;

  T Determinate() const;

  ctMatrix4<T> Mul(const T &rhs) const;
  ctMatrix4<T> Sub(const T &rhs) const;
  ctMatrix4<T> Add(const T &rhs) const;
  ctMatrix4<T> AddDiagonal(const T &rhs) const;
  ctMatrix4<T> Mul(const ctMatrix4<T> &rhs) const;
  ctVector4<T> Mul(const ctVector4<T> &rhs) const;
  ctVector3<T> Mul(const ctVector3<T> &rhs) const;
  ctMatrix4<T> Apply(T(*func)(const T &)) const;

  ctVector3<T> GetTranslation() const;
  ctVector3<T> GetRotation() const;
  ctVector3<T> GetScale() const;
  ctQuaternion<T> GetOrientation() const;

  void Decompose(ctVector3<T> *pTranslation, ctVector3<T> *pRotation, ctVector3<T> *pScale) const;

  static ctMatrix4<T> Identity();
  static ctMatrix4<T> Projection(const T aspect, const T FOV, const T nearPlane, const T farPlane, const T zClipNear = 0, const T zClipFar = 1);
  static ctMatrix4<T> Ortho(const T width, const T height, const T nearPlane, const T farPlane);
  static ctMatrix4<T> Ortho(const T left, const T right, const T top, const T bottom, const T nearPlane, const T farPlane);
  static ctMatrix4<T> RotationX(const T rads);
  static ctMatrix4<T> RotationY(const T rads);
  static ctMatrix4<T> RotationZ(const T rads);
  static ctMatrix4<T> Rotation(const ctVector3<T> &axis, T rads);
  static ctMatrix4<T> Rotation(const ctQuaternion<T> &quat);
  static ctMatrix4<T> YawPitchRoll(const T yaw, const T pitch, const T roll);
  static ctMatrix4<T> YawPitchRoll(const ctVector3<T> &ypr);
  static ctMatrix4<T> Translation(const ctVector3<T> &translation);
  static ctMatrix4<T> Scale(const ctVector3<T> &scale);
  static ctMatrix4<T> Scale(const ctVector4<T> &scale);
  static ctMatrix4<T> ScaleUniform(const T &scale);

  ctVector4<T> operator*(const ctVector4<T> &rhs) const;
  ctVector3<T> operator*(const ctVector3<T> &rhs) const;
  ctMatrix4<T> operator*(const T &rhs) const;
  ctMatrix4<T> operator*(const ctMatrix4<T> &rhs) const;

  bool operator==(const ctMatrix4<T> &rhs) const;
  bool operator!=(const ctMatrix4<T> &rhs) const;
  const ctMatrix4<T> &operator=(const ctMatrix4<T> &copy);

  T& operator[](const int64_t index);
  const T& operator[](const int64_t index) const;

  union
  {
    T m[16];
    ctVector4<T> row[4];
  };
};

#include "ctMatrix4.inl"

#endif // tmlMatrix4_h__