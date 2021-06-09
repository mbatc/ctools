
template<typename T>
inline ctQuaternion<T>::ctQuaternion(const ctVector3<T> &axis, const T &angle) { Set(axis, angle); }

template<typename T>
inline ctQuaternion<T>::ctQuaternion(const T &eulerX, const T &eulerY, const T &eulerZ) { Set(eulerX, eulerY, eulerZ); }

template<typename T>
inline ctQuaternion<T>::ctQuaternion()
  : x(0)
  , y(0)
  , z(0)
  , w(1)
{}

template<typename T>
inline ctQuaternion<T>::ctQuaternion(const T &x, const T &y, const T &z, const T &w)
  : x(x)
  , y(y)
  , z(z)
  , w(w)
{}

template<typename T>
inline void ctQuaternion<T>::Set(const T &_x, const T &_y, const T &_z, const T &_w)
{
  x = _x;
  y = _y;
  z = _z;
  w = _w;
}

template<typename T>
inline void ctQuaternion<T>::Set(const ctVector3<T> &axis, const T &angle)
{
  T dist = axis.Mag();
  T s = ctSin(angle * T(0.5)) / dist;
  Set(axis.x * s, axis.y * s, axis.z * s, ctCos(angle * T(0.5)));
}

template<typename T>
inline void ctQuaternion<T>::Set(const T &eulerX, const T &eulerY, const T &eulerZ)
{
  double cy = cos(eulerY * 0.5);
  double sy = sin(eulerY * 0.5);
  double cp = cos(eulerX * 0.5);
  double sp = sin(eulerX * 0.5);
  double cr = cos(eulerZ * 0.5);
  double sr = sin(eulerZ * 0.5);

  w = cy * cp * cr + sy * sp * sr;
  x = cy * cp * sr - sy * sp * cr;
  y = sy * cp * sr + cy * sp * cr;
  z = sy * cp * cr - cy * sp * sr;
}

template<typename T>
inline ctQuaternion<T> ctQuaternion<T>::zero()
{
  return ctQuaternion<T>(0, 0, 0, 0);
}

template<typename T>
inline ctQuaternion<T> ctQuaternion<T>::Identity()
{
  return ctQuaternion<T>(0, 0, 0, 1);
}

template<typename T>
inline ctQuaternion<T> ctQuaternion<T>::Normalize(const ctQuaternion<T> &quat)
{
  return quat / Mag(quat);
}

template<typename T>
inline ctQuaternion<T> ctQuaternion<T>::Inverse(const ctQuaternion<T> &quat)
{
  return ctQuaternion<T>(-quat.x, -quat.y, -quat.z, quat.w);
}

template<typename T>
inline ctQuaternion<T> ctQuaternion<T>::Mul(const ctQuaternion<T> &lhs, const ctQuaternion<T> &rhs)
{
  return ctQuaternion<T>(
    lhs.w * rhs.x + lhs.x * rhs.w + lhs.y * rhs.z - lhs.z * rhs.y,
    lhs.w * rhs.y + lhs.y * rhs.w + lhs.z * rhs.x - lhs.x * rhs.z,
    lhs.w * rhs.z + lhs.z * rhs.w + lhs.x * rhs.y - lhs.y * rhs.x,
    lhs.w * rhs.w - lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z
    );
}

template<typename T>
inline ctQuaternion<T> ctQuaternion<T>::Mul(const ctQuaternion<T> &lhs, const T &rhs)
{
  return ctQuaternion<T>(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs, lhs.w * rhs);
}

template<typename T>
inline ctQuaternion<T> ctQuaternion<T>::Add(const ctQuaternion<T> &lhs, const ctQuaternion<T> &rhs)
{
  return ctQuaternion<T>(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
}

template<typename T>
inline ctQuaternion<T> ctQuaternion<T>::Sub(const ctQuaternion<T> &lhs, const ctQuaternion<T> &rhs)
{
  return ctQuaternion<T>(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
}

template<typename T>
inline ctQuaternion<T> ctQuaternion<T>::Div(const ctQuaternion<T> &lhs, const T &rhs)
{
  T reciprocal = T(1) / rhs;
  return Mul(lhs, reciprocal);
}

template<typename T>
inline T ctQuaternion<T>::AngleTo(const ctQuaternion<T> &from, const ctQuaternion<T> &to)
{
  T s = ctSqrt(from.Length() * to.Length());
  if (abs(s) < tmlLimitsSmallest<T>())
    return 0;
  return 2 * ctACos(Dot(to) / s);
}

template<typename T>
inline T ctQuaternion<T>::Angle(const ctQuaternion<T> &quat)
{
  return T(2) * ctACos(quat.w);
}

template<typename T>
inline T ctQuaternion<T>::Length(const ctQuaternion<T> &quat)
{
  return Dot(quat, quat);
}

template<typename T>
inline T ctQuaternion<T>::Mag(const ctQuaternion<T> &quat)
{
  return ctSqrt(Length(quat));
}

template<typename T>
inline T ctQuaternion<T>::Dot(const ctQuaternion<T> &lhs, const ctQuaternion<T> &rhs)
{
  return
    lhs.x * rhs.x +
    lhs.y * rhs.y +
    lhs.z * rhs.z +
    lhs.w * rhs.w;
}

template<typename T>
inline ctVector3<T> ctQuaternion<T>::Rotate(const ctQuaternion<T> &quat, const ctVector3<T> &vec)
{
  ctQuaternion<T> q = quat * vec;
  q *= quat.Inverse();
  return ctVector3<T>(q.x, q.y, q.z);
}

template<typename T>
inline ctVector3<T> ctQuaternion<T>::Axis(const ctQuaternion<T> &quat)
{
  T s_sqr = T(1) - ctSquare(quat.w);
  T s = T(1) / ctSqrt(s_sqr);
  if (s < tmlLimitsSmallest<T>())
    return ctVector3<T>(1, 0, 0);
  return ctVector3<T>(quat.x * s, quat.y * s, quat.z * s);
}

template<typename T>
inline ctVector3<T> ctQuaternion<T>::EulerAngles(const ctQuaternion<T> &quat)
{
  ctVector3<T> euler;
  // roll (x-axis rotation)
  double sinr_cosp = +2.0 * (quat.w * quat.x + quat.y * quat.z);
  double cosr_cosp = +1.0 - 2.0 * (quat.x * quat.x + quat.y * quat.y);
  euler.z = ctATan2(sinr_cosp, cosr_cosp);

  // pitch (y-axis rotation)
  double sinp = +2.0 * (quat.w * quat.y - quat.z * quat.x);
  if (fabs(sinp) >= 1)
    euler.x = copysign(ctPi_2, sinp); // use 90 degrees if out of range
  else
    euler.x = ctASin(sinp);

  // yaw (z-axis rotation)
  double siny_cosp = +2.0 * (quat.w * quat.z + quat.x * quat.y);
  double cosy_cosp = +1.0 - 2.0 * (quat.y * quat.y + quat.z * quat.z);
  euler.y = ctATan2(siny_cosp, cosy_cosp);
  return euler;
}

template<typename T>
inline ctQuaternion<T> ctQuaternion<T>::Slerp(const ctQuaternion<T> &to, const T &factor) const
{
  ctQuaternion<T> result = *this;

  T mag = ctSqrt(Length() * to.Length());
  if (mag > ctLimitsSmallest<T>())
  {
    const T product = Dot(to) / mag;
    const T absProduct = abs(product);

    if (absProduct < T(1) - ctLimitsSmallest<T>())
    {
      const T theta = ctACos(absProduct);
      const T d = ctSin(theta);
      if (d > 0)
      {
        T sign = product < 0 ? -1 : 1;
        T s0 = ctSin((T(1) - factor) * theta) / d;
        T s1 = ctSin(sign * factor * theta) / d;
        result.Set(
          x * s0 + to.x * s1,
          y * s0 + to.y * s1,
          z * s0 + to.z * s1,
          w * s0 + to.w * s1
        );
      }
    }
  }
  return result;
}

template<typename T>
inline bool ctQuaternion<T>::operator==(const ctQuaternion<T> &rhs) const
{
  return x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w;
}

template<typename T> 
inline bool ctQuaternion<T>::operator!=(const ctQuaternion<T>& rhs) const
{
  return !(*this == rhs);
}

template<typename T>
inline ctQuaternion<T> ctQuaternion<T>::operator-() const
{
  return ctQuaternion<T>(-x, -y, -z, -w);
}

template<typename T>
inline const ctQuaternion<T>& ctQuaternion<T>::operator=(const ctQuaternion<T> &rhs)
{
  x = rhs.x;
  y = rhs.y;
  z = rhs.z;
  w = rhs.w;
  return *this;
}

template<typename T>
inline ctQuaternion<T> operator*(const ctVector3<T> &lhs, const ctQuaternion<T> &rhs)
{
  return ctQuaternion<T>(
    +lhs.x * rhs.w + lhs.y * rhs.z - lhs.z * rhs.y,
    +lhs.y * rhs.w + lhs.z * rhs.x - lhs.x * rhs.z,
    +lhs.z * rhs.w + lhs.x * rhs.y - lhs.y * rhs.x,
    -lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z);
}

template<typename T>
inline ctQuaternion<T> operator*(const ctQuaternion<T> &lhs, const ctVector3<T> &rhs)
{
  return ctQuaternion<T>(
    lhs.w * rhs.x + lhs.y * rhs.z - lhs.z * rhs.y,
    lhs.w * rhs.y + lhs.z * rhs.x - lhs.x * rhs.z,
    lhs.w * rhs.z + lhs.x * rhs.y - lhs.y * rhs.x,
    -lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z);
}

template<typename T>
inline ctQuaternion<T> ctQuaternion<T>::Normalize() const
{
  return Normalize(*this);
}

template<typename T>
inline ctQuaternion<T> ctQuaternion<T>::Inverse() const
{
  return Inverse(*this);
}

template<typename T>
inline ctQuaternion<T> ctQuaternion<T>::Mul(const ctQuaternion<T> &rhs) const
{
  return Mul(*this, rhs);
}

template<typename T>
inline ctQuaternion<T> ctQuaternion<T>::Mul(const T &rhs) const
{
  return Mul(*this, rhs);
}

template<typename T>
inline ctQuaternion<T> ctQuaternion<T>::Add(const ctQuaternion<T> &rhs) const
{
  return Add(*this, rhs);
}

template<typename T>
inline ctQuaternion<T> ctQuaternion<T>::Sub(const ctQuaternion<T> &rhs) const
{
  return Sub(*this, rhs);
}

template<typename T>
inline ctQuaternion<T> ctQuaternion<T>::Div(const T &rhs) const
{
  return Div(*this, rhs);
}

template<typename T>
inline T ctQuaternion<T>::AngleTo(const ctQuaternion<T> &to) const
{
  return AngleTo(*this, to);
}

template<typename T>
inline T ctQuaternion<T>::Angle() const
{
  return Angle(*this);
}

template<typename T>
inline T ctQuaternion<T>::Length() const
{
  return Length(*this);
}

template<typename T>
inline T ctQuaternion<T>::Mag() const
{
  return Mag(*this);
}

template<typename T>
inline T ctQuaternion<T>::Dot(const ctQuaternion<T>& rhs) const
{
  return Dot(*this, rhs);
}

template<typename T>
inline ctVector3<T> ctQuaternion<T>::EulerAngles() const
{
  return EulerAngles(*this);
}

template<typename T>
inline ctVector3<T> ctQuaternion<T>::Axis() const
{
  return Axis(*this);
}

template<typename T>
inline ctVector3<T> ctQuaternion<T>::Rotate(const ctVector3<T> &vec) const
{
  return Rotate(*this, vec);
}

template<typename T>
inline ctQuaternion<T> ctQuaternion<T>::operator*(const ctQuaternion<T> &rhs) const
{
  return Mul(rhs);
}

template<typename T>
inline ctQuaternion<T> ctQuaternion<T>::operator-(const ctQuaternion<T> &rhs) const
{ 
  return Sub(rhs);
}

template<typename T>
inline ctQuaternion<T> ctQuaternion<T>::operator+(const ctQuaternion<T> &rhs) const
{
  return Add(rhs); }

template<typename T>
inline ctQuaternion<T> ctQuaternion<T>::operator*(const T &rhs) const
{
  return Mul(rhs);
}

template<typename T>
inline ctQuaternion<T> ctQuaternion<T>::operator/(const T &rhs) const
{
  return Div(rhs);
}

template<typename T>
inline const ctQuaternion<T>& ctQuaternion<T>::operator*=(const ctQuaternion<T> &rhs)
{
  return *this = Mul(rhs);
}

template<typename T>
inline const ctQuaternion<T>& ctQuaternion<T>::operator*=(const T &rhs)
{
  return *this = Mul(rhs);
}

template<typename T>
inline const T& ctQuaternion<T>::at(int64_t index) const
{
  return (&x)[index];
}

template<typename T>
inline const T& ctQuaternion<T>::operator[](int64_t index) const
{
  return at(index);
}

template<typename T>
inline T& ctQuaternion<T>::at(int64_t index)
{
  return (&x)[index];
}

template<typename T>
inline T& ctQuaternion<T>::operator[](int64_t index)
{
  return at(index);
}

template<typename T>
inline const T* ctQuaternion<T>::begin() const
{
  return &x;
}

template<typename T>
inline const T* ctQuaternion<T>::end() const
{
  return &w + 1;
}

template<typename T>
inline T* ctQuaternion<T>::begin()
{
  return &x;
}

template<typename T>
inline T* ctQuaternion<T>::end()
{
  return &w + 1;
}
