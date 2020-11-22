
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

#include "ctAlloc.h"
#include <malloc.h>

void* _ctRawAlloc(const int64_t size) 
{ 
  return malloc((size_t)size);
}

void* _ctRawRealloc(void *pBlock, const int64_t size) 
{ 
  return realloc(pBlock, (size_t)size);
}

void* _ctAllocTrace(const int64_t size, const int64_t line, const char *file, const char *function)
{
#ifdef _DEBUG
  return _ctAllocRelTrace(size, line, file, function);
#else
  return _ctRawAlloc(size);
#endif
}

void* _ctReallocTrace(void *pBlock, const int64_t size, const int64_t line, const char *file, const char *function)
{
#ifdef _DEBUG
  return _ctReallocRelTrace(pBlock, size, line, file, function);
#else
  return _ctRawAlloc(size);
#endif
}

void* _ctAllocRelTrace(const int64_t size, const int64_t line, const char * file, const char *function)
{
  void *ret = _ctRawAlloc(size);
  _ctAssert(ret != nullptr, "ret != nullptr", "Memory Allocation Failure", line, file, function);
  return ret;
}

void* _ctReallocRelTrace(void *pBlock, const int64_t size, const int64_t line, const char * file, const char *function)
{
  void *ret = _ctRawRealloc(pBlock, size);
  _ctAssert(ret != nullptr, "ret != nullptr", "Memory Allocation Failure", line, file, function);
  return ret;
}

void ctReleaseMemRef(void *pBlock)
{
  pBlock;
}

void ctFree(void *pBlock)
{
  free(pBlock);
}
