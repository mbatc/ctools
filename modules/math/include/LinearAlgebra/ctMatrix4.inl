
template<typename T>
inline T atMatrixDet2x2(T a, T b, T c, T d)
{
  return
    (a * d) -
    (b * c);
}

template<typename T>
inline T atMatrixDet3x3(T a, T b, T c, T d, T e, T f, T g, T h, T i)
{
  return
    a * atMatrixDet2x2(e, f, h, i) -
    b * atMatrixDet2x2(d, f, g, i) +
    c * atMatrixDet2x2(d, e, g, h);
}

template<typename T>
ctMatrix4<T>::ctMatrix4(const ctMatrix4<T> &copy)
{
  memcpy(&m, &copy.m, sizeof(T) * 16);
}

template<typename T>
inline ctMatrix4<T>::ctMatrix4(T _00, T _01, T _02, T _03, T _10, T _11, T _12, T _13, T _20, T _21, T _22, T _23, T _30, T _31, T _32, T _33)
{
  m[0] = _00;  m[1] = _01;  m[2] = _02;  m[3] = _03;
  m[4] = _10;  m[5] = _11;  m[6] = _12;  m[7] = _13;
  m[8] = _20;  m[9] = _21;  m[10] = _22; m[11] = _23;
  m[12] = _30; m[13] = _31; m[14] = _32; m[15] = _33;
}

template<typename T>
inline ctMatrix4<T>::ctMatrix4(ctMatrix4<T> &&move)
{
  memcpy(&m, &move.m, sizeof(m));
  move = Identity();
}

template<typename T>
inline ctMatrix4<T> ctMatrix4<T>::Identity()
{
  return ctMatrix4<T>(
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1);
}

template<typename T>
inline ctMatrix4<T> ctMatrix4<T>::Transpose() const
{
  return ctMatrix4<T>(
    m[0], m[4], m[8], m[12],
    m[1], m[5], m[9], m[13],
    m[2], m[6], m[10], m[14],
    m[3], m[7], m[11], m[15]);
}

template<typename T>
inline ctMatrix4<T> ctMatrix4<T>::Cofactors() const
{
  return ctMatrix4<T>(
    atMatrixDet3x3(m[5], m[6], m[7], m[9], m[10], m[11], m[13], m[14], m[15]),
    -atMatrixDet3x3(m[4], m[6], m[7], m[8], m[10], m[11], m[12], m[14], m[15]),
    atMatrixDet3x3(m[4], m[5], m[7], m[8], m[9], m[11], m[12], m[13], m[15]),
    -atMatrixDet3x3(m[4], m[5], m[6], m[8], m[9], m[10], m[12], m[13], m[14]),

    -atMatrixDet3x3(m[1], m[2], m[3], m[9], m[10], m[11], m[13], m[14], m[15]),
    atMatrixDet3x3(m[0], m[2], m[3], m[8], m[10], m[11], m[12], m[14], m[15]),
    -atMatrixDet3x3(m[0], m[1], m[3], m[8], m[9], m[11], m[12], m[13], m[15]),
    atMatrixDet3x3(m[0], m[1], m[2], m[8], m[9], m[10], m[12], m[13], m[14]),

    atMatrixDet3x3(m[1], m[2], m[3], m[5], m[6], m[7], m[13], m[14], m[15]),
    -atMatrixDet3x3(m[0], m[2], m[3], m[4], m[6], m[7], m[12], m[14], m[15]),
    atMatrixDet3x3(m[0], m[1], m[3], m[4], m[5], m[7], m[12], m[13], m[15]),
    -atMatrixDet3x3(m[0], m[1], m[2], m[4], m[5], m[6], m[12], m[13], m[14]),

    -atMatrixDet3x3(m[1], m[2], m[3], m[5], m[6], m[7], m[9], m[10], m[11]),
    atMatrixDet3x3(m[0], m[2], m[3], m[4], m[6], m[7], m[8], m[10], m[11]),
    -atMatrixDet3x3(m[0], m[1], m[3], m[4], m[5], m[7], m[8], m[9], m[11]),
    atMatrixDet3x3(m[0], m[1], m[2], m[4], m[5], m[6], m[8], m[9], m[10])
    );
}

template<typename T>
inline T ctMatrix4<T>::Determinate() const
{
  return
    m[0] * atMatrixDet3x3(m[5], m[6], m[7], m[9], m[10], m[11], m[13], m[14], m[15])
    - m[1] * atMatrixDet3x3(m[4], m[6], m[7], m[8], m[10], m[11], m[12], m[14], m[15])
    + m[2] * atMatrixDet3x3(m[4], m[5], m[7], m[8], m[9], m[11], m[12], m[13], m[15])
    - m[3] * atMatrixDet3x3(m[4], m[5], m[6], m[8], m[9], m[10], m[12], m[13], m[14]);
}

template<typename T>
inline ctMatrix4<T> ctMatrix4<T>::Mul(const ctMatrix4<T> &rhs) const
{
  return ctMatrix4<T>(
    m[0] * rhs.m[0] + m[1] * rhs.m[4] + m[2] * rhs.m[8] + m[3] * rhs.m[12],
    m[0] * rhs.m[1] + m[1] * rhs.m[5] + m[2] * rhs.m[9] + m[3] * rhs.m[13],
    m[0] * rhs.m[2] + m[1] * rhs.m[6] + m[2] * rhs.m[10] + m[3] * rhs.m[14],
    m[0] * rhs.m[3] + m[1] * rhs.m[7] + m[2] * rhs.m[11] + m[3] * rhs.m[15],

    m[4] * rhs.m[0] + m[5] * rhs.m[4] + m[6] * rhs.m[8] + m[7] * rhs.m[12],
    m[4] * rhs.m[1] + m[5] * rhs.m[5] + m[6] * rhs.m[9] + m[7] * rhs.m[13],
    m[4] * rhs.m[2] + m[5] * rhs.m[6] + m[6] * rhs.m[10] + m[7] * rhs.m[14],
    m[4] * rhs.m[3] + m[5] * rhs.m[7] + m[6] * rhs.m[11] + m[7] * rhs.m[15],

    m[8] * rhs.m[0] + m[9] * rhs.m[4] + m[10] * rhs.m[8] + m[11] * rhs.m[12],
    m[8] * rhs.m[1] + m[9] * rhs.m[5] + m[10] * rhs.m[9] + m[11] * rhs.m[13],
    m[8] * rhs.m[2] + m[9] * rhs.m[6] + m[10] * rhs.m[10] + m[11] * rhs.m[14],
    m[8] * rhs.m[3] + m[9] * rhs.m[7] + m[10] * rhs.m[11] + m[11] * rhs.m[15],

    m[12] * rhs.m[0] + m[13] * rhs.m[4] + m[14] * rhs.m[8] + m[15] * rhs.m[12],
    m[12] * rhs.m[1] + m[13] * rhs.m[5] + m[14] * rhs.m[9] + m[15] * rhs.m[13],
    m[12] * rhs.m[2] + m[13] * rhs.m[6] + m[14] * rhs.m[10] + m[15] * rhs.m[14],
    m[12] * rhs.m[3] + m[13] * rhs.m[7] + m[14] * rhs.m[11] + m[15] * rhs.m[15]
    );
}

template<typename T>
inline ctVector4<T> ctMatrix4<T>::Mul(const ctVector4<T> &rhs) const
{
  return ctVector4<T>(
    rhs.x * m[0] + rhs.y * m[1] + rhs.z * m[2] + rhs.w *  m[3],
    rhs.x * m[4] + rhs.y * m[5] + rhs.z * m[6] + rhs.w *  m[7],
    rhs.x * m[8] + rhs.y * m[9] + rhs.z * m[10] + rhs.w * m[11],
    rhs.x * m[12] + rhs.y * m[13] + rhs.z * m[14] + rhs.w * m[15]
    );
}

template<typename T>
inline ctMatrix4<T> ctMatrix4<T>::Apply(T(*func)(const T &)) const
{
  ctMatrix4<T> ret = *this;
  for (T &val : ret.m)
    val = func(val);
  return ret;
}

template<typename T>
inline ctMatrix4<T> ctMatrix4<T>::Mul(const T &rhs) const
{
  return ctMatrix4<T>(
    m[0] * rhs, m[1] * rhs, m[2] * rhs, m[3] * rhs,
    m[4] * rhs, m[5] * rhs, m[6] * rhs, m[7] * rhs,
    m[8] * rhs, m[9] * rhs, m[10] * rhs, m[11] * rhs,
    m[12] * rhs, m[13] * rhs, m[14] * rhs, m[15] * rhs
    );
}

template<typename T>
inline ctMatrix4<T> ctMatrix4<T>::Sub(const T &rhs) const
{
  return ctMatrix4<T>(
    m[0] - rhs, m[1] - rhs, m[2] - rhs, m[3] - rhs,
    m[4] - rhs, m[5] - rhs, m[6] - rhs, m[7] - rhs,
    m[8] - rhs, m[9] - rhs, m[10] - rhs, m[11] - rhs,
    m[12] - rhs, m[13] - rhs, m[14] - rhs, m[15] - rhs
    );
}

template<typename T>
inline ctMatrix4<T> ctMatrix4<T>::Add(const T &rhs) const
{
  return ctMatrix4<T>(
    m[0] + rhs, m[1] + rhs, m[2] + rhs, m[3] + rhs,
    m[4] + rhs, m[5] + rhs, m[6] + rhs, m[7] + rhs,
    m[8] + rhs, m[9] + rhs, m[10] + rhs, m[11] + rhs,
    m[12] + rhs, m[13] + rhs, m[14] + rhs, m[15] + rhs
    );
}

template<typename T>
inline ctMatrix4<T> ctMatrix4<T>::AddDiagonal(const T &rhs) const
{
  ctMatrix4<T> ret = *this;
  ret.m[0] += rhs;
  ret.m[5] += rhs;
  ret.m[10] += rhs;
  ret.m[15] += rhs;
  return ret;
}

template<typename T>
inline const ctMatrix4<T>& ctMatrix4<T>::operator=(const ctMatrix4<T> &copy)
{
  memcpy(&m, &copy.m, sizeof(T) * 16);
  return *this;
}

template<typename T>
inline ctVector3<T> ctMatrix4<T>::Mul(const ctVector3<T> &rhs) const
{
  return ((*this) * ctVector4<T>(rhs, 1)).xyz();
}

template<typename T>
inline ctMatrix4<T> ctMatrix4<T>::Inverse() const
{
  return Cofactors().Transpose().Mul((T)1 / Determinate());
}

template<typename T>
inline ctVector4<T> ctMatrix4<T>::operator*(const ctVector4<T> &rhs) const
{
  return Mul(rhs);
}

template<typename T>
inline ctVector3<T> ctMatrix4<T>::operator*(const ctVector3<T> &rhs) const
{ 
  return Mul(rhs);
}

template<typename T>
inline ctMatrix4<T> ctMatrix4<T>::operator*(const T &rhs) const
{
  return Mul(rhs);
}

template<typename T>
inline ctMatrix4<T> ctMatrix4<T>::operator*(const ctMatrix4<T> &rhs) const
{
  return Mul(rhs);
}

template<typename T>
inline bool ctMatrix4<T>::operator==(const ctMatrix4<T> &rhs) const
{
  return memcmp(&m, &rhs.m, sizeof(T) * 16) == 0;
}

template<typename T>
inline bool ctMatrix4<T>::operator!=(const ctMatrix4<T> &rhs) const
{
  return !(*this == rhs);
}

template<typename T>
inline T& ctMatrix4<T>::operator[](const int64_t index)
{
  return m[index];
}

template<typename T>
inline const T& ctMatrix4<T>::operator[](const int64_t index) const
{
  return m[index];
}

template<typename T>
inline ctMatrix4<T> ctMatrix4<T>::Projection(const T aspect, const T FOV, const T nearPlane, const T farPlane, const T clipSpaceNearZ, const T clipSpaceFarZ)
{
  const T A = (clipSpaceFarZ * farPlane - clipSpaceNearZ * nearPlane) / (nearPlane - farPlane);
  const T B = (clipSpaceFarZ - clipSpaceNearZ) * farPlane * nearPlane / (nearPlane - farPlane);
  const T yScale = 1.0 / atTan(FOV / 2);
  return

    ctMatrix4<T>(
      yScale / aspect, 0, 0, 0,
      0, yScale, 0, 0,
      0, 0, A, B,
      0, 0, -1, 0
      );
}

template<typename T>
inline ctMatrix4<T> ctMatrix4<T>::Ortho(const T width, const T height, const T nearPlane, const T farPlane) { return atMatrixOrtho<T>((T)0, width, (T)0, height, nearPlane, farPlane); }

template<typename T>
inline ctMatrix4<T> ctMatrix4<T>::Ortho(const T left, const T right, const T top, const T bottom, const T nearPlane, const T farPlane)
{
  return
    ctMatrix4<T>(
    (T)2 / (right - left), 0, 0, 0,
      0, (T)2 / (top - bottom), 0, 0,
      0, 0, 2 / (farPlane - nearPlane), 0,
      -(right + left) / (right - left), -(top + bottom) / (top - bottom), -(farPlane + nearPlane) / (farPlane - nearPlane), 1
      );
}

template<typename T>
inline ctMatrix4<T> ctMatrix4<T>::RotationX(const T rads)
{
  const T c = atCos(rads);
  const T s = atSin(rads);
  return
    ctMatrix4<T>(
      1, 0, 0, 0,
      0, c, -s, 0,
      0, s, c, 0,
      0, 0, 0, 1
      );
}

template<typename T>
inline ctMatrix4<T> ctMatrix4<T>::RotationY(const T rads)
{
  const T c = atCos(rads);
  const T s = atSin(rads);
  return
    ctMatrix4<T>(
      c, 0, s, 0,
      0, 1, 0, 0,
      -s, 0, c, 0,
      0, 0, 0, 1
      );
}

template<typename T>
inline ctMatrix4<T> ctMatrix4<T>::RotationZ(const T rads)
{
  const T c = atCos(rads);
  const T s = atSin(rads);
  return
    ctMatrix4<T>(
      c, -s, 0, 0,
      s, c, 0, 0,
      0, 0, 1, 0,
      0, 0, 0, 1
      );
}

template<typename T>
inline ctMatrix4<T> ctMatrix4<T>::Rotation(const ctVector3<T> &axis, T rads)
{
  T c = atCos(rads);
  T s = atSin(rads);
  return
    ctMatrix4<T>(
      c + atSquare(axis.x) * (1 - c), axis.x * axis.y * (1 - c) - axis.z * s, axis.x * axis.z * (1 - c) + axis.y * s, 0,
      axis.y * axis.x * (1 - c) + axis.z * s, c + atSquare(axis.y) * (1 - c), axis.y * axis.z * (1 - c) - axis.x * s, 0,
      axis.z * axis.x * (1 - c) - axis.y * s, axis.z * axis.y * (1 - c) + axis.x * s, c + atSquare(axis.z) * (1 - c), 0,
      0, 0, 0, 1
      );
}

template<typename T>
inline ctMatrix4<T> ctMatrix4<T>::Rotation(const ctQuaternion<T> &quat)
{
  const ctQuaternion<T> &q = quat; // for shorter notation
  const T d = q.Length();
  if (d < atLimitsSmallest<T>())
    return ctMatrix4<T>::Identity();

  T s = T(2) / d;
  T xs = q.x * s, ys = q.y * s, zs = q.z * s;
  T wx = q.w * xs, wy = q.w * ys, wz = q.w * zs;
  T xx = q.x * xs, xy = q.x * ys, xz = q.x * zs;
  T yy = q.y * ys, yz = q.y * zs, zz = q.z * zs;

  return
    ctMatrix4<T>(
      T(1) - (yy + zz), xy - wz, xz + wy, 0,
      xy + wz, T(1) - (xx + zz), yz - wx, 0,
      xz - wy, yz + wx, T(1) - (xx + yy), 0,
      0, 0, 0, 1);
}

template<typename T>
inline ctMatrix4<T> ctMatrix4<T>::Translation(const ctVector3<T> &translation)
{
  return
    ctMatrix4<T>(
      1, 0, 0, translation.x,
      0, 1, 0, translation.y,
      0, 0, 1, translation.z,
      0, 0, 0, 1
      );
}

template<typename T>
inline ctMatrix4<T> ctMatrix4<T>::Scale(const ctVector3<T> &scale)
{
  return
    ctMatrix4<T>(
      scale.x, 0, 0, 0,
      0, scale.y, 0, 0,
      0, 0, scale.z, 0,
      0, 0, 0, 1
      );
}

template<typename T>
inline ctMatrix4<T> ctMatrix4<T>::Scale(const ctVector4<T> &scale)
{
  return
    ctMatrix4<T>(
      scale.x, 0, 0, 0,
      0, scale.y, 0, 0,
      0, 0, scale.z, 0,
      0, 0, 0, scale.w
      );
}

template<typename T>
inline ctMatrix4<T> ctMatrix4<T>::ScaleUniform(const T &scale)
{
  return
    ctMatrix4<T>(
      1, 0, 0, 0,
      0, 1, 0, 0,
      0, 0, 1, 0,
      0, 0, 0, (T)1 / scale
      );
}

template<typename T>
inline ctMatrix4<T> ctMatrix4<T>::YawPitchRoll(const T yaw, const T pitch, const T roll) { return RotationY(yaw) * RotationX(pitch) * RotationZ(roll); }

template<typename T>
inline ctMatrix4<T> ctMatrix4<T>::YawPitchRoll(const ctVector3<T> &ypr) { return YawPitchRoll(ypr.x, ypr.y, ypr.z); }

template<typename T>
inline ctVector3<T> ctMatrix4<T>::GetRotation() const { return GetOrientation().EulerAngles(); }

template<typename T>
inline ctVector3<T> ctMatrix4<T>::GetTranslation() const { return ctVector3<T>(m[3], m[7], m[11]); }

template<typename T>
inline ctVector3<T> ctMatrix4<T>::GetScale() const
{
  return ctVector3<T>(
    ctVector3<T>(m[0], m[4], m[12]).Mag(),
    ctVector3<T>(m[1], m[5], m[13]).Mag(),
    ctVector3<T>(m[2], m[6], m[14]).Mag());
}

template<typename T>
inline ctQuaternion<T> ctMatrix4<T>::GetOrientation() const
{
  T x = T{0};
  T y = T{0};
  T z = T{0};
  T w = T{0};
  T trace = m[0] + m[5] + m[10];
  if (trace > tmlLimitsSmallest<T>())
  {
    T s = T(0.5) / ctSqrt(trace + T(1));
    w = T(0.25) / s;
    x = (m[9] - m[6]) * s;
    y = (m[2] - m[8]) * s;
    z = (m[4] - m[1]) * s;
  }
  else
  {
    if (m[0] > m[5] && m[0] > m[10])
    {
      T s = T(2) * ctSqrt(T(1) + m[0] - m[5] - m[10]);
      w = (m[9] - m[6]) / s;
      x = T(0.25) * s;
      y = (m[1] + m[4]) / s;
      z = (m[2] + m[8]) / s;
    }
    else if (m[5] > m[10])
    {
      T s = T(2) * ctSqrt(T(1) + m[5] - m[0] - m[10]);
      w = (m[2] - m[8]) / s;
      x = (m[1] + m[4]) / s;
      y = T(0.25) * s;
      z = (m[6] + m[9]) / s;
    }
    else
    {
      T s = T(2) * ctSqrt(T(1) + m[10] - m[0] - m[5]);
      w = (m[4] - m[1]) / s;
      x = (m[2] + m[8]) / s;
      y = (m[6] + m[9]) / s;
      z = T(0.25) * s;
    }
  }

  return ctQuaternion(x, y, z, w);
}

template<typename T>
inline void ctMatrix4<T>::Decompose(ctVector3<T> *pTranslation, ctVector3<T> *pRotation, ctVector3<T> *pScale) const
{
  if (pTranslation) *pTranslation = GetTranslation();
  if (pRotation) *pRotation = GetRotation();
  if (pScale) *pScale = GetScale();
}
