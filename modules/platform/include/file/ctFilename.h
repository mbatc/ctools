
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

#ifndef _atFilename_h__
#define _atFilename_h__

#include "ctString.h"
#include "ctObjectDescriptor.h"

class ctFilename
{
public:
  ctFilename();
  ctFilename(const char *path);
  ctFilename(const ctString &path);
  ctFilename(const std::string &path);
  ctFilename(const ctFilename &copy);
  ctFilename(ctFilename &&move);
  
  ctString Path(const bool withExtension = true) const;
  ctString Name(const bool withExtension = true) const;
  const ctString& Extension() const;
  const ctString& Directory() const;
  const ctString& Drive() const;

  ctFilename ResolveFullPath() const;
  static ctFilename ResolveFullPath(const ctFilename &path);

  // Not Implemented!
  ctFilename ResolveRelativePath(const ctFilename &to) const;
  static ctFilename ResolveRelativePath(const ctFilename &to, const ctFilename &from);

  void assign(const ctString &path);

  const char* c_str() const;

  bool operator==(const ctFilename &fn) const;
  bool operator!=(const ctFilename &fn) const;
  bool operator==(const ctString &fn) const;
  bool operator!=(const ctString &fn) const;
  bool operator==(const char *fn) const;
  bool operator!=(const char *fn) const;

  ctFilename operator=(const ctFilename &fn);
  ctFilename operator=(const ctString &fn);
  ctFilename operator=(const char *fn);

  friend int64_t ctStreamRead(ctReadStream *pStream, ctFilename *pData, const int64_t count);
  friend int64_t ctStreamWrite(ctWriteStream *pStream, const ctFilename *pData, const int64_t count);

  friend bool ctSerialize(ctObjectDescriptor *pSerialized, const ctFilename &src);
  friend bool ctDeserialize(const ctObjectDescriptor &serialized, ctFilename *pDst);

  // Concatenate a filepath
  ctFilename operator/(const ctFilename &fn) const;
  
protected:
  ctString m_fullpath;
  ctString m_name;
  ctString m_extension;
  ctString m_directory;
  ctString m_drive;
};

template<> ctFilename ctFromString<ctFilename>(const ctString &str);

#endif