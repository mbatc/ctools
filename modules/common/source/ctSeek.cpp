#include "ctSeek.h"
#include "ctString.h"

static inline bool _Seek(const char **ppText, const int64_t &dist)
{
  if (dist != CT_INVALID_INDEX)
  {
    *ppText += dist;
    return true;
  }

  return false;
}

bool ctSeek::SeekTo(const char **ppText, const char target, const bool &positionAtEnd) { return _Seek(ppText, ctString::_find(*ppText, target, 0) + (positionAtEnd ? 1 : 0)); }
bool ctSeek::SeekTo(const char **ppText, const char *target, const bool &positionAtEnd) { return _Seek(ppText, ctString::_find(*ppText, target, 0) + (positionAtEnd ? strlen(target) : 0)); }
bool ctSeek::SeekToSet(const char **ppText, const char *charList) { return _Seek(ppText, ctString::_find_first_of(*ppText, charList)); }

bool ctSeek::SeekToRange(const char **ppText, const char low, const char high)
{
  while ((**ppText < low || **ppText > high) && **ppText != 0) ++(*ppText);
  return **ppText != 0;
}

bool ctSeek::SkipRange(const char **ppText, const char low, const char high)
{
  while ((**ppText >= low && **ppText <= high) && **ppText != 0) ++(*ppText);
  return **ppText != 0;
}

bool ctSeek::Skip(const char **ppText, const char c) { return _Seek(ppText, ctString::_find_first_not(*ppText, c)); }
bool ctSeek::Skip(const char **ppText, const char *charList) { return _Seek(ppText, ctString::_find_first_not(*ppText, charList)); }
bool ctSeek::SkipWhitespace(const char **ppText) { return _Seek(ppText, ctString::_find_first_not(*ppText, ctString::Whitespace())); }
bool ctSeek::SeekToWhitespace(const char **ppText) { return _Seek(ppText, ctString::_find_first_of(*ppText, ctString::Whitespace())); }

ctStringSeeker::ctStringSeeker(ctString const * pStr)
  : m_pStr(pStr)
{
  m_pText = m_pLast = m_pLastLast = m_pStr->c_str();
}

bool ctStringSeeker::Seek(int64_t pos, const ctSeekOrigin &origin)
{
  m_pLastLast = m_pText;

  switch (origin)
  {
  case atSO_Current:
    break;
  case atSO_Start:
    m_pText = m_pStr->begin();
    break;
  case atSO_End:
    m_pText = m_pStr->end();
    pos = -pos;
    break;
  }

  m_pText += pos;

  bool result = m_pText < m_pStr->begin() || m_pText > m_pStr->end();
  m_pText = ctClamp(m_pText, m_pStr->begin(), m_pStr->end());
  m_pLast = m_pText;
  return result;
}

int64_t ctStringSeeker::Length() const { return m_pStr->length(); }
const char* ctStringSeeker::Text() const { return m_pText; }
const char* ctStringSeeker::LastText() const { return m_pLastLast; }
const char* ctStringSeeker::begin() const { return m_pStr->begin(); }
const char* ctStringSeeker::end() const { return m_pStr->end(); }

ctString ctStringSeeker::GetString(const int64_t &endIdx, const int64_t &startIdx)
{
  const char *startPos = startIdx == CT_INVALID_INDEX ? m_pText : (m_pText + startIdx);
  const char *endPos = endIdx == CT_INVALID_INDEX ? m_pStr->end() : (m_pText + endIdx);
  return ctString(ctClamp(startPos, begin(), end()), ctClamp(endPos, begin(), end()));
}

char ctStringSeeker::Char() const { return *Text(); }

bool ctStringSeeker::SeekTo(const char target, const bool &positionAtEnd) { return DoSeek(ctSeek::SeekTo(&m_pText, target, positionAtEnd)); }
bool ctStringSeeker::SeekTo(const char *target, const bool &positionAtEnd) { return DoSeek(ctSeek::SeekTo(&m_pText, target, positionAtEnd)); }
bool ctStringSeeker::SeekToSet(const char *charList) { return DoSeek(ctSeek::SeekToSet(&m_pText, charList)); }
bool ctStringSeeker::SeekToRange(const char low, const char high) { return DoSeek(ctSeek::SeekToRange(&m_pText, low, high)); }
bool ctStringSeeker::SkipRange(const char low, const char high) { return DoSeek(ctSeek::SkipRange(&m_pText, low, high)); }
bool ctStringSeeker::Skip(const char c) { return DoSeek(ctSeek::Skip(&m_pText, c)); }

bool ctStringSeeker::Skip(const char *charList) { return DoSeek(ctSeek::Skip(&m_pText, charList)); }
bool ctStringSeeker::SkipWhitespace() { return DoSeek(ctSeek::SkipWhitespace(&m_pText)); }
bool ctStringSeeker::SeekToWhitespace() { return DoSeek(ctSeek::SeekToWhitespace(&m_pText)); }

bool ctStringSeeker::DoSeek(const bool &success)
{
  m_pLastLast = m_pLast;
  m_pLast = m_pText;
  if (success)
    return true;

  m_pText = end();
  return false;
}
