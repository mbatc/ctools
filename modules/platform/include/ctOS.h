#ifndef ctOS_h__
#define ctOS_h__

#include "file/ctFileCommon.h"
#include "ctStreamSeekable.h"

typedef void* ctFileHandle;

class ctOS
{
public:
  ctOS() = delete;

  class File
  {
  public:
    File() = delete;

    static ctFileHandle Open(const char *path, const ctFileMode &mode);
    static void Close(const ctFileHandle &handle);

    static bool Flush(const ctFileHandle &handle);
    static bool Seek(const ctFileHandle &handle, const int64_t &loc, const ctSeekOrigin &origin, int64_t* pNewPosition);
    static int64_t Tell(const ctFileHandle &handle);

    static int64_t Read(const ctFileHandle &handle, void *pDst, const int64_t &size);
    static int64_t Write(const ctFileHandle &handle, const void *pSrc, const int64_t &size);

    static bool Exists(const char *path);
    static bool Copy(const char *src, const char *dst, bool overwrite = false);
    static bool Move(const char *src, const char *dst);
    static bool EndOfFile(const ctFileHandle &handle);
  };
};

#endif // ctOS_h__
