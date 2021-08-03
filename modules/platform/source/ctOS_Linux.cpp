#include "ctOS.h"

#ifdef ctPLATFORM_LINUX

#include<stdio.h>
#include<unistd.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

ctFileHandle ctOS::File::Open(const char *path, const ctFileMode &mode)
{
  FILE *pFile = nullptr;
  return fopen(path, ctFileCommon::FileMode(mode));
}

void ctOS::File::Close(const ctFileHandle &handle)
{
  if (handle != nullptr)
    fclose((FILE*)handle);
}

bool ctOS::File::Flush(const ctFileHandle &handle)
{
  return handle && fflush((FILE*)handle) == 0;
}

bool ctOS::File::Seek(const ctFileHandle &handle, const int64_t &loc, const ctSeekOrigin &origin, int64_t *pNewPosition)
{
  if (!handle)
    return false;

  int sysOrigin = 0;
  switch (origin)
  {
  case atSO_Start:   sysOrigin = SEEK_SET; break;
  case atSO_Current: sysOrigin = SEEK_CUR; break;
  case atSO_End:     sysOrigin = SEEK_END; break;
  default:
    return false;
  }

  fseek((FILE*)handle, (long)loc, (int)sysOrigin);
  
  if (pNewPosition)
    *pNewPosition = Tell(handle);

  return true;
}

int64_t ctOS::File::Tell(const ctFileHandle &handle)
{
  return ftell((FILE*)handle);
}

int64_t ctOS::File::Read(const ctFileHandle &handle, void *pDst, const int64_t &size)
{
  int64_t bytesRead = 0;
  if (handle != nullptr && pDst != nullptr && size > 0)
    bytesRead = fread(pDst, 1, (size_t)size, (FILE *)handle);
  return bytesRead;
}

int64_t ctOS::File::Write(const ctFileHandle &handle, const void *pSrc, const int64_t &size)
{
  return fwrite(pSrc, 1, size, (FILE*)handle);
}

bool ctOS::File::Exists(const char *path)
{
  return access(path, F_OK) != -1;
}

bool ctOS::File::EndOfFile(const ctFileHandle &handle)
{
  return feof((FILE*)handle) != 0;
}

#endif
