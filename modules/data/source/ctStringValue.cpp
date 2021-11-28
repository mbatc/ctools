#include "ctStringValue.h"
#include "ctScan.h"
#include "ctPrint.h"

ctStringValue& ctStringValue::SetValue(const ctString &str, const ctType &type)
{
  m_value = str;
  m_type = type;
  return *this;
}

ctStringValue& ctStringValue::operator=(const ctStringValue &o)
{
  m_value = o.m_value;
  m_type = o.m_type;
  return *this;
}

ctStringValue& ctStringValue::operator=(ctStringValue &&o)
{
  std::swap(m_value, o.m_value);
  std::swap(m_type, o.m_type);
  return *this;
}

ctStringValue::ctStringValue(ctStringValue &&o) { *this = std::move(o); }
ctStringValue::ctStringValue(const ctStringValue &o) { *this = o; }

ctStringValue::ctStringValue(const double &val) { Set(val); }
ctStringValue::ctStringValue(const int64_t &val) { Set(val); }
ctStringValue::ctStringValue(const ctString &val) { Set(val); }
ctStringValue::ctStringValue(const bool &val) { Set(val); }

ctStringValue &ctStringValue::Set(const double &val) { return SetValue(ctPrint::Float(val), ctType_Float64); }
ctStringValue &ctStringValue::Set(const int64_t &val) { return SetValue(ctPrint::Int(val), ctType_Int64); }
ctStringValue &ctStringValue::Set(const ctString &val) { return SetValue(val, ctType_Unknown); }
ctStringValue &ctStringValue::Set(const bool &val) { return SetValue(ctPrint::Bool(val), ctType_Uint8); }

ctStringValue &ctStringValue::Parse(const ctString &value)
{
  if (value.find_first_not("-0123456789") == -1)
  {
    Set(ctScan::Int(value));
  }
  else if (value.find_first_not("-.0123456789") == -1)
  {
    Set(ctScan::Float(value));
  }
  else
  {
    int boolValue = value.compare("true", atSCO_None) ? 1 : (value.compare("false", atSCO_None) ? 0 : -1);
    switch (boolValue)
    {
    case 0: Set(false);
    case 1: Set(true);
    case -1: Set(value);
    }
  }

  return *this;
}

bool ctStringValue::IsEmpty() const { return m_value.length() == 0; }
bool ctStringValue::AsBool() const { return ctScan::Bool(m_value); }
double ctStringValue::AsFloat() const { return ctScan::Float(m_value); }
int64_t ctStringValue::AsInt() const { return ctScan::Int(m_value); }

const ctString &ctStringValue::AsString() const { return m_value; }

ctStringValue::operator bool() const { return AsBool(); }
ctStringValue::operator double() const { return AsFloat(); }
ctStringValue::operator int64_t() const { return AsInt(); }
ctStringValue::operator ctString() const { return AsString(); }

const ctType &ctStringValue::GetType() const { return m_type; }

ctStringValue &ctStringValue::operator=(const ctString &value) { return Set(value); }
ctStringValue &ctStringValue::operator=(const bool &value) { return Set(value); }
ctStringValue &ctStringValue::operator=(const int64_t &value) { return Set(value); }
ctStringValue &ctStringValue::operator=(const double &value) { return Set(value); }

ctString ctToString(const ctStringValue &o) { return o.m_value; }
