
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

#ifndef atScan_h__
#define atScan_h__

#include "ctString.h"

class ctScan
{
public:
  ctScan() = delete;

  static bool Bool(const char *str, int64_t *pLen = nullptr, int64_t srclen = -1);
  static bool Bool(const char **pStr, int64_t *pLen = nullptr, const int64_t srcLen = -1);
  static bool Bool(const ctString &str, int64_t *pLen);
  
  static int64_t Int(const char *pStr, int64_t *pLen = nullptr, int64_t srclen = -1);
  static int64_t Int(const char **pStr, int64_t *pLen = nullptr, const int64_t srcLen = -1);
  static int64_t Int(const ctString &str, int64_t *pLen = nullptr);
  
  static int64_t Hex(const char *str, int64_t *pLen = nullptr, int64_t srclen = -1);
  static int64_t Hex(const char **pStr, int64_t *pLen = nullptr, const int64_t srcLen = -1);
  static int64_t Hex(const ctString &str, int64_t *pLen = nullptr);
  
  static double Float(const char *str, int64_t *pLen = nullptr, int64_t srclen = -1);
  static double Float(const char **pStr, int64_t *pLen = nullptr, const int64_t srcLen = -1);
  static double Float(const ctString &str, int64_t *pLen = nullptr);

  static ctString String(const char *str, int64_t *pLen = nullptr, int64_t srclen = -1);
  static ctString String(const char **pStr, int64_t *pLen = nullptr, const int64_t srcLen = -1);
  static ctString String(const ctString &str, int64_t *pLen = nullptr);

  static ctString Quote(const char *str, int64_t *pLen = nullptr, int64_t srclen = -1);
  static ctString Quote(const char **pStr, int64_t *pLen = nullptr, const int64_t srcLen = -1);
  static ctString Quote(const ctString &str, int64_t *pLen = nullptr);

  static bool String(char *pOut, const int64_t maxLen, const char *str, int64_t *pLen = nullptr);
  static bool String(char *pOut, const int64_t maxLen, const char **str, int64_t *pLen = nullptr);
  static bool String(char *pOut, const int64_t maxLen, const char *str, int64_t strLen, int64_t *pLen = nullptr);
  static bool String(char *pOut, const int64_t maxLen, const char **pStr, int64_t strLen, int64_t * pLen = nullptr);
};

#endif // atScan_h__
