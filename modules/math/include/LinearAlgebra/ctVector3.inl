
template<typename T>
inline ctVector3<T>::ctVector3()
  : x(0)
  , y(0)
  , z(0)
{}

template<typename T>
inline ctVector3<T>::ctVector3(T val)
  : x(val)
  , y(val)
  , z(val)
{}

template<typename T>
inline ctVector3<T>::ctVector3(T _x, T _y, T _z)
  : x(_x)
  , y(_y)
  , z(_z)
{}

template<typename T>
inline ctVector3<T>::ctVector3(const ctVector3<T> &copy)
  : x(copy.x)
  , y(copy.y)
  , z(copy.z)
{}

template<typename T>
inline ctVector3<T>::ctVector3(ctVector3<T> &&move)
  : x(move.x)
  , y(move.y)
  , z(move.z)
{}

template<typename T>
template<typename T2>
inline ctVector3<T>::ctVector3(T2 val)
  : x((T)val)
  , y((T)val)
  , z((T)val)
{}

template<typename T>
template<typename T2>
inline ctVector3<T>::ctVector3(T2 _x, T2 _y, T2 _z)
  : x((T)_x)
  , y((T)_y)
  , z((T)_z)
{}

template<typename T>
template<typename T2>
inline ctVector3<T>::ctVector3(T2 _x, const ctVector2<T2> &yz)
  : x((T)_x)
  , y((T)yz.x)
  , z((T)yz.y)
{}

template<typename T>
template<typename T2>
inline ctVector3<T>::ctVector3(const ctVector2<T2> &xy, T2 _z)
  : x((T)xy.x)
  , y((T)xy.y)
  , z((T)_z)
{}

template<typename T>
template<typename T2>
inline ctVector3<T>::ctVector3(ctVector3<T2> copy)
  : x((T)copy.x)
  , y((T)copy.y)
  , z((T)copy.z)
{}

template<typename T>
inline const ctVector3<T>& ctVector3<T>::assign(T _x, T _y, T _z)
{
  x = _x;
  y = _y;
  z = _z;
  return *this;
}

template<typename T> 
inline const ctVector3<T>& ctVector3<T>::assign(const ctVector3<T>& rhs)
{
  x = rhs.x;
  y = rhs.y;
  z = rhs.z;
  return *this;
}

template<typename T>
template<typename T2>
inline const ctVector3<T>& ctVector3<T>::assign(T2 _x, T2 _y, T2 _z)
{
  x = (T)_x;
  y = (T)_y;
  z = (T)_z;
  return *this;
}

template<typename T>
template<typename T2>
inline const ctVector3<T>& ctVector3<T>::assign(const ctVector3<T2>& rhs)
{
  x = (T)rhs.x;
  y = (T)rhs.y;
  z = (T)rhs.z;
  return *this;
}

template<typename T>
inline T ctVector3<T>::Length(const ctVector3<T> &rhs)
{
  return rhs.x * rhs.x +
    rhs.y * rhs.y +
    rhs.z * rhs.z;
}

template<typename T>
inline T ctVector3<T>::Dot(const ctVector3<T> &lhs, const ctVector3<T> &rhs)
{
  return lhs.x * rhs.x +
    lhs.y * rhs.y +
    lhs.z * rhs.z;
}

template<typename T>
inline ctVector3<T> ctVector3<T>::Cross(const ctVector3<T> &lhs, const ctVector3<T> &rhs)
{
  return  ctVector3<T>(
    lhs[1] * rhs[2] - lhs[2] * rhs[1],
    lhs[2] * rhs[0] - lhs[0] * rhs[2],
    lhs[0] * rhs[1] - lhs[1] * rhs[0]
    );
}

template<typename T>
inline ctVector3<T> ctVector3<T>::Reflect(const ctVector3<T> &dir, const ctVector3<T> &norm)
{
  return dir - norm * Dot(dir, norm) * 2;
}

template<typename T>
inline ctVector3<T> ctVector3<T>::Project(const ctVector3<T>& vec, const ctVector3<T>& to)
{
  double toMag = to.Mag();
  return vec.Dot(to) / toMag * (to / toMag);
}

template<typename T>
inline ctVector3<T> ctVector3<T>::Max(const ctVector3<T> &a, const ctVector3<T> &b)
{
  return ctVector3<T>(ctMax(a.x, b.x), ctMax(a.y, b.y), ctMax(a.z, b.z));
}

template<typename T>
inline ctVector3<T> ctVector3<T>::Min(const ctVector3<T> &a, const ctVector3<T> &b)
{
  return ctVector3<T>(ctMin(a.x, b.x), ctMin(a.y, b.y), ctMin(a.z, b.z));
}

template<typename T>
inline ctVector3<T> ctVector3<T>::Clamp(const ctVector3<T> &vec, const ctVector3<T> &min, const ctVector3<T> &max)
{
  return Max(min, Min(vec, max));
}

template<typename T>
inline ctVector3<T>::ctVector3(T _x, const ctVector2<T>& yz)
  : ctVector3(_x, yz.x, yz.y)
{}

template<typename T>
inline ctVector3<T>::ctVector3(const ctVector2<T>& xy, T _z)
  : ctVector3(xy.x, xy.y, _z)
{}

template<typename T>
inline T ctVector3<T>::Mag() const
{
  return Mag(*this);
}

template<typename T>
inline T ctVector3<T>::Length() const
{
  return Length(*this);
}

template<typename T>
inline T ctVector3<T>::Dot(const ctVector3<T> &rhs) const
{
  return Dot(*this, rhs);
}

template<typename T>
inline T ctVector3<T>::Angle(const ctVector3<T> &rhs) const
{
  return Angle(*this, rhs);
}

template<typename T>
inline ctVector3<T> ctVector3<T>::Add(const ctVector3<T> &rhs) const
{
  return *this + rhs;
}

template<typename T>
inline ctVector3<T> ctVector3<T>::Sub(const ctVector3<T> &rhs) const
{
  return *this - rhs;
}

template<typename T>
inline ctVector3<T> ctVector3<T>::Mul(const ctVector3<T> &rhs) const
{
  return *this * rhs;
}

template<typename T>
inline ctVector3<T> ctVector3<T>::Div(const ctVector3<T> &rhs) const
{
  return *this / rhs;
}

template<typename T>
inline ctVector3<T> ctVector3<T>::Normalize() const
{
  return Normalize(*this);
}

template<typename T>
inline ctVector3<T> ctVector3<T>::Reflect(const ctVector3<T> &norm) const
{
  return Reflect(*this, norm);
}

template<typename T>
inline ctVector3<T> ctVector3<T>::Cross(const ctVector3<T> &rhs) const
{
  return Cross(*this, rhs);
}

template<typename T>
inline ctVector3<T> ctVector3<T>::Project(const ctVector3<T>& to) const
{
  return Project(*this, to);
}

template<typename T>
inline ctVector3<T> ctVector3<T>::Max(const ctVector3<T> &b) const
{
  return Max(*this, b);
}

template<typename T>
inline ctVector3<T> ctVector3<T>::Min(const ctVector3<T> &b) const
{
  return Min(*this, b);
}

template<typename T>
inline ctVector3<T> ctVector3<T>::Clamp(const ctVector3<T> &min, const ctVector3<T> &max) const
{
  return Clamp(*this, min, max);
}

template<typename T>
inline T ctVector3<T>::Mag(const ctVector3<T> &rhs)
{
  return ctSqrt(Length(rhs));
}

template<typename T>
inline T ctVector3<T>::Angle(const ctVector3<T> &lhs, const ctVector3<T> &rhs)
{
  return ctACos(Dot(lhs, rhs) / (Mag(lhs) * Mag(rhs)));
}

template<typename T>
inline ctVector3<T> ctVector3<T>::Normalize(const ctVector3<T> &rhs)
{
  return rhs / Mag(rhs);
}

template<typename T>
inline ctVector3<T> ctVector3<T>::zero()
{
  return ctVector3<T>(0);
}

template<typename T>
inline ctVector3<T> ctVector3<T>::one()
{
  return ctVector3<T>(1);
}

template<typename T>
inline ctVector2<T> ctVector3<T>::xy() const
{
  return ctVector2<T>(x, y);
}

template<typename T>
inline ctVector2<T> ctVector3<T>::xz() const
{
  return ctVector2<T>(x, z);
}

template<typename T>
inline ctVector2<T> ctVector3<T>::yz() const
{
  return ctVector2<T>(y, z);
}

template<typename T>
inline T& ctVector3<T>::operator[](int64_t index)
{
  return m[index];
}

template<typename T>
inline const T & ctVector3<T>::operator[](int64_t index) const
{
  return m[index];
}

template<typename T>
inline ctVector3<T> ctVector3<T>::operator-() const
{
  return ctVector3<T>(-x, -y, -z);
}

template<typename T>
inline ctVector3<T> ctVector3<T>::operator+(const ctVector3<T>& rhs) const
{
  return ctVector3<T>(x + rhs.x, y + rhs.y, z + rhs.z);
}

template<typename T>
inline ctVector3<T> ctVector3<T>::operator-(const ctVector3<T>& rhs) const
{
  return ctVector3<T>(x - rhs.x, y - rhs.y, z - rhs.z);
}

template<typename T>
inline ctVector3<T> ctVector3<T>::operator*(const ctVector3<T>& rhs) const
{
  return ctVector3<T>(x * rhs.x, y * rhs.y, z * rhs.z);
}

template<typename T>
inline ctVector3<T> ctVector3<T>::operator/(const ctVector3<T>& rhs) const
{
  return ctVector3<T>(x / rhs.x, y / rhs.y, z / rhs.z);
}

template<typename T>
inline ctVector3<T> ctVector3<T>::operator%(const ctVector3<T>& rhs) const
{
  return ctVector3<T>(atMod(x, rhs.x), ctMod(y, rhs.y), ctMod(z, rhs.z));
}

template<typename T>
inline const ctVector3<T>& ctVector3<T>::operator+=(const ctVector3<T>& rhs)
{
  return assign(*this + rhs);
}

template<typename T>
inline const ctVector3<T>& ctVector3<T>::operator-=(const ctVector3<T>& rhs)
{
  return assign(*this - rhs);
}

template<typename T>
inline const ctVector3<T>& ctVector3<T>::operator*=(const ctVector3<T>& rhs)
{
  return assign(*this * rhs);
}

template<typename T>
inline const ctVector3<T>& ctVector3<T>::operator/=(const ctVector3<T>& rhs)
{
  return assign(*this / rhs);
}

template<typename T>
inline const ctVector3<T>& ctVector3<T>::operator%=(const ctVector3<T>& rhs)
{
  return assign(*this % rhs);
}

template<typename T>
inline const ctVector3<T>& ctVector3<T>::operator=(const ctVector3<T>& rhs)
{
  return assign(rhs);
}

template<typename T>
inline bool ctVector3<T>::operator==(const ctVector3<T>& rhs) const
{
  return memcmp(this, &rhs, sizeof(ctVector3<T>)) == 0;
}

template<typename T>
inline bool ctVector3<T>::operator!=(const ctVector3<T>& rhs) const
{
  return !(*this == rhs);
}

template<typename T>
inline const T* ctVector3<T>::end() const
{
  return m + ElementCount;
}

template<typename T>
inline const T* ctVector3<T>::begin() const
{
  return m;
}

template<typename T>
inline const T* ctVector3<T>::data() const
{
  return m_;
}

template<typename T>
inline T* ctVector3<T>::end()
{
  return m + ElementCount;
}

template<typename T>
inline T* ctVector3<T>::begin()
{
  return m;
}

template<typename T>
inline T* ctVector3<T>::data()
{
  return m;
}

template<typename T>
inline ctVector3<T> operator*(const T &lhs, const ctVector3<T>& rhs)
{
  return rhs * lhs;
}

template<typename T>
inline ctVector3<T> operator/(const T &lhs, const ctVector3<T>& rhs)
{
  return ctVector3<T>(lhs / rhs.x, lhs / rhs.y, lhs / rhs.z);
}
