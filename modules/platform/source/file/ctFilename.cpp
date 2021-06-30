
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

#include "file/ctFilename.h"

ctFilename::ctFilename(ctFilename &&move)
  : m_fullpath(move.m_fullpath)
  , m_directory(move.m_directory)
  , m_extension(move.m_extension)
  , m_name(move.m_name)
{}

void ctFilename::assign(const ctString &path)
{
  m_fullpath = path;
  m_fullpath = m_fullpath.replace('\\', '/');
  m_fullpath = m_fullpath.replace("//", "/");

  const int64_t lastDot = m_fullpath.find_last('.');
  const int64_t lastSlash = m_fullpath.find_last('/');
  if (lastDot > 0 && lastDot > lastSlash)
    m_extension = m_fullpath.substr(lastDot + 1, m_fullpath.length());
  m_name = m_fullpath.substr(lastSlash + 1, m_fullpath.length() - (m_extension.length() > 0 ? 1 : 0) - m_extension.length());
  m_directory = m_fullpath.substr(0, lastSlash);
}

ctFilename ctFilename::ResolveFullPath(const ctFilename &path)
{
  char buffer[1024] = { 0 };
  #ifdef ctPLATFORM_WIN32
  _fullpath(buffer, path.Path(), 1024);
  return ctFilename(buffer);
  #else
  return path;
  #endif
}

ctFilename ctFilename::ResolveRelativePath(const ctFilename &to, const ctFilename &from)
{
  int64_t toDriveLoc = CT_INVALID_INDEX;
  int64_t fromDriveLoc = CT_INVALID_INDEX;

  toDriveLoc = to.Path().find(':');
  fromDriveLoc = to.Path().find(':');

  if (toDriveLoc != fromDriveLoc)
    return to;

  // If drives are not the same return 'to' as there is no relative path between them
  if (to.Path().substr(0, toDriveLoc).compare(from.Path().substr(0, fromDriveLoc), atSCO_None))
    return to;

  // Get a list of all directories
  ctVector<ctString> toDirs = to.Path().split('/');
  ctVector<ctString> fromDirs = from.Path().split('/');

  // Remove all matching directories from the start of the path
  while (toDirs.front().compare(fromDirs.front(), atSCO_None))
  {
    toDirs.pop_front();
    fromDirs.pop_front();
  }

  // For all remaining directories in 'fromDirs' add a step up. (size() - 1 as the last string will be the filename)
  ctString finalPath;
  for (int64_t i = 0; i < fromDirs.size() - 1; ++i)
    finalPath += "../";

  // Add the remaining directories in toDirs to the final path
  finalPath += ctString::join(toDirs, '/');

  return finalPath;
}

template<> ctFilename ctFromString<ctFilename>(const ctString &str)
{
  ctFilename file;
  file.assign(str);
  return file;
}

ctFilename ctFilename::operator=(const ctFilename &fn) { assign(fn.m_fullpath);  return *this; }
ctFilename ctFilename::operator=(const ctString &fn) { assign(fn); return *this; }
ctFilename ctFilename::operator=(const char *fn) { assign(fn);  return *this; }
bool ctFilename::operator==(const ctFilename &fn) const { return m_fullpath == fn.m_fullpath; }
bool ctFilename::operator!=(const ctFilename &fn) const { return !(*this == fn); }
bool ctFilename::operator==(const ctString &fn) const { return m_fullpath == fn.replace('\\', '/'); }
bool ctFilename::operator!=(const ctString &fn) const { return !(*this == fn); }
bool ctFilename::operator==(const char *fn) const { return *this == ctString(fn); }
bool ctFilename::operator!=(const char *fn) const { return *this != ctString(fn); }
const char* ctFilename::c_str() const { return m_fullpath.c_str(); }
ctFilename::ctFilename() : ctFilename("") {}
ctFilename::ctFilename(const char *path) { assign(path); }
ctFilename::ctFilename(const ctString &path) { assign(path); }
ctFilename::ctFilename(const ctFilename &copy) { assign(copy.m_fullpath); }
ctString ctFilename::Path(const bool withExtension) const { return withExtension ? m_fullpath : m_directory + "/" + m_name; }
ctString ctFilename::Name(const bool withExtension) const { return withExtension && m_extension.length() > 0 ? m_name + "." + m_extension : m_name; }
const ctString& ctFilename::Extension() const { return m_extension; }
const ctString& ctFilename::Directory() const { return m_directory; }
ctFilename ctFilename::ResolveFullPath() const { return ResolveFullPath(*this); }
ctFilename ctFilename::ResolveRelativePath(const ctFilename &to) const { return ResolveRelativePath(to, *this); }

int64_t ctStreamRead(ctReadStream *pStream, ctFilename *pData, const int64_t count)
{
  ctString path;
  int64_t ret = 0;
  for (ctFilename &fn : ctIterate(pData, count))
  {
    ret += ctStreamRead(pStream, &path, 1);
    fn.assign(path);
  }
  return ret;
}

int64_t ctStreamWrite(ctWriteStream *pStream, const ctFilename *pData, const int64_t count)
{
  int64_t ret = 0;
  for (const ctFilename &fn : ctIterate(pData, count))
    ret += ctStreamWrite(pStream, &fn.m_fullpath, 1);
  return ret;
}

bool ctSerialize(ctObjectDescriptor *pSerialized, const ctFilename &src)
{
  pSerialized->Set(src.m_fullpath);
  return true;
}

bool ctDeserialize(const ctObjectDescriptor &serialized, ctFilename *pDst)
{
  ctString fullPath = serialized.AsString(pDst->m_fullpath);
  *pDst = ctFilename(fullPath);
  return true;
}
