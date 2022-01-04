
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

#ifndef _atString_h__
#define _atString_h__

#include "ctIterator.h"
#include "ctVector.h"
#include <string>

enum atStringCompareOptions
{
  atSCO_None,
  atSCO_MatchCase,
};

class ctString
{
public:
  typedef char* iterator;
  typedef const char* const_iterator;

  ctString();
  ctString(char c);
  ctString(char *str);
  ctString(const char *str);
  ctString(const ctString &copy);
  ctString(const std::string &path);
  ctString(ctString &&move);
  ctString(const ctVector<char> &str);
  ctString(ctVector<char> &&move);
  ctString(char *pStart, char *pEnd);
  ctString(const char *pStart, const char *pEnd);
  ctString(const ctVector<uint8_t> &str);
  template<typename T> explicit ctString(const T &o);

  explicit operator ctVector<uint8_t>() const;
  explicit operator std::string() const;

  // implicit conversion to c-string
  operator const char* () const;

  static ctString _to_lower(const char *str);
  static ctString _to_upper(const char *str);

  ctString to_lower() const;
  ctString to_upper() const;

  //******************
  // Compare functions
  bool compare(const char *str, const atStringCompareOptions options = atSCO_MatchCase) const;
  static bool compare(const char *lhs, const char *rhs, const atStringCompareOptions options = atSCO_MatchCase, const int64_t &compareLen = -1);

  //******************
  // Replace functions

  static ctString _replace(const char *str, const char _char, const char with, const int64_t start = 0, int64_t count = -1);
  static ctString _replace(const char *str, const char* find, const char* with, const int64_t start = 0, int64_t count = -1);
  ctString replace(const char _char, const char with, const int64_t start = 0, int64_t count = -1) const;
  ctString replace(const char* str, const char* with, const int64_t start = 0, int64_t count = -1) const;
  void append(const char* str);
  void append(const char _char);

  ctString substr(int64_t start, int64_t end) const;
  ctString substr(int64_t count) const;

  //***************
  // Static Find functions
  // Returns the index of the char/substring
  // Returns -1 if not found 

  static int64_t _find(const char *str, const char _char, int64_t start = 0, int64_t end = INT64_MAX);
  static int64_t _find(const char *str, const char *find, int64_t start = 0, int64_t end = INT64_MAX);
  static int64_t _find_end(const char *str, const char *find, int64_t start = 0, int64_t end = INT64_MAX);
  static int64_t _find_reverse(const char *str, const char _char, int64_t start = 0, int64_t end = INT64_MAX);
  static int64_t _find_reverse(const char *str, const char* find, int64_t start = 0, int64_t end = INT64_MAX);
  static int64_t _find_first_not(const char *str, const char _char, int64_t start = 0, int64_t end = INT64_MAX);
  static int64_t _find_first_not(const char *str, const char* find, int64_t start = 0, int64_t end = INT64_MAX);
  static int64_t _find_last_not(const char *str, const char _char, int64_t start = 0, int64_t end = INT64_MAX);
  static int64_t _find_last_not(const char *str, const char* find, int64_t start = 0, int64_t end = INT64_MAX);
  static int64_t _find_first_of(const char *str, const char _char, int64_t start = 0, int64_t end = INT64_MAX);
  static int64_t _find_first_of(const char *str, const char *set, int64_t start = 0, int64_t end = INT64_MAX);
  static int64_t _find_last_of(const char *str, const char _char, int64_t start = 0, int64_t end = INT64_MAX);
  static int64_t _find_last_of(const char *str, const char* find, int64_t start = 0, int64_t end = INT64_MAX);
  static int64_t _find_first(const char *str, const char _char);
  static int64_t _find_first(const char *str, const char* find);
  static int64_t _find_last(const char *str, const char _char);
  static int64_t _find_last(const char *str, const char* find);
  static bool _starts_with(const char *str, const char *find);
  static bool _starts_with(const char *str, const char _char);
  static bool _ends_with(const char *str, const char *find);
  static bool _ends_with(const char *str, const char _char);

  //***************
  // Find functions
  // Returns the index of the char/substring
  // Returns -1 if not found 

  int64_t find(const char _char, int64_t start = 0, int64_t end = INT64_MAX) const;
  int64_t find(const char *str, int64_t start = 0, int64_t end = INT64_MAX) const;
  int64_t find_end(const  char *str, int64_t start = 0, int64_t end = INT64_MAX);
  int64_t find_reverse(const char _char, int64_t start = 0, int64_t end = INT64_MAX) const;
  int64_t find_reverse(const char *str, int64_t start = 0, int64_t end = INT64_MAX) const;
  int64_t find_first_not(const char _char, int64_t start = 0, int64_t end = INT64_MAX) const;
  int64_t find_first_not(const char *str, int64_t start = 0, int64_t end = INT64_MAX) const;
  int64_t find_last_not(const char _char, int64_t start = 0, int64_t end = INT64_MAX) const;
  int64_t find_last_not(const char *str, int64_t start = 0, int64_t end = INT64_MAX) const;
  int64_t find_first_of(const char _char, int64_t start = 0, int64_t end = INT64_MAX) const;
  int64_t find_first_of(const char *set, int64_t start = 0, int64_t end = INT64_MAX) const;
  int64_t find_last_of(const char _char, int64_t start = 0, int64_t end = INT64_MAX) const;
  int64_t find_last_of(const char *str, int64_t start = 0, int64_t end = INT64_MAX) const;
  int64_t find_first(const char _char) const;
  int64_t find_first(const char *str) const;
  int64_t find_last(const char _char) const;
  int64_t find_last(const char *str) const;
  bool starts_with(const char *str) const;
  bool ends_with(const char *str) const;
  bool ends_with(const char str) const;

  ctString trim(const char *characters = ctString::Whitespace()) const;
  ctString trim_start(const char *characters = ctString::Whitespace()) const;
  ctString trim_end(const char *characters = ctString::Whitespace()) const;

  static ctVector<ctString> _split(const char *src, const char &_char, const bool dropEmpty = true);
  static ctVector<ctString> _split(const char *src, const char *split, const bool isSet = false, const bool dropEmpty = true);
  ctVector<ctString> split(const char &_char, const bool dropEmpty = true) const;
  ctVector<ctString> split(const char *split, bool isSet = false, const bool dropEmpty = true) const;
  
  static ctString join(const ctVector<ctString> &strings, const ctString &separator, const bool ignoreEmpty = true);

  const char* c_str() const;
  int64_t capacity()  const;
  int64_t length() const;

  ctVector<char> &vector();
  const ctVector<char> &vector() const;

  void set_string(const ctVector<char> &data);
  void set_string(const char* str, const int64_t len);

  char& operator[](int64_t index);
  const char& operator[](int64_t index) const;

  bool operator==(const char *rhs) const;
  bool operator!=(const char *rhs) const;

  const ctString& operator=(const ctString &str);
  const ctString& operator=(ctString &&str);
  const ctString& operator=(const char *rhs);
  const ctString& operator=(const char rhs);
  ctString operator+=(const ctString &rhs);
  ctString operator+=(const char *rhs);
  ctString operator+=(const char rhs);
  ctString operator+=(const ctVector<char> &rhs);
  ctString operator+(const char *rhs) const;
  ctString operator+(const char rhs) const;
  ctString operator+(const ctVector<char> &rhs) const;
  ctString operator+(const ctString &str) const;

  template<typename T> inline ctString operator+(const T &rhs) const;
  template<typename T> inline ctString operator+=(const T &rhs);

  iterator begin();
  iterator end();
  const_iterator begin() const;
  const_iterator end() const;

  static const char* Integer();
  static const char* Decimal();
  static const char* Whitespace();
  static const char* Hex();
  static const char* Binary();
  static const char* AlphabetUpper();
  static const char* AlphabetLower();
  static const char* AlphabetAll();

protected:
  void validate();

  ctVector<char> m_data;
};

ctString operator+(const char _char, const ctString &rhs);
ctString operator+(const char *lhs, const ctString &rhs);
ctString operator+(char _char, const ctString &rhs);
ctString operator+(char *lhs, const ctString &rhs);

template<> inline ctTypeDesc ctGetTypeDesc(const ctString &str);
template<typename T> inline T ctFromString(const ctString &str)
{
  static_assert(!std::is_same<T, T>::value, "ctFromString is not defined for this type.");
  return T();
}

template<typename T> ctString ctToString(const T &o)
{
  static_assert(!std::is_same<T, T>::value, "ctToString is not defined for type T");
  return ctString();
}

#include "ctFromString.h"
#include "ctToString.h"

int64_t ctStreamRead(ctReadStream *pStream, ctString *pData, const int64_t count);
int64_t ctStreamWrite(ctWriteStream *pStream, const ctString *pData, const int64_t count);

#include "ctString.inl"
#endif
