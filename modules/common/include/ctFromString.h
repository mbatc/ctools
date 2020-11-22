#ifndef ctFromString_h__
#define ctFromString_h__

class ctString;

template<> std::wstring ctFromString<std::wstring>(const ctString &str);
template<> double ctFromString<double>(const ctString &str);
template<> float ctFromString<float>(const ctString &str);
template<> int8_t ctFromString<int8_t>(const ctString &str);
template<> int16_t ctFromString<int16_t>(const ctString &str);
template<> int32_t ctFromString<int32_t>(const ctString &str);
template<> int64_t ctFromString<int64_t>(const ctString &str);
template<> uint8_t ctFromString<uint8_t>(const ctString &str);
template<> uint16_t ctFromString<uint16_t>(const ctString &str);
template<> uint32_t ctFromString<uint32_t>(const ctString &str);
template<> uint64_t ctFromString<uint64_t>(const ctString &str);
template<> bool ctFromString<bool>(const ctString &str);

#endif // ctFromString_h__
