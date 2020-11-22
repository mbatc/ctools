
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

#include "ctOS.h"
#include "file/ctFile.h"
#include "file/ctFileSystem.h"

ctFile::ctFile() : m_handle(nullptr) { Close(); }
ctFile::ctFile(const ctFilename &file, const ctFileMode mode) : m_handle(nullptr), m_mode(atFM_None) { Open(file, mode); }
ctFile::ctFile(const ctFile &copy) : m_handle(nullptr) { Open(copy.m_fn, copy.m_mode); }
ctFile::~ctFile() { Close(); }

ctFile::ctFile(ctFile &&move)
  : m_handle(move.m_handle)
  , m_mode(move.m_mode)
  , m_fn(move.m_fn)
{
  move.m_handle = nullptr;
  move.m_mode = atFM_None;
  move.m_fn = "";
}

bool ctFile::Open(const ctFilename &file, const int64_t mode)
{
  if (file == m_fn)
    return file != "";
  Close();
  m_handle = ctOS::File::Open(file.c_str(), (ctFileMode)mode);
  if (m_handle)
  {
    m_mode = mode;
    m_fn = file;
    m_info.SetFile(file);
    m_pos = Tell();
  }

  return m_handle != nullptr;
}

bool ctFile::Close()
{
  if (!m_handle)
    return false;
  Flush();
  ctOS::File::Close(m_handle);
  m_handle = nullptr;
  m_fn = "";
  m_mode = atFM_None; 
  m_info = ctFileInfo();
  return true;
}

bool ctFile::Seek(const int64_t loc, const ctSeekOrigin origin)
{
  return ctOS::File::Seek(m_handle, loc, origin);
}

int64_t ctFile::Tell() const
{
  return ctOS::File::Tell(m_handle);
}

int64_t ctFile::Write(const void *pData, const int64_t len)
{
  return ctOS::File::Write(m_handle, pData, len);
}

ctString ctFile::ReadText()
{
  if (!IsOpen() || m_info.Size() == 0)
    return "";
  ctVector<char> data(m_info.Size(), 0);
  Read(data.data(), data.size());
  return data.data();
}

ctString ctFile::ReadText(const ctFilename &filename)
{
  ctFile file;
  ctString ret;
  if (file.Open(filename, atFM_Read))
    ret = file.ReadText();
  return ret;
}

int64_t ctFile::WriteFile(const ctFilename &filename, const void *pData, const int64_t &len)
{
  ctFile output;
  if (!output.Open(filename, atFM_WriteBinary))
    return 0;
  return output.Write(pData, len);
}

int64_t ctFile::WriteTextFile(const ctFilename &filename, const ctString &content)
{
  return WriteFile(filename, content.c_str(), content.length());
}

bool ctFile::Flush()
{
  return ctOS::File::Flush(m_handle);
}

int64_t ctFile::Read(void *pBuffer, const int64_t size)
{
  return ctOS::File::Read(m_handle, pBuffer, size);
}

ctFilename ctFile::Find(const ctFilename &fn, bool *pResult)
{
  if (pResult)
    *pResult = true;

  if (Exists(fn))
    return fn;

  // Iterate sub folders and check if the file exists in them
  for (const ctFileSystem::FileInfo &subFolder : ctFileSystem::EnumerateFolders(fn.Directory()))
  {
    ctFilename path = subFolder.path.Path() + "/" + fn.Name();
    if (Exists(path))
      return path;
  }

  if (pResult)
    *pResult = false;
  return fn;
}

bool ctFile::Exists(const ctFilename &fn)
{
  return ctOS::File::Exists(fn.c_str());

#ifndef ctPLATFORM_WIN32
  int result = access(fn.c_str(), X_OK);
  return result == 0 || result == EACCES || result == EROFS;
#endif
}

int64_t ctFile::WriteText(const ctString &text) { return Write(text.c_str(), text.length()); }
bool ctFile::Create(const ctFilename &fn) { return ctFile(fn, atFM_Append).IsOpen(); }
bool ctFile::Copy(const ctFilename &fn) { ctUnused(fn); return false; }
bool ctFile::Move(const ctFilename &fn) { ctUnused(fn); return false; }
const ctFileInfo& ctFile::Info() { return m_info; }
int64_t ctFile::GetMode() { return m_mode; }
bool ctFile::IsOpen() const { return m_handle && m_mode != atFM_None; }
bool ctFile::Delete(const ctFilename &fn) { return remove(fn.Path().c_str()) == 0; }
