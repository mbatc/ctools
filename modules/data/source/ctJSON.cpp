#include "ctJSON.h"
#include "ctScan.h"

static const ctString _delimterSet = ctString("[]{},:");
static const ctString _delimiterAndWhitespaceSet = ctString::Whitespace() + _delimterSet;

static ctString _ParseString(const char **pJson, int64_t *pLength);
static ctJSON _ParseObject(const char **pJson, int64_t *pLength);
static ctJSON _ParseArray(const char **pJson, int64_t *pLength);
static ctJSON _ParseValue(const char **pJson, int64_t *pLength);

ctJSON::ctJSON(ctJSON &&o) { *this = std::move(o); }
ctJSON::ctJSON(const ctJSON &o) { *this = o; }
ctJSON::ctJSON(const ctString &json) { Parse(json); }
ctJSON::~ctJSON() { MakeNull(); }

const ctJSON& ctJSON::operator=(ctJSON &&o)
{
  m_isString = o.m_isString;
  m_pValue = o.m_pValue;
  m_pArray = o.m_pArray;
  m_pObject = o.m_pObject;
  o.m_pArray = nullptr;
  o.m_pValue = nullptr;
  o.m_pObject = nullptr;
  return *this;
}

const ctJSON& ctJSON::operator=(const ctJSON &o)
{
  if (o.IsObject())
  {
    MakeObject();
    *m_pObject = *o.m_pObject;
  }

  if (o.IsArray())
  {
    MakeArray();
    *m_pArray = *o.m_pArray;
  }

  if (o.IsValue() || o.IsString())
  {
    MakeValue(o.IsString());
    *m_pValue = *o.m_pValue;
  }

  return *this;
}

ctString ctJSON::ToString(const bool prettyPrint) const
{
  if (IsNull()) return "null";

  ctString ret;
  if (IsObject() || IsArray())
  {
    ret += IsArray() ? "[" : "{";
    bool useNewLines = false;
    ctVector<ctString> pairs;
    if (IsObject())
    {
      useNewLines = true;
      for (auto &kvp : *m_pObject)
        pairs.push_back("\"" + kvp.m_key + (prettyPrint ? "\": " : "\":") + kvp.m_val.ToString(prettyPrint));
    }

    if (IsArray())
    {
      for (auto &val : *m_pArray)
      {
        useNewLines |= val.IsObject() || val.IsArray();
        pairs.push_back(val.ToString(prettyPrint));
      }
    }
    ctString valueString = ctString::join(pairs, prettyPrint ? (useNewLines ? ",\n" : ", ") : ",");
    if (prettyPrint && useNewLines)
      valueString = "\n  " + valueString.replace("\n", "\n  ") + "\n";

    ret += valueString;
    ret += IsArray() ? "]" : "}";
  }
  else if (IsString())
  {
    ret = "\"" + *m_pValue + "\"";
  }
  else
  {
    return *m_pValue;
  }

  return ret;
}

void ctJSON::MakeNull()
{
  if (m_pValue) ctDelete(m_pValue);
  if (m_pObject) ctDelete(m_pObject);
  if (m_pArray) ctDelete(m_pArray);
  m_pValue = nullptr;
  m_pObject = nullptr;
  m_pArray = nullptr;
  m_isString = false;
}

void ctJSON::MakeArray()
{
  if (IsArray())
    return;
  MakeNull();
  m_pArray = ctNew(ctVector<ctJSON>);
}

void ctJSON::MakeObject()
{
  if (IsObject())
    return;
  MakeNull();
  m_pObject = ctNew(ctHashMap<ctString, ctJSON>);
}

void ctJSON::MakeValue(const bool &isString)
{
  if (!(IsValue() || IsString()))
  {
    MakeNull();
    m_pValue = ctNew(ctString);
  }
  m_isString = isString;
}

int64_t ctJSON::Parse(const char *json, const int64_t &length)
{
  if (length == 0)
    return 0;
  int64_t len = length;
  *this = _ParseValue(&json, &len);
  return length - len;
}

void ctJSON::SetElement(const int64_t &index, const ctJSON &value)
{
  MakeArray();
  GetElement(index) = value;
}

void ctJSON::SetMember(const ctString &key, const ctJSON &value)
{
  MakeObject();
  GetMember(key) = value;
}

int64_t ctJSON::ElementCount() const
{
  if (IsNull() || IsValue() || IsString())
    return 0;
  if (IsObject())
    return m_pObject->Size();
  return m_pArray->size();
}

ctJSON& ctJSON::GetMember(const ctString &key)
{
  MakeObject();
  ctJSON *pMember = TryGetMember(key);
  if (!pMember)
  {
    m_pObject->Add(key, ctJSON());
    return *m_pObject->TryGet(key);
  }
  return *pMember;
}

ctJSON& ctJSON::GetElement(const int64_t &index)
{
  MakeArray();
  if (m_pArray->size() <= index)
    m_pArray->resize(index + 1, ctJSON());
  return m_pArray->at(index);
}

void ctJSON::SetBool(const bool &val, const bool verbose) { SetValue(ctToString(val, verbose), false); }
void ctJSON::SetInt(const int &val) { SetValue(val, false); }
void ctJSON::SetDecimal(const float &val) { SetValue(val, false); }
void ctJSON::SetDecimal(const double &val) { SetValue(val, false); }

int64_t ctJSON::ToInt() const { return GetValueAs<int64_t>(); }
float ctJSON::ToFloat() const { return GetValueAs<float>(); }
double ctJSON::ToDouble() const { return GetValueAs<double>(); }
bool ctJSON::ToBool() const { return GetValueAs<bool>(); }

bool ctJSON::IsNull() const { return !(IsObject() || IsString() || IsValue() || IsArray()); }
bool ctJSON::IsArray() const { return m_pArray != nullptr; }
bool ctJSON::IsObject() const { return m_pObject != nullptr; }
bool ctJSON::IsString() const { return m_pValue != nullptr && m_isString; }
bool ctJSON::IsValue() const { return m_pValue != nullptr && !m_isString; }

ctJSON* ctJSON::TryGetMember(const ctString &key) const { return IsObject() ? m_pObject->TryGet(key) : nullptr; }
ctJSON* ctJSON::TryGetElement(const int64_t &index) const { return IsArray() ? &m_pArray->at(index) : nullptr; }

ctVector<ctString> ctJSON::GetKeys() const { return IsObject() ? m_pObject->GetKeys() : ctVector<ctString>{}; }

const ctString& ctJSON::Value() const { return *m_pValue; }

const ctVector<ctJSON>& ctJSON::Array() const { return *m_pArray; }
const ctHashMap<ctString, ctJSON>& ctJSON::Object() const { return *m_pObject; }

ctJSON& ctJSON::operator[](const ctString &key) { return GetMember(key); }
ctJSON& ctJSON::operator[](const int64_t &index) { return GetElement(index); }

const ctJSON& ctJSON::operator[](const ctString &key) const { return *TryGetMember(key); }
const ctJSON& ctJSON::operator[](const int64_t &index) const { return *TryGetElement(index); }

bool ctJSON::Parse(const ctString &json) { return Parse(json.c_str(), json.length()) == json.length(); }

ctString ctToString(const ctJSON &json) { return json.ToString(); }
ctJSON ctFromString(const ctString &json) { return ctJSON(json); }

static bool _Seek(const char **pJson, int64_t *pLength, int64_t dist)
{
  if (*pLength == 0)
    return false;
  if (dist < 0) dist = *pLength;
  dist = ctMin(*pLength, dist);
  (*pJson) += dist;
  (*pLength) -= dist;
  return *pLength != 0;
}

static bool _SkipDelimiter(const char **pJson, int64_t *pLength) { return _Seek(pJson, pLength, ctString::_find_first_not(*pJson, _delimterSet)); }
static bool _SkipToDelimiter(const char **pJson, int64_t *pLength) { return _Seek(pJson, pLength, ctString::_find_first_of(*pJson, _delimterSet)); }
static bool _SkipWhitespace(const char **pJson, int64_t *pLength) { return _Seek(pJson, pLength, ctString::_find_first_not(*pJson, ctString::Whitespace())); }

static ctString _ParseString(const char **pJson, int64_t *pLength)
{
  ctString ret;
  _SkipWhitespace(pJson, pLength);
  if (**pJson != '"')
    return ret;

  int64_t len = 0;
  ret = ctScan::Quote(pJson, &len, *pLength);
  *pLength -= len;
  return ret;
}

static ctJSON _ParseObject(const char **pJson, int64_t *pLength)
{
  _SkipWhitespace(pJson, pLength);
  ctJSON ret;
  if (**pJson != '{')
    return ret;
  _Seek(pJson, pLength, 1);
  ctString key;
  ctJSON value;

  while (_SkipWhitespace(pJson, pLength))
  {
    key = _ParseString(pJson, pLength);
    while (**pJson != ':' || *pLength == 0)
      if (!_Seek(pJson, pLength, 1) || !_SkipToDelimiter(pJson, pLength))
        return ret;

    _Seek(pJson, pLength, 1);
    value = _ParseValue(pJson, pLength);
    ret.SetMember(key, value); // TODO: Add error reporting if this fails
    _SkipToDelimiter(pJson, pLength);
    if (**pJson == '}')
      break;
    _Seek(pJson, pLength, 1);
  }
  _Seek(pJson, pLength, 1);
  return ret;
}

static ctJSON _ParseArray(const char **pJson, int64_t *pLength)
{
  ctJSON ret;
  if (**pJson != '[')
    return ret;
  _Seek(pJson, pLength, 1);
  ctJSON value;
  int64_t arrayLen = 0;
  while (_SkipWhitespace(pJson, pLength))
  {
    value = _ParseValue(pJson, pLength);
    ret.SetElement(arrayLen, value);
    arrayLen++;

    _SkipToDelimiter(pJson, pLength);
    if (**pJson == ']')
      break;
    _Seek(pJson, pLength, ctString::_find_first(*pJson, ',') + 1);
  }
  _Seek(pJson, pLength, 1);
  return ret;
}

static ctJSON _ParseValue(const char **pJson, int64_t *pLength)
{
  _SkipWhitespace(pJson, pLength);
  ctJSON ret;
  switch (**pJson)
  {
  case '{': return _ParseObject(pJson, pLength);
  case '[': return _ParseArray(pJson, pLength);
  case '"': ret.SetValue(_ParseString(pJson, pLength)); break;
  default:
    const char *start = *pJson;
    _Seek(pJson, pLength, ctString::_find_first_of(*pJson, _delimiterAndWhitespaceSet));
    ret.SetValue(ctString(start, *pJson), false);
  }
  return ret;
}