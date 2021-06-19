#ifndef tmlHelpers_h__
#define tmlHelpers_h__

#include "ctLimits.h"

#include <math.h>

#define ctE 2.71828182845904523536         // e
#define ctLog2e 1.44269504088896340736     // log2(e)
#define ctLog10e 0.434294481903251827651   // log10(e)
#define ctLn2 0.693147180559945309417      // ln(2)
#define ctLn10  2.30258509299404568402     // ln(10)
#define ctPi 3.14159265358979323846        // pi
#define ctPi_2 1.57079632679489661923      // pi/2
#define ctPi_4 0.785398163397448309616     // pi/4
#define ct1_PI 0.318309886183790671538     // 1/pi
#define ct2_PI 0.636619772367581343076     // 2/pi
#define ctSqrtPi 1.12837916709551257390    // 2/sqrt(pi)
#define ctSqrt2 1.41421356237309504880     // sqrt(2)
#define ctSqrt1_2 0.707106781186547524401  // 1/sqrt(2)
#define ctDegs2Rads(x) (x * 0.017453292519943295769)
#define ctRads2Degs(x) (x * 57.29577951308232087679)
#define ctDegs2Radsf(x) (x * 0.017453292519943295769f)
#define ctRads2Degsf(x) (x * 57.29577951308232087679f)

template<typename T> inline T ctSquare(const T &x);
template<typename T> inline T ctSin(const T &rads);
template<typename T> inline T ctCos(const T &rads);
template<typename T> inline T ctTan(const T &rads);
template<typename T> inline T ctASin(const T &rads);
template<typename T> inline T ctACos(const T &rads);
template<typename T> inline T ctATan(const T &rads);
template<typename T> inline T ctATan2(const T &y, const T &x);

template<typename T> inline T ctSigmoid(T val);
template<typename InT, typename OutT> inline OutT ctDerivative(const InT &val, OutT (*func)(InT), const InT step = InT(0.001));
template<typename InT, typename OutT> inline OutT ctNthDerivative(const InT &val, OutT(*func)(InT), const int64_t &n, const InT step = InT(0.001));

template<typename T> inline T ctMod(const T &a, const T &b);
template<> inline float ctMod(const float &a, const float &b);
template<> inline double ctMod(const double &a, const double &b);

template<typename T, typename T2> inline T ctLerp(const T &a, const T &b, const T2 &time);

template<typename T> inline T ctNumBitsOn(const T &val);

inline int64_t ctFactorial(const int64_t &n);

inline float ctSqrt(const float &val);
inline double ctSqrt(const double &val);
inline int64_t ctSqrt(const int64_t &val);
inline int32_t ctSqrt(const int32_t &val);
inline int16_t ctSqrt(const int16_t &val);
inline int8_t ctSqrt(const int8_t &val);

#include "ctMathHelpers.inl"

#endif // tmlHelpers_h__
