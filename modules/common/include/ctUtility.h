#ifndef ctUtility_h__
#define ctUtility_h__

#include <utility>

// Compiler specific Function/File/Line definitions
#if _MSC_VER // MS Visual studio / MS build
#define ctMSVC 1
#define ctCOMPILER ctMSVC

#define ctVS2019 5
#define ctVS2017 4
#define ctVS2015 3
#define ctVS2013 2
#define ctVS2012 1
#define ctVS2010 0

// Determine the visual studio IDE version
#if _MSC_VER >= 1920
#define ctMSVC_VER ctVS2019
#define ctIDE "vs2019"
#elif _MSC_VER >= 1910
#define ctMSVC_VER ctVS2019
#define ctIDE "vs2017"
#elif _MSC_VER >= 1900
#define ctMSVC_VER ctVS2015
#define ctIDE "vs2015"
#elif _MSC_VER >= 1800
#define ctMSVC_VER ctVS2013
#define ctIDE "vs2013"
#elif _MSC_VER >= 1700
#define ctMSVC_VER ctVS2012
#define ctIDE "vs2012"
#elif _MSC_VER >= 1600
#define ctMSVC_VER ctVS2010
#define ctIDE "vs2010"
#endif
#endif

#ifdef __GNUC__
#define ctGCC 2
#define ctCOMPILER atGCC
#endif

#ifdef __clang__
#define ctCLANG 3
#define ctCOMPILER atCLANG
#endif

#ifdef __MINGW64__
#define ctMINGW 4
#define ctCOMPILER atMINGW
#endif

#if ctMSVC
#define ctFUNCSIG __FUNCSIG__
#else
#define ctFUNCSIG __PRETTY_FUNCTION__
#endif

#define ctLINE __LINE__
#define ctFILE __FILE__

// OS Defines
#if defined(_WIN32) || defined(_WIN64)
#define ctPLATFORM_WIN32
#elif defined _LINUX_
#define ctPLATFORM_LINUX
#endif

template<typename T> inline T ctMin(const T &a);
template<typename T> inline T ctMax(const T &a);
template<typename T, typename T2, typename... Args> inline T ctMin(const T &first, const T2 &second, Args ...args);
template<typename T, typename T2, typename... Args> inline T ctMax(const T &first, const T2 &second, Args ...args);

template<typename T, typename T2, typename T3> inline T ctClamp(const T &val, const T2 &min, const T3 &max);

#define ctArraySize(val) (sizeof(val) / sizeof(decltype(val[0])))

#include "ctUtility.inl"

#endif // ctUtility_h__
