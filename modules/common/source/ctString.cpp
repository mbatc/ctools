
// -----------------------------------------------------------------------------
// The MIT License
// 
// Copyright(c) 2020 Michael Batchelor, 
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
// -----------------------------------------------------------------------------

#include "ctString.h"
#include <string.h>
#include <codecvt>

char _ToLower(const char c) { return c >= 65 && c <= 90 ? c + 32 : c; }
char _ToUpper(const char c) { return c >= 97 && c <= 122 ? c - 32 : c; }

ctString::ctString(ctVector<char> &&move)
{
  m_data.swap(move);
  validate();
}

ctString::ctString(char *pStart, char *pEnd) : ctString((const char*)pStart, (const char*)pEnd) {}

ctString::ctString(const char *pStart, const char *pEnd)
{
  m_data.assign(pStart, pEnd);
  validate();
}

ctString ctString::_to_lower(const char *str)
{
  ctVector<char> data;
  while (*str != 0)
    data.push_back(_ToLower(*str++));
  return ctString(data);
}

ctString ctString::_to_upper(const char *str)
{
  ctVector<char> data;
  while (*str != 0)
    data.push_back(_ToUpper(*str++));
  return ctString(data);
}

ctString ctString::_replace(const char *str, const char _char, const char with, const int64_t start, int64_t count)
{
  ctString ret = str;
  int64_t pos = start;
  while ((pos = ret.find(_char, pos)) >= 0 && count != 0)
  {
    ret[pos++] = with;
    --count;
  }
  return ret;
}

ctString ctString::_replace(const char *str, const char *find, const char *with, const int64_t start, int64_t count)
{
  if (_find(str, find, start) < 0)
    return str;

  ctString ret;
  ret.vector().insert(0, str, str + start);

  const int64_t matchLen = strlen(find);
  int64_t pos = start;
  int64_t found = 0;
  while ((found = _find(str, find, pos)) >= 0 && count != 0)
  {
    ret.vector().insert(ret.vector().size() - 1, str + pos, str + found);
    ret.append(with);
    pos = found + matchLen;
    --count;
  }
  ret.append(str + pos);
  return ret;
}

void ctString::append(const char *str)
{
  if (!str)
    return;
  m_data.insert(length(), str, str + strlen(str));
}

ctString ctString::substr(int64_t start, int64_t end) const
{
  if (end >= 0 && end <= start)
    return "";
  start = ctMax(0, start);
  ctVector<char> subData;
  int64_t len = (end < 0 ? length() : end) - start;
  subData.reserve(len);
  subData.assign(m_data.begin() + start, m_data.begin() + start + len);
  subData.push_back(0);
  return std::move(subData);
}

ctString ctString::substr(const int64_t count) const { return substr(0, count); }

int64_t ctString::_find(const char *str, const char _char, int64_t start, int64_t end)
{
  start = ctMax(start, 0);
  while (*(str + start) != 0 && start < end)
    if (*(str + start++) == _char)
      return start - 1;
  return CT_INVALID_INDEX;
}

int64_t ctString::_find(const char *str, const char *find, int64_t start, int64_t end)
{
  start = ctMax(start, 0);
  while (*(str + start) != 0 && start < end)
  {
    const char *s = str + start;
    const char *f = find;
    while (*f++ == *s++)
      if (*f == 0)
        return start;
      else if (*s == 0)
        return CT_INVALID_INDEX;
    ++start;
  }
  return CT_INVALID_INDEX;
}

int64_t ctString::_find_end(const char *str, const char *find, int64_t start, int64_t end)
{
  int64_t found = _find(str, find, start, end);
  return found >= 0 ? found + strlen(find) : found;
}

int64_t ctString::_find_reverse(const char *str, const char _char, int64_t start, int64_t end)
{
  int64_t len = strlen(str);
  start = ctMin(ctMax(start, 0), len);
  end = ctMin(end, len - 1);
  for (int64_t i = end; i >= start; --i)
    if (str[i] == _char)
      return i;
  return CT_INVALID_INDEX;
}

int64_t ctString::_find_reverse(const char *str, const char *find, int64_t start, int64_t end)
{
  int64_t ssLen = 0;
  if (!find || (ssLen = strlen(find)) == 0)
    return CT_INVALID_INDEX;

  int64_t len = strlen(str);
  start = ctMin(ctMax(start, 0), len);
  end = ctMin(end, len);

  for (int64_t i = end - ssLen; i >= start; --i)
    if (str[i] == find[0] && str[i + ssLen - 1] == find[ssLen - 1])
    {
      bool isMatch = true;
      for (int64_t j = 1; isMatch && j < ssLen - 1; ++j)
        if (str[i + j] != find[j])
          isMatch = false;
      if (isMatch)
        return i;
    }
  return CT_INVALID_INDEX;
}

int64_t ctString::_find_first_not(const char *str, const char _char, int64_t start, int64_t end)
{
  start = ctMax(start, 0);
  while (*(str + start) != 0 && start < end)
    if (str[start++] != _char)
      return start - 1;
  return CT_INVALID_INDEX;
}

int64_t ctString::_find_first_not(const char *str, const char *find, int64_t start, int64_t end)
{
  start = ctMax(start, 0);
  int64_t nChecks = strlen(find);
  bool found = false;
  while (str[start] != 0 && start < end)
  {
    const char *f = find;
    bool found = false;
    while (*f != 0 && !found)
      found = *f++ == str[start];
    if (!found)
      return start;
    ++start;
  }

  return CT_INVALID_INDEX;
}

int64_t ctString::_find_last_not(const char *str, const char _char, int64_t start, int64_t end)
{
  start = ctMax(start, 0);
  end = ctMin(end, strlen(str) - 1);
  for (int64_t i = end; i >= start; --i)
    if (str[i] != _char)
      return i;
  return CT_INVALID_INDEX;
}

int64_t ctString::_find_last_not(const char *str, const char *find, int64_t start, int64_t end)
{
  start = ctMax(start, 0);
  end = ctMin(end, strlen(str));

  int64_t nChecks = strlen(find);
  bool found = false;
  for (int64_t i = end - 1; i >= start; --i, found = false)
  {
    for (int64_t j = 0; j < nChecks && !found; ++j)
      found |= str[i] == find[j];
    if (!found)
      return i;
  }
  return CT_INVALID_INDEX;
}

int64_t ctString::_find_first_of(const char *str, const char _char, int64_t start, int64_t end)
{
  start = ctMax(start, 0);
  while (str[start] != 0 && start < end)
    if (str[start++] == _char)
      return start - 1;
  return CT_INVALID_INDEX;
}

int64_t ctString::_find_first_of(const char *str, const char *set, int64_t start, int64_t end)
{
  start = ctMax(start, 0);
  while (str[start] != 0 && start < end)
  {
    const char *s = set;
    while (*s != 0)
      if (str[start] == *s++)
        return start;
    start++;
  }
  return CT_INVALID_INDEX;
}

int64_t ctString::_find_last_of(const char *str, const char _char, int64_t start, int64_t end)
{
  int64_t len = strlen(str);
  start = ctMax(start, 0);
  end = ctMin(end, len);

  for (int64_t i = end - 1; i >= start; --i)
    if (str[i] == _char)
      return i;
  return CT_INVALID_INDEX;
}

int64_t ctString::_find_last_of(const char *str, const char *find, int64_t start, int64_t end)
{
  int64_t len = strlen(str);
  start = ctMin(ctMax(start, 0), len);
  end = ctMin(end, len);

  int64_t nChecks = strlen(find);
  for (int64_t i = end - 1; i >= start; --i)
    for (int64_t j = 0; j < nChecks; ++j)
      if (str[i] == find[j])
        return i;
  return CT_INVALID_INDEX;
}

void ctString::set_string(const char *str, const int64_t len)
{
  m_data.clear();
  m_data.reserve(len + 1);
  m_data.resize(len);
  memcpy(m_data.data(), str, len);
  validate();
}

void ctString::validate()
{
  if (m_data.size() == 0 || m_data.back() != '\0')
    m_data.push_back(0); // make sure there is a null terminating character
}

ctVector<ctString> ctString::_split(const char *src, const char &_char, const bool dropEmpty)
{
  ctVector<ctString> ret;
  ctString atStr;
  int64_t start = 0;
  int64_t end = _find(src, _char);
  while (end != CT_INVALID_INDEX)
  {
    atStr.set_string(src + start, end - start);
    start = end + 1;
    end = _find(src, _char, start);

    if (!dropEmpty || atStr.length() > 0)
      ret.push_back(atStr);
  }
  ret.emplace_back(src + start);
  return ret;
}

ctVector<ctString> ctString::_split(const char *src, const char *split, const bool isSet, const bool dropEmpty)
{
  ctVector<ctString> ret;
  ctString atStr;
  int64_t start = 0;
  int64_t(*find_func)(const char *, const char*, int64_t, int64_t);
  if (isSet)
    find_func = _find_first_of;
  else
    find_func = _find;
  int64_t end = find_func(src, split, 0, INT64_MAX);
  int64_t setLen = isSet ? 1 : strlen(split);
  while (end != -1)
  {
    atStr.set_string(src + start, end - start);
    start = end + setLen;
    end = find_func(src, split, start, INT64_MAX);
    if (!dropEmpty || atStr.length() > 0)
      ret.push_back(atStr);
  }
  atStr = ctString(src + start);
  if (atStr.length())
    ret.emplace_back(atStr);
  return ret;
}

bool ctString::compare(const char *lhs, const char *rhs, const atStringCompareOptions options, const int64_t &compareLen)
{
  if (options == atSCO_MatchCase)
    return !strcmp(lhs, rhs);
  
  char cUpper = 0;
  char cLower = 0;
  const char *start = lhs;
  while (*lhs != 0 && *rhs != 0)
  {
    cUpper = _ToUpper(*rhs);
    cLower = _ToLower(*rhs);
    if (*lhs != cUpper && *lhs != cLower)
      return false;

    if (lhs - start == compareLen)
      return true;
    ++lhs;
    ++rhs;
  }

  return *lhs == 0 && *rhs == 0;
}

template<> ctTypeDesc ctGetTypeDesc(const ctString &str)
{
  ctTypeDesc ret = ctGetTypeDesc<char>();
  ret.count = str.length() + 1;
  ret.size = ctSize(ret.type) * ret.count;
  return ret;
}

ctString ctString::join(const ctVector<ctString> &strings, const ctString &separator, const bool ignoreEmpty)
{
  ctString ret;
  for (const ctString &str : strings)
    if (!ignoreEmpty || str.length() > 0)
    {
      ret += str;
      if (&str - &strings.back())
        ret += separator;
    }
  return ret;
}

ctString operator+(const char &_char, const ctString &rhs) { return ctString(_char).operator+(rhs); }
ctString operator+(const char *lhs, const ctString &rhs) { return ctString(lhs).operator+(rhs); }
ctString operator+(char _char, const ctString &rhs) { return ctString(_char).operator+(rhs); }
ctString operator+(char *lhs, const ctString &rhs) { return ctString(lhs).operator+(rhs); }

ctString::operator std::string() const { return std::string(c_str(), length() + 1); }
ctString::operator ctVector<uint8_t>() const { return ctVector<uint8_t>((uint8_t*)m_data.begin(), m_data.size() - 1); }
ctString::ctString(const ctVector<uint8_t> &data) { set_string((const char*)data.data(), data.size()); }

bool ctString::compare(const char *str, const atStringCompareOptions options) const { return compare(c_str(), str, options); }
ctString ctString::to_lower() const { return _to_lower(c_str()); }
ctString ctString::to_upper() const { return _to_upper(c_str()); }
ctString::ctString() { set_string("", 0); }
ctString::ctString(char c) { m_data.assign(c, 1); validate(); }
ctString::ctString(char *str) { set_string(str, strlen(str)); }
ctString::ctString(const char *str) : ctString((char*)str) {}
ctString::ctString(const ctString &copy) { set_string(copy.m_data); }
ctString::ctString(const std::string &path) { set_string(path.c_str(), path.length()); }
ctString::ctString(ctString &&move) { m_data.swap(move.m_data); }
ctString::ctString(const ctVector<char> &str) { set_string(str.data(), str.size()); }
void ctString::append(const char _char) { m_data.insert(length(), &_char, &_char + 1); }
ctString ctString::replace(const char _char, const char with, const int64_t start, int64_t count) const { return _replace(c_str(), _char, with, start, count); }
ctString ctString::replace(const char *str, const char *with, const int64_t start, int64_t count) const { return _replace(c_str(), str, with, start, count); }
int64_t ctString::_find_first(const char *str, const char _char) { return _find(str, _char, 0); }
int64_t ctString::_find_first(const char *str, const char *find) { return _find(str, find, 0); }
int64_t ctString::_find_last(const char *str, const char _char) { return _find_reverse(str, _char); }
int64_t ctString::_find_last(const char *str, const char *find) { return _find_reverse(str, find); }
int64_t ctString::find(const char _char, int64_t start, int64_t end) const { return _find(c_str(), _char, start, end); }
int64_t ctString::find(const char *str, int64_t start, int64_t end) const { return _find(c_str(), str, start, end); }
int64_t ctString::find_end(const char *str, int64_t start, int64_t end) { return _find_end(c_str(), str, start, end); }
int64_t ctString::find_reverse(const char _char, int64_t start, int64_t end) const { return _find_reverse(c_str(), _char, start, end); }
int64_t ctString::find_reverse(const char *str, int64_t start, int64_t end) const { return _find_reverse(c_str(), str, start, end); }
int64_t ctString::find_first_not(const char _char, int64_t start, int64_t end) const { return _find_first_not(c_str(), _char, start, end); }
int64_t ctString::find_first_not(const char *str, int64_t start, int64_t end) const { return _find_first_not(c_str(), str, start, end); }
int64_t ctString::find_last_not(const char _char, int64_t start, int64_t end) const { return _find_last_not(c_str(), _char, start, end); }
int64_t ctString::find_last_not(const char *str, int64_t start, int64_t end) const { return _find_last_not(c_str(), str, start, end); }
int64_t ctString::find_first_of(const char _char, int64_t start, int64_t end) const { return _find_first_of(c_str(), _char, start, end); }
int64_t ctString::find_first_of(const char *set, int64_t start, int64_t end) const { return _find_first_of(c_str(), set, start, end); }
int64_t ctString::find_last_of(const char _char, int64_t start, int64_t end) const { return _find_last_of(c_str(), _char, start, end); }
int64_t ctString::find_last_of(const char *str, int64_t start, int64_t end) const { return _find_last_of(c_str(), str, start, end); }
int64_t ctString::find_first(const char _char) const { return find(_char, 0); }
int64_t ctString::find_first(const char *str) const { return find(str, 0); }
int64_t ctString::find_last(const char _char) const { return find_reverse(_char); }
int64_t ctString::find_last(const char *str) const { return find_reverse(str); }
bool ctString::starts_with(const char *str) const { return _starts_with(c_str(), str); }

ctString ctString::trim(const char *characters) const
{
  int64_t start = find_first_not(characters);
  if (start == -1)
    return "";

  int64_t end   = find_last_not(characters);
  return substr(start, end + 1);
}

ctString ctString::trim_start(const char *characters) const
{
  int64_t start = find_first_not(characters);
  return substr(start, -1);
}

ctString ctString::trim_end(const char *characters) const
{
  int64_t end = ctMax(0, find_last_not(characters));
  return substr(0, end + 1);
}

ctVector<ctString> ctString::split(const char & _char, const bool dropEmpty) const { return _split(m_data.data(), _char, dropEmpty); }
ctVector<ctString> ctString::split(const char * split, bool isSet, const bool dropEmpty) const { return _split(m_data.data(), split, isSet, dropEmpty); }
const char *ctString::c_str() const { return m_data.data(); }
int64_t ctString::capacity()  const { return m_data.capacity(); }
int64_t ctString::length() const { return m_data.size() - 1; }
ctVector<char> &ctString::vector() { return m_data; }
const ctVector<char> &ctString::vector() const { return m_data; }
void ctString::set_string(const ctVector<char> &data) { set_string(data.data(), data.size()); }
char& ctString::operator[](int64_t index) { return m_data.at(index); }
const char& ctString::operator[](int64_t index) const { return m_data.at(index); }
ctString::operator const char* () const { return c_str(); }
bool ctString::operator==(const char *rhs) const { return compare(rhs); }
bool ctString::operator!=(const char *rhs) const { return !compare(rhs); }
const ctString& ctString::operator=(const ctString &str) { return *this = str.c_str(); }
const ctString& ctString::operator=(ctString &&str) { m_data.swap(str.m_data); return *this; }
const ctString& ctString::operator=(const char *rhs) { set_string(rhs, strlen(rhs)); return *this; }
const ctString& ctString::operator=(const char rhs) { set_string({ rhs, '\0' }); return *this; }
ctString ctString::operator+=(const ctString &rhs) { append(rhs);  return *this; }
ctString ctString::operator+=(const char *rhs) { append(rhs); return *this; }
ctString ctString::operator+=(const char rhs) { append(rhs); return *this; }
ctString ctString::operator+=(const ctVector<char> &rhs) { append(ctString(rhs).c_str()); return *this; }
ctString ctString::operator+(const char *rhs) const { ctString ret(*this); return ret += rhs; }
ctString ctString::operator+(const char rhs) const { ctString ret(*this); return ret += rhs; }
ctString ctString::operator+(const ctVector<char> &rhs) const { ctString ret(*this); return ret += rhs; }
ctString ctString::operator+(const ctString &str) const { ctString ret(*this); return ret += str; }
typename ctString::iterator ctString::begin() { return m_data.begin(); }
typename ctString::iterator ctString::end() { return m_data.end() - 1; }
typename ctString::const_iterator ctString::begin() const { return m_data.begin(); }
typename ctString::const_iterator ctString::end() const { return m_data.end() - 1; }
const char* ctString::Integer() { return "0123456789"; }
const char* ctString::Decimal() { return "0123456789."; }
const char* ctString::Whitespace() { return " \n\t\r"; }
const char* ctString::Hex() { return "0x0123456789ABCDEF"; }
const char* ctString::Binary() { return "01"; }
const char* ctString::AlphabetUpper() { return "ABCDEFGHIJKLMNOPQRSTUVWXYZ"; }
const char* ctString::AlphabetLower() { return "abcdefghijklmnopqrstuvwxyz"; }
const char* ctString::AlphabetAll() { return "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"; }

int64_t ctStreamRead(ctReadStream *pStream, ctString *pData, const int64_t count)
{
  ctVector<char> bytes;
  int64_t ret = 0;
  for (ctString &str : ctIterate(pData, count))
  {
    ret += ctStreamRead(pStream, &bytes, 1);
    str.set_string(bytes);
  }
  return ret;
}

int64_t ctStreamWrite(ctWriteStream *pStream, const ctString *pData, const int64_t count)
{
  int64_t ret = 0;
  for (const ctString &str : ctIterate(pData, count))
    ret += ctStreamWrite(pStream, &str.vector(), 1);
  return ret;
}

bool ctString::_starts_with(const char *str, const char *find)
{
  while (*find != 0)
    if (*str == 0 || *str++ != *find++)
      return false;
  return true;
}

bool ctString::_starts_with(const char *str, const char _char) { return str[0] == _char; }

bool ctString::ends_with(const char *str) const
{
  int64_t len = strlen(str);
  if (length() < len)
    return false;

  char const * start = end() - len;
  for (int64_t i = 0; i < len; ++i) {
    if (start[i] != str[i]) {
      return false;
    }
  }

  return true;
}

bool ctString::ends_with(const char str) const { return length() > 0 && m_data[m_data.size() - 1] == str; }
