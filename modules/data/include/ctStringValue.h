#ifndef ctStringValue_h__
#define ctStringValue_h__

#include "ctString.h"

class ctStringValue
{
public:
  ctStringValue() = default;
  ctStringValue(ctStringValue &&o);
  ctStringValue(const ctStringValue &o);

  ctStringValue(const double &val);
  ctStringValue(const int64_t &val);
  ctStringValue(const ctString &val);
  ctStringValue(const bool &val);

  ctStringValue& Set(const double &val);
  ctStringValue& Set(const int64_t &val);
  ctStringValue& Set(const ctString &val);
  ctStringValue& Set(const bool &val);

  ctStringValue& Parse(const ctString &value);

  bool IsEmpty() const;

  bool    AsBool() const;
  double  AsFloat() const;
  int64_t AsInt() const;
  const ctString& AsString() const;

  ctStringValue& operator=(ctStringValue &&o);
  ctStringValue& operator=(const ctStringValue &o);

  ctStringValue& operator=(const double &value);
  ctStringValue& operator=(const int64_t &value);
  ctStringValue& operator=(const bool &value);
  ctStringValue& operator=(const ctString &value);

  explicit operator bool() const;
  explicit operator double() const;
  explicit operator int64_t() const;
  explicit operator ctString() const;

  const ctType& GetType() const;

  friend ctString ctToString(const ctStringValue &o);

protected:
  ctStringValue& SetValue(const ctString &str, const ctType &type);

  ctString m_value;
  ctType m_type = ctType_Unknown;
};

#endif // ctStringValue_h__
