
template<typename T> inline ctObjectDescriptor::ctObjectDescriptor(const T &o) : ctObjectDescriptor() { ctSerialize(this, &o); }

template<typename T> inline ctObjectDescriptor ctObjectDescriptor::operator=(const T &o) { return Serialize(o); }

template<typename T>
inline ctObjectDescriptor ctObjectDescriptor::Serialize(const T &value)
{
  Clear();
  ctSerialize(this, value);
  return *this;
}

template<typename T>
inline T ctObjectDescriptor::Deserialize() const
{
  uint8_t buffer[sizeof(T)] = { 0 };
  Deserialize((T *)buffer);
  return std::move(*(T*)buffer);
}

template<typename T>
inline void ctObjectDescriptor::Deserialize(T *pValue) const
{
  if (IsNull())
    return;
  ctDeserialize(*this, pValue);
}
