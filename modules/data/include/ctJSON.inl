
template<typename T> inline void ctJSON::SetValue(const T &val, const bool &isString)
{
  MakeValue(isString);
  *m_pValue = ctString(val);
}

template<typename T> inline T ctJSON::GetValueAs() const { return IsValue() || IsString() ? ctFromString<T>(*m_pValue) : T(); }