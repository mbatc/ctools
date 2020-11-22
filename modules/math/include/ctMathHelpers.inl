#include <utility>

template<typename T> inline T ctATan2(const T &y, const T &x)
{
  if (x == y == 0) return 0;
  const double rad = sqrt(x * x + y * y);
  return 2 * ctATan(x <= 0 ? (rad - x) / y : y / (rad + y));
}

template<typename T> 
inline T ctSquare(const T &x)
{
  return x * x;
}

template<typename T>
inline T ctSin(const T &rads)
{
  return (T)sin(rads);
}

template<typename T>
inline T ctCos(const T &rads)
{
  return (T)cos(rads);
}

template<typename T>
inline T ctTan(const T &rads)
{
  return (T)tan(rads);
}

template<typename T>
inline T ctASin(const T &rads)
{
  return (T)asin(rads);
}

template<typename T>
inline T ctACos(const T &rads)
{
  return (T)acos(rads);
}

template<typename T>
inline T ctATan(const T &rads)
{
  return (T)atan(rads);
}

template<typename T, typename T2>
inline T ctLerp(const T &a, const T &b, const T2 &time)
{
  return a + (b - a) * (time);
}

template<typename InT, typename OutT>
inline OutT ctDerivative(const InT &val, OutT(*func)(InT), const InT step)
{
  return OutT(func(val + step) - func(val - step)) / (step * 2);
}

template<typename InT, typename OutT>
inline OutT ctNthDerivative(const InT &val, OutT(*func)(InT), const int64_t &n, const InT step)
{
  if (n <= 0)
    return ctDerivative(val, func, step);
  OutT rhs = ctNthDerivative(val + step, func, n - 1, step);
  OutT lhs = ctNthDerivative(val - step, func, n - 1, step);
  return OutT(rhs - lhs) / (step * 2);
}

template<typename T>
inline T ctSigmoid(const T &val)
{
  return 1 / (1 + exp(-val));
}

template<typename T>
inline T tmlSigmoidApprox(const T &val)
{
  static T sigLookup[] = {
    T(0),
    ctSigmoid(T(-5)),
    ctSigmoid(T(-4)),
    ctSigmoid(T(-3)),
    ctSigmoid(T(-2)),
    ctSigmoid(T(-1)),
    ctSigmoid(T(0)),
    ctSigmoid(T(1)),
    ctSigmoid(T(2)),
    ctSigmoid(T(3)),
    ctSigmoid(T(4)),
    ctSigmoid(T(5)),
    T(1)
  };
  static int64_t lookupSize = ctArraySize(sigLookup);
  static int64_t sigMid = lookupSize / 2;

  T offset = val + sigMid;
  int64_t lower = ctClamp(int64_t(offset), 0, lookupSize - 1);
  int64_t upper = ctClamp(int64_t(offset + 1), 0, lookupSize - 1);
  return ctLerp(sigLookup[lower], sigLookup[upper], (offset - lower));
}

template<typename T>
inline T ctMod(const T &a, const T &b)
{
  return a % b;
}

template<> inline float ctMod(const float &a, const float &b)
{
  return fmodf(a, b);
}

template<> inline double ctMod(const double &a, const double &b)
{
  return fmod(a, b);
}

template<typename T>
inline T ctNumBitsOn(const T &val)
{
  static uint8_t lookup[16] = { 0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4 };
  if (val == 0)
    return 0;
  uint8_t addr = uint8_t(val & 0x0F);
  return lookup[addr] + ctNumBitsOn(val >> 4);
}

inline float ctSqrt(const float &val)
{
  return sqrtf(val);
}

inline double ctSqrt(const double &val)
{
  return sqrtl(val);
}

inline int64_t ctSqrt(const int64_t &val)
{
  return (int64_t)sqrt((double)val);
}

inline int32_t ctSqrt(const int32_t &val)
{
  return (int32_t)sqrt((double)val);
}

inline int16_t ctSqrt(const int16_t &val)
{
  return (int16_t)sqrt((double)val);
}

inline int8_t ctSqrt(const int8_t &val)
{
  return (int8_t)sqrt((double)val);
}

inline int64_t ctFactorial(const int64_t &n)
{
  return n <= 1 ? 1 : n * ctFactorial(n - 1);
}
