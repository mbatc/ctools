#include "ctString.h"
#include "ctScan.h"
#include <codecvt>

template<> std::wstring ctFromString<std::wstring>(const ctString &str) { std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter; return converter.from_bytes(str.c_str()); }
template<> std::string ctFromString<std::string>(const ctString &str) { return str.c_str(); }
template<> double ctFromString<double>(const ctString &str) { return ctScan::Float(str); }
template<> float ctFromString<float>(const ctString &str) { return (float)ctScan::Float(str); }
template<> int8_t ctFromString<int8_t>(const ctString &str) { return (int8_t)ctScan::Int(str); }
template<> int16_t ctFromString<int16_t>(const ctString &str) { return (int16_t)ctScan::Int(str); }
template<> int32_t ctFromString<int32_t>(const ctString &str) { return (int32_t)ctScan::Int(str); }
template<> int64_t ctFromString<int64_t>(const ctString &str) { return (int64_t)ctScan::Int(str); }
template<> uint8_t ctFromString<uint8_t>(const ctString &str) { return (uint8_t)ctScan::Int(str); }
template<> uint16_t ctFromString<uint16_t>(const ctString &str) { return (uint16_t)ctScan::Int(str); }
template<> uint32_t ctFromString<uint32_t>(const ctString &str) { return (uint32_t)ctScan::Int(str); }
template<> uint64_t ctFromString<uint64_t>(const ctString &str) { return (uint64_t)ctScan::Int(str); }
template<> bool ctFromString<bool>(const ctString &str) { return ctScan::Bool(str); }