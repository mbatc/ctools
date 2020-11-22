
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

#ifndef atFileSystem_h__
#define atFileSystem_h__

#include "ctFileCommon.h"
#include "ctFilename.h"

class ctFileSystem
{
public:
  ctFileSystem();

  struct FileInfo
  {
    bool isFolder = false;
    int64_t size = 0;
    ctFilename path;
  };

  static bool CreateFolders(const ctFilename &path);
  static ctVector<FileInfo> EnumerateFiles(const ctFilename &path);
  static ctVector<FileInfo> EnumerateFolders(const ctFilename &path);
  static ctVector<FileInfo> Enumerate(const ctFilename &path);

  static ctFilename GetDirectory_AppData();
  static ctFilename GetDirectory_AppData_Local();
  static ctFilename GetDirectory_Windows();
  static ctFilename GetDirectory_Desktop();
  static ctFilename GetDirectory_Documents();
  static ctFilename GetDirectory_Fonts();
  static ctFilename GetDirectory_History();
  static ctFilename GetDirectory_Recents();
  static ctFilename GetDirectory_StartMenu();
  static ctFilename GetDirectory_ProgramFiles();
  static ctFilename GetDirectory_ProgramFiles86();
};

#endif // atFileSystem_h__
