
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

#include "file/ctFileSystem.h"

#ifdef ctPLATFORM_WIN32
#include <direct.h>
#include <shlobj.h>

static bool _CreateFoldersRecursive(const ctString &done, const ctString &remaining)
{
  if (remaining.length() == 0)
    return true;

  int64_t nextSlash = ctMax(remaining.find_first_of('/'));
  ctString nextFolder = remaining.substr(0, nextSlash);
  int64_t res = _mkdir(done + nextFolder);
  if (res < 0 && errno != EEXIST && *(nextFolder.end() - 1) != ':')
    return false;

  return nextSlash != -1 ? _CreateFoldersRecursive(done + nextFolder + "/", remaining.substr(nextSlash + 1, -1)) : true;
}

static ctFilename _GetSystemPath(const int &folderID)
{
  char buffer[MAX_PATH] = { 0 };
  SHGetSpecialFolderPathA(0, buffer, folderID, false);
  return buffer;
}

ctFileSystem::ctFileSystem() {}
bool ctFileSystem::CreateFolders(const ctFilename &path) { return _CreateFoldersRecursive("", path.Path()); }

static ctVector<ctFileSystem::FileInfo> _EnumerateFolder(const ctFilename &path, const bool &findFiles, const bool &findFolders)
{
  ctVector<ctFileSystem::FileInfo> fileInfo;

  // Find the first file in the directory.
  WIN32_FIND_DATA ffd;
  LARGE_INTEGER filesize;
  HANDLE hFind = FindFirstFile(path.Path().replace("/", "\\") + "\\*", &ffd);

  if (INVALID_HANDLE_VALUE == hFind)
    return{};

  // List all the files in the directory with some info about them.

  do
  {
    ctFileSystem::FileInfo newInfo;
    newInfo.isFolder = (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) > 0;
    if ((newInfo.isFolder && findFolders) || (!newInfo.isFolder && findFiles))
    {
      if (!newInfo.isFolder)
      {
        filesize.LowPart = ffd.nFileSizeLow;
        filesize.HighPart = ffd.nFileSizeHigh;
        newInfo.size = filesize.QuadPart;
      }

      newInfo.path = path.Path() + "/" + ffd.cFileName;
      fileInfo.push_back(newInfo);
    }
  } while (FindNextFile(hFind, &ffd) != 0);

  FindClose(hFind);
  return fileInfo;
}

ctVector<ctFileSystem::FileInfo> ctFileSystem::EnumerateFiles(const ctFilename &path) { return _EnumerateFolder(path, true, false); }
ctVector<ctFileSystem::FileInfo> ctFileSystem::EnumerateFolders(const ctFilename &path) { return _EnumerateFolder(path, false, true); }
ctVector<ctFileSystem::FileInfo> ctFileSystem::Enumerate(const ctFilename &path) { return _EnumerateFolder(path, true, true); }

ctFilename ctFileSystem::GetDirectory_AppData() { return _GetSystemPath(CSIDL_APPDATA); }
ctFilename ctFileSystem::GetDirectory_AppData_Local() { return _GetSystemPath(CSIDL_LOCAL_APPDATA); }
ctFilename ctFileSystem::GetDirectory_Windows() { return _GetSystemPath(CSIDL_WINDOWS); }
ctFilename ctFileSystem::GetDirectory_Desktop() { return _GetSystemPath(CSIDL_DESKTOP); }
ctFilename ctFileSystem::GetDirectory_Documents() { return _GetSystemPath(CSIDL_MYDOCUMENTS); }
ctFilename ctFileSystem::GetDirectory_Fonts() { return _GetSystemPath(CSIDL_FONTS); }
ctFilename ctFileSystem::GetDirectory_History() { return _GetSystemPath(CSIDL_HISTORY); }
ctFilename ctFileSystem::GetDirectory_Recents() { return _GetSystemPath(CSIDL_RECENT); }
ctFilename ctFileSystem::GetDirectory_StartMenu() { return _GetSystemPath(CSIDL_STARTMENU); }
ctFilename ctFileSystem::GetDirectory_ProgramFiles() { return _GetSystemPath(CSIDL_PROGRAM_FILES); }
ctFilename ctFileSystem::GetDirectory_ProgramFiles86() { return _GetSystemPath(CSIDL_PROGRAM_FILESX86); }

#endif
