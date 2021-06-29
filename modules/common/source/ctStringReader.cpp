#include "ctStringReader.h"

ctStringReader::ctStringReader(ctReadStream *pStream)
  : m_pStream(pStream)
{}

int64_t ctStringReader::Available() const
{
  return m_pStream->Available();
}

char ctStringReader::Peek()
{
  char c;
  m_pStream->Peek(&c, 1);
  return c;
}

char ctStringReader::Read()
{
  char c;
  m_pStream->Read(&c, 1);
  return c;
}

int64_t ctStringReader::Read(char *pBuffer, int64_t count)
{
  return m_pStream->Read(pBuffer, count);
}

ctString const & ctStringReader::ReadLine()
{
  static ctString newLineSet = "\r\n";
  return ReadUntil(newLineSet);
}

ctString const & ctStringReader::ReadUntil(char character)
{
  m_lastToken = "";
  char c = 0;
  while (c != character && c != 0) {
    m_lastToken += c;
    c = Read();
  }

  return m_lastToken;
}

ctString const & ctStringReader::ReadUntil(const ctString &set)
{
  m_lastToken = "";
  char c = Read();
  while (c != 0 && set.find(c) == CT_INVALID_INDEX) {
    m_lastToken += c;
    c = Read();
  }

  return m_lastToken;
}

ctString const & ctStringReader::ReadUntilToken(const ctString &token)
{
  m_lastToken = "";
  while (!m_lastToken.ends_with(token.c_str())) {
    m_lastToken += Read();
  }
  return m_lastToken;
}

ctReadStream * ctStringReader::GetStream()
{
  return m_pStream;
}

ctReadStream const * ctStringReader::GetStream() const
{
  return m_pStream;
}
