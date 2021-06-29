#include "ctStringReader.h"

static int64_t _bufferSize = 1024 * 1024ll;

ctStringReader::ctStringReader(ctReadStream *pStream)
  : m_pStream(pStream)
{}

int64_t ctStringReader::Available() const
{
  return m_pStream->Available();
}

char ctStringReader::Peek()
{
  TryReadBuffer();

  return m_buffer[m_offset];
}

char ctStringReader::Read()
{
  TryReadBuffer();

  return m_buffer[m_offset++];
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
    m_lastToken.append(c);
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

void ctStringReader::TryReadBuffer()
{
  if (m_offset < m_buffer.size())
    return;

  m_buffer.resize(_bufferSize);
  m_buffer.resize(m_pStream->Read(m_buffer.data(), m_buffer.size()));
  m_offset = 0;
}
