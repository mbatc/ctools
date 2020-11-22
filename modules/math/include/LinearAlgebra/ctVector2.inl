
template<typename T>
inline ctVector2<T>::ctVector2()
  : x(0)
  , y(0)
{}

template<typename T>
inline ctVector2<T>::ctVector2(T val)
  : x(val)
  , y(val)
{}

template<typename T>
inline ctVector2<T>::ctVector2(T _x, T _y)
  : x(_x)
  , y(_y)
{}

template<typename T>
inline ctVector2<T>::ctVector2(const ctVector2<T> &copy)
  : x(copy.x)
  , y(copy.y)
{}

template<typename T>
inline ctVector2<T>::ctVector2(ctVector2<T> &&move)
  : x(move.x)
  , y(move.y)
{}

template<typename T>
template<typename T2>
inline ctVector2<T>::ctVector2(T2 val)
  : x((T)val)
  , y((T)val)
{}

template<typename T>
template<typename T2>
inline ctVector2<T>::ctVector2(T2 _x, T2 _y)
  : x((T)_x)
  , y((T)_y)
{}

template<typename T>
template<typename T2> 
inline ctVector2<T>::ctVector2(const ctVector2<T2> &copy)
  : x((T)copy.x)
  , y((T)copy.y)
{}

template<typename T>
inline ctVector2<T> ctVector2<T>::zero()
{
  return ctVector2<T>(0);
}

template<typename T>
inline ctVector2<T> ctVector2<T>::one()
{
  return ctVector2<T>(1);
}

template<typename T>
inline const ctVector2<T>& ctVector2<T>::assign(T x, T y)
{
  this->x = x;
  this->y = y;
  return *this;
}

template<typename T>
template<typename T2>
inline const ctVector2<T>& ctVector2<T>::assign(T2 x, T2 y)
{
  this->x = (T)x;
  this->y = (T)y;
  return *this;
}

template<typename T>
inline const ctVector2<T>& ctVector2<T>::assign(const ctVector2<T> &rhs)
{
  x = rhs.x;
  y = rhs.y;
  return *this;
}

template<typename T>
template<typename T2>
inline const ctVector2<T>& ctVector2<T>::assign(const ctVector2<T2> &rhs)
{
  x = (T)rhs.x;
  y = (T)rhs.y;
  return *this;
}

template<typename T>
inline ctVector2<T> ctVector2<T>::Normalize() const
{
  return Normalize(*this);
}

template<typename T>
inline T ctVector2<T>::Length(const ctVector2<T> &rhs)
{
  return rhs.x * rhs.x + rhs.y * rhs.y;
}

template<typename T>
inline T ctVector2<T>::Dot(const ctVector2<T> &lhs, const ctVector2<T> &rhs)
{
  return  lhs.x * rhs.x +
    lhs.y * rhs.y +
    lhs.z * rhs.z;
}

template<typename T>
inline T ctVector2<T>::Angle(const ctVector2<T> &lhs, const ctVector2<T> &rhs)
{
  return ctACos(Dot(lhs, rhs) / (lhs.Mag() * rhs.Mag()));
}

template<typename T>
inline ctVector2<T> ctVector2<T>::Project(const ctVector2<T> &vec, const ctVector2<T> &to)
{
  return vec * to.Normalize();
}

template<typename T>
inline ctVector2<T> ctVector2<T>::Reflect(const ctVector2<T> &dir, const ctVector2<T> &norm)
{
  return dir - norm * Dot(dir, norm) * 2;
}

template<typename T>
inline ctVector2<T> ctVector2<T>::Max(const ctVector2<T> &a, const ctVector2<T> &b)
{
  return ctVector2<T>(ctMax(a.x, b.x), ctMax(a.y, b.y));
}

template<typename T>
inline ctVector2<T> ctVector2<T>::Min(const ctVector2<T> &a, const ctVector2<T> &b)
{
  return ctVector2<T>(ctMin(a.x, b.x), ctMin(a.y, b.y));
}

template<typename T>
inline ctVector2<T> ctVector2<T>::Clamp(const ctVector2<T> &vec, const ctVector2<T> &min, const ctVector2<T> &max)
{
  return Max(min, Min(vec, max));
}

template<typename T>
inline ctVector2<T> ctVector2<T>::Add(const ctVector2<T> &rhs) const
{
  return *this + rhs;
}

template<typename T>
inline ctVector2<T> ctVector2<T>::Sub(const ctVector2<T> &rhs) const
{
  return *this - rhs;
}

template<typename T>
inline ctVector2<T> ctVector2<T>::Mul(const ctVector2<T> &rhs) const
{
  return *this * rhs;
}

template<typename T>
inline ctVector2<T> ctVector2<T>::Div(const ctVector2<T> &rhs) const
{
  return *this / rhs;
}

template<typename T>
inline T ctVector2<T>::Mag() const
{
  return Mag(*this);
}

template<typename T>
inline T ctVector2<T>::Length() const
{
  return Length(*this);
}

template<typename T>
inline T ctVector2<T>::Dot(const ctVector2<T> &rhs) const
{
  return Dot(*this, rhs);
}

template<typename T>
inline T ctVector2<T>::Angle(const ctVector2<T> &rhs) const
{
  return Angle(*this, rhs);
}

template<typename T>
inline ctVector2<T> ctVector2<T>::Reflect(const ctVector2<T> &norm) const
{
  return Reflect(*this, norm);
}

template<typename T>
inline ctVector2<T> ctVector2<T>::Project(const ctVector2<T> &to) const
{
  return Project(*this, to);
}

template<typename T>
inline ctVector2<T> ctVector2<T>::Max(const ctVector2<T> &b) const
{
  return Max(*this, b);
}

template<typename T>
inline ctVector2<T> ctVector2<T>::Min(const ctVector2<T> &b) const
{
  return Min(*this, b);
}

template<typename T>
inline ctVector2<T> ctVector2<T>::Clamp(const ctVector2<T> &min, const ctVector2<T> &max) const
{
  return Clamp(*this, min, max);
}

template<typename T>
inline T ctVector2<T>::Mag(const ctVector2<T> &rhs)
{
  return ctSqrt(Length(rhs));
}

template<typename T>
inline ctVector2<T> ctVector2<T>::Normalize(const ctVector2<T> &rhs)
{
  return rhs / rhs.Mag();
}

template<typename T>
inline const T* ctVector2<T>::end() const
{
  return data() + 2;
}

template<typename T>
inline const T* ctVector2<T>::begin() const
{
  return data();
}

template<typename T>
inline T* ctVector2<T>::end()
{
  return data() + 2;
}

template<typename T>
inline T* ctVector2<T>::begin()
{
  return data();
}

template<typename T>
inline T* ctVector2<T>::data()
{
  return m;
}

template<typename T>
inline const T* ctVector2<T>::data() const
{
  return m;
}

template<typename T>
inline T& ctVector2<T>::operator[](int64_t index)
{
  return data()[index];
}

template<typename T>
inline const T& ctVector2<T>::operator[](int64_t index) const
{
  return data()[index];
}

template<typename T>
inline ctVector2<T> ctVector2<T>::operator-() const
{
  return ctVector2<T>(-x, -y);
}

template<typename T>
inline ctVector2<T> ctVector2<T>::operator+(const ctVector2<T> &rhs) const
{
  return ctVector2<T>(x + rhs.x, y + rhs.y);
}

template<typename T>
inline ctVector2<T> ctVector2<T>::operator-(const ctVector2<T> &rhs) const
{
  return ctVector2<T>(x - rhs.x, y - rhs.y);
}

template<typename T>
inline ctVector2<T> ctVector2<T>::operator*(const ctVector2<T> &rhs) const
{
  return ctVector2<T>(x * rhs.x, y * rhs.y);
}

template<typename T>
inline ctVector2<T> ctVector2<T>::operator/(const ctVector2<T> &rhs) const
{
  return ctVector2<T>(x / rhs.x, y / rhs.y);
}

template<typename T>
inline ctVector2<T> ctVector2<T>::operator%(const ctVector2<T> &rhs) const
{
  return ctVector2<T>(atMod(x, rhs.x), ctMod(y, rhs.y));
}

template<typename T>
inline const ctVector2<T>& ctVector2<T>::operator+=(const ctVector2<T> &rhs)
{
  return assign(*this + rhs);
}

template<typename T>
inline const ctVector2<T>& ctVector2<T>::operator-=(const ctVector2<T> &rhs)
{
  return assign(*this - rhs);
}

template<typename T>
inline const ctVector2<T>& ctVector2<T>::operator*=(const ctVector2<T> &rhs)
{
  return assign(*this * rhs);
}

template<typename T>
inline const ctVector2<T>& ctVector2<T>::operator/=(const ctVector2<T> &rhs)
{
  return assign(*this / rhs);
}

template<typename T>
inline const ctVector2<T>& ctVector2<T>::operator%=(const ctVector2<T> &rhs)
{
  return assign(*this % rhs);
}

template<typename T>
inline const ctVector2<T>& ctVector2<T>::operator=(const ctVector2<T> &rhs)
{
  return assign(rhs); }

template<typename T>
inline bool ctVector2<T>::operator==(const ctVector2<T> &rhs) const
{
  return x == rhs.x && y == rhs.y;
}

template<typename T>
inline bool ctVector2<T>::operator!=(const ctVector2<T> &rhs) const
{
  return !(*this == rhs);
}

template<typename T>
inline ctVector2<T> tmlSolveQuadratic(const T &a, const T &b, const T &c)
{
  T val = (T)sqrt(b * b - 4 * a * c);
  T ac_2 = 2 * a * c;
  return ctVector2<T>((-b - val) / ac_2, (-b + val) / ac_2);
}

template<typename T>
inline T ctATan2(const ctVector2<T> &pos)
{
  return ctATan2(pos.x, pos.y);
}

template<typename T>
inline ctVector2<T> operator*(const T &lhs, const ctVector2<T>& rhs)
{
  return rhs * lhs;
}

template<typename T>
inline ctVector2<T> operator/(const T &lhs, const ctVector2<T>& rhs)
{
  return ctVector2<T>(lhs / rhs.x, lhs / rhs.y);
}
