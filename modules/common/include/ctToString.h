#ifndef atToString_h__
#define atToString_h__

class ctString;

ctString ctToString(const std::wstring &str);
ctString ctToString(const std::string &str);
ctString ctToString(const int8_t val);
ctString ctToString(const int16_t val);
ctString ctToString(const int64_t val);
ctString ctToString(const int32_t val);
ctString ctToString(const uint8_t val);
ctString ctToString(const uint16_t val);
ctString ctToString(const uint64_t val);
ctString ctToString(const uint32_t val);
ctString ctToString(const double val);
ctString ctToString(const float val);
ctString ctToString(const bool val, const bool verbose = true);
ctString ctToString(const ctString &str);

#endif // atToString_h__
