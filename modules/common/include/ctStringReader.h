#ifndef ctStringReader_h__
#define ctStringReader_h__

#include "ctString.h"

class ctReadStream;

class ctStringReader
{
public:
  ctStringReader(ctReadStream *pStream);

  int64_t Available() const;

  char Peek();

  char Read();

  int64_t Read(char *pBuffer, int64_t count);

  ctString const & ReadLine();

  ctString const & ReadUntil(char character);

  ctString const & ReadUntil(const ctString &set);

  ctString const & ReadUntilToken(const ctString &token);

  ctReadStream * GetStream();

  ctReadStream const * GetStream() const;

private:
  void TryReadBuffer();

  ctString m_lastToken;
  ctReadStream * m_pStream = nullptr;
  ctVector<char> m_buffer;
  int64_t m_offset = 0;
};

#endif // ctStringReader_h__
