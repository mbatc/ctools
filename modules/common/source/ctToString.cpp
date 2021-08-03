#include "ctString.h"
#include <codecvt>
#include <locale>
#include "ctPrint.h"

ctString ctToString(const std::wstring &str) { std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> coverter; return ctString(coverter.to_bytes(str.c_str())); }
ctString ctToString(const std::string &val) { return ctString(val.c_str(), val.c_str() + val.length()); }
ctString ctToString(const int8_t val) { return ctPrint::Int(val); }
ctString ctToString(const int16_t val) { return ctPrint::Int(val); }
ctString ctToString(const int64_t val) { return ctPrint::Int(val); }
ctString ctToString(const int32_t val) { return ctPrint::Int(val); }
ctString ctToString(const uint8_t val) { return ctPrint::Int(val); }
ctString ctToString(const uint16_t val) { return ctPrint::Int(val); }
ctString ctToString(const uint64_t val) { return ctPrint::Int(val); }
ctString ctToString(const uint32_t val) { return ctPrint::Int(val); }
ctString ctToString(const double val) { return ctPrint::Float(val); }
ctString ctToString(const float val) { return ctPrint::Float(val); }
ctString ctToString(const bool val, const bool verbose) { return ctPrint::Bool(val, verbose); }
ctString ctToString(const ctString &str) { return str; }
