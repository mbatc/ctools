
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

#include "file/ctFileInfo.h"

bool ctFileInfo::SetFile(const ctFilename &file)
{
  m_exists = stat(file.c_str(), &m_data) == 0;
  return m_exists;
}

ctFileInfo::ctFileInfo() : m_exists(false) {}
ctFileInfo::ctFileInfo(const ctFilename &file) { SetFile(file); }
bool ctFileInfo::IsDirectory() const { return (m_data.st_mode & S_IFDIR) > 0; }
int64_t ctFileInfo::Size() const { return m_data.st_size; }
int64_t ctFileInfo::DriveNo() const { return m_data.st_dev; }
ctDateTime ctFileInfo::Created() const { return ctDateTime(m_data.st_ctime); }
ctDateTime ctFileInfo::LastAccessed() const { return ctDateTime(m_data.st_atime); }
ctDateTime ctFileInfo::LastModified() const { return ctDateTime(m_data.st_mtime); }
const ctFilename& ctFileInfo::Filename() const { return m_fn; }
bool ctFileInfo::IsDirectory(const ctFilename &file) { return ctFileInfo(file).IsDirectory(); }
int64_t ctFileInfo::Size(const ctFilename &file) { return ctFileInfo(file).Size(); }
int64_t ctFileInfo::DriveNo(const ctFilename &file) { return ctFileInfo(file).DriveNo(); }
ctDateTime ctFileInfo::Created(const ctFilename &file) { return ctFileInfo(file).Created(); }
ctDateTime ctFileInfo::LastAccessed(const ctFilename &file) { return ctFileInfo(file).LastAccessed(); }
ctDateTime ctFileInfo::LastModified(const ctFilename &file) { return ctFileInfo(file).LastModified(); }
