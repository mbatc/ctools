
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

#include "ctStreamSeekable.h"
#include "ctAssert.h"

bool ctStreamSeekable::Seek(const int64_t loc, const ctSeekOrigin origin)
{
  ctAssert(false, "Seek operation not supported.");
  return false;
}

int64_t ctStreamSeekable::Tell() const
{
  ctAssert(false, "Tell operation not supported.");
  return 0;
}
// Returns the length of the stream
int64_t ctStreamSeekable::Length() const
{
  ctAssert(false, "Length operation not supported");
  return 0;
}

// Returns the number of bytes available to read
int64_t ctStreamSeekable::Available() const
{
  ctAssert(false, "Available operation not supported");
  return 0;
}

bool ctStreamSeekable::SeekToStart() { return Seek(0, atSO_Start); }
bool ctStreamSeekable::SeekToEnd() { return Seek(0, atSO_End); }
