
template<typename T> inline T ctMin(const T &a) { return a; }
template<typename T> inline T ctMax(const T &a) { return a; }

template<typename T, typename T2, typename... Args> inline T ctMin(const T &first, const T2 &second, Args ...args)
{
  T argMin = (T)ctMin(second, std::forward<Args>(args)...);
  return (argMin < first) ? argMin : first;
}

template<typename T, typename T2, typename... Args> inline T ctMax(const T &first, const T2 &second, Args ...args)
{
  T argMin = (T)ctMax(second, std::forward<Args>(args)...);
  return (argMin > first) ? argMin : first;
}

template<typename T, typename T2, typename T3> inline T ctClamp(const T &val, const T2 &min, const T3 &max) { return ctMin(max, ctMax(min, val)); }
