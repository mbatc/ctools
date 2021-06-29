#include "ctXML.h"
#include "ctScan.h"
#include "ctSeek.h"

#define XML_SEPERATOR " \t\r\n/=<>"

ctXML::ctXML(const ctString &xml)
{
  if (xml.length() > 0)
    Parse(xml);
}

ctXML::ctXML(const ctXML &copy) { *this = copy; }
ctXML::ctXML(ctXML &&move) { *this = std::move(move); }

ctXML& ctXML::operator=(ctXML &&rhs)
{
  m_children = std::move(rhs.m_children);
  m_tag = std::move(rhs.m_tag);
  m_value = std::move(rhs.m_value);
  m_attributes = std::move(rhs.m_attributes);
  return *this;
}

ctXML& ctXML::operator=(const ctXML &rhs)
{
  m_children = rhs.m_children;
  m_tag = rhs.m_tag;
  m_value = rhs.m_value;
  m_attributes = rhs.m_attributes;
  return *this;
}

bool ctXML::Parse(const ctString &xml)
{
  if (xml.length() == 0)
    return false;

  // Strip XML comments
  ctString strippedXml;
  {
    ctStringSeeker seeker(&xml);
    bool addEnd = true;
    const char *lastPos = seeker.Text();
    while (seeker.SeekTo("<!--"))
    {
      strippedXml += ctString(lastPos, seeker.Text());

      if (!seeker.SeekTo("-->", true))
      {
        addEnd = false;
        break;
      }

      lastPos = seeker.Text();
    }

    if (addEnd)
      strippedXml += lastPos;
  }

  if (strippedXml.length() == 0)
    return false;

  { // Build XML elements
    ctStringSeeker seeker(&strippedXml);
    ctXML child;
    ctString endTag;
    ctVector<ctString> tags;
    while (BuildElement(&seeker, &child, &tags, &endTag) != 0)
      m_children.push_back(std::move(child));
  }

  return true;
}

void ctXML::SetTag(const ctString &tag) { m_tag = tag; }
const ctString& ctXML::GetTag() const { return m_tag; }

void ctXML::SetAttribute(const ctString &name, const ctString &value)
{
  if (!m_attributes.TryAdd(name, value))
    m_attributes[name] = value;
}

ctString ctXML::GetAttribute(const ctString &name) const
{
  const ctString *pValue = m_attributes.TryGet(name);
  return pValue ? *pValue : "";
}

void ctXML::SetAttributes(const ctHashMap<ctString, ctString> &attribs) { m_attributes = attribs; }
const ctHashMap<ctString, ctString>& ctXML::GetAttributes() const { return m_attributes; }

double ctXML::AsFloat() const { return ctScan::Float(m_value); }
int64_t ctXML::AsInt() const { return ctScan::Int(m_value); }
bool ctXML::AsBool() const { return m_value == "1" || m_value.compare("true", atSCO_None); }

const ctString& ctXML::AsString() const { return m_value; }

void ctXML::Set(const bool &value) { Set(ctString(value)); }
void ctXML::Set(const double &value) { Set(ctString(value)); }
void ctXML::Set(const int64_t &value) { Set(ctString(value)); }
void ctXML::Set(const ctString &value) { m_value = value; }

int64_t ctXML::GetChildCount() const { return m_children.size(); }

ctXML* ctXML::GetChild(const int64_t &index) { return index >= 0 && index < m_children.size() ? &m_children[index] : nullptr; }
const ctXML* ctXML::GetChild(const int64_t &index) const { return index >= 0 && index < m_children.size() ? &m_children[index] : nullptr; }

void ctXML::AddChild(const ctXML &xml, const int64_t &index) { m_children.insert(ctClamp(index, 0, m_children.size()), xml); }
void ctXML::RemoveChild(const int64_t &index)
{
  if (index >= 0 && index < m_children.size())
    m_children.erase(index);
}

static ctString _FormatContent(const ctString &text)
{
  ctString formatted;
  bool first = true;
  bool startsWithWhitespace = false;

  ctStringSeeker seeker(&text);

  while (seeker.SkipWhitespace())
  {
    startsWithWhitespace |= first && seeker.Text() == seeker.begin();
    const char *start = seeker.Text();
    bool exit = !seeker.SeekToWhitespace();
    formatted += ctString(start, seeker.Text());
    if (seeker.Text() != seeker.end())
      formatted += " ";
    if (exit)
      break;
  }

  if (seeker.end() != seeker.Text())
    formatted += (formatted.length() ? " " : "") + seeker.GetString();
  return formatted;
}

int64_t ctXML::BuildElement(ctStringSeeker *pSeeker, ctXML *pElem, ctVector<ctString> *pTagStack, ctString *pEndTag)
{
  *pElem = ctXML();

  pSeeker->SeekTo('<');
  if (pSeeker->Text()[1] == '/')
  {
    if (pEndTag->length() == 0)
    {
      const char *start = pSeeker->Text();
      if (!pSeeker->SeekTo('>'))
        return 0;

      *pEndTag = ctString(start + 2, pSeeker->Text());
      pSeeker->Seek(start - pSeeker->begin(), atSO_Start);

      if (pEndTag->length() > 0)
      { // Check for an invalid end tag
        for (int64_t tagIdx = pTagStack->size() - 1; tagIdx >= 0; --tagIdx)
          if (pTagStack->at(tagIdx) == *pEndTag)
            return 0;

        *pEndTag = "";
        pSeeker->SeekTo('>', true); // Seek past this end tag
        return -1;
      }
    }

    return 0;
  }

  // Find opening tag
  int64_t closePos = ctString::_find_first_of(pSeeker->Text(), '>');
  if (closePos < 0)
    return false;

  bool hasBody = (pSeeker->Text())[closePos - 1] != '/';
  const char *end = pSeeker->Text() + closePos - !hasBody;
  pSeeker->Seek(1);
  int64_t tagEnd = ctString::_find_first_of(pSeeker->Text(), XML_SEPERATOR);
  if (tagEnd > closePos || tagEnd <= 0)
    return 0;
  pElem->m_tag = ctString(pSeeker->Text(), pSeeker->Text() + tagEnd).to_lower();

  if (!hasBody)
    return 1;

  // Find attributes
  ctVector<ctString> attributes;
  ctVector<ctString> attrValues;
  while (pSeeker->SeekToSet(XML_SEPERATOR))
  {
    pSeeker->SkipWhitespace();
    if (pSeeker->Char() == '=')
    {
      pSeeker->Seek(1); // Skip '=' character
      pSeeker->SkipWhitespace();
      char quoteType = pSeeker->Char();
      bool isQuote = ctString::_find_first_of("'\"", quoteType) != CT_INVALID_INDEX;
      pSeeker->Seek(isQuote);
      int64_t end = isQuote ? ctString::_find(pSeeker->Text(), quoteType) : ctString::_find_first_of(pSeeker->Text(), XML_SEPERATOR);

      if (attrValues.size() >= attributes.size())
        attrValues.at(attributes.size() - 1) = pSeeker->GetString(end);
      pSeeker->Seek(end);
    }
    else
    {
      const char *start = pSeeker->Text();
      if (!pSeeker->SeekToSet(XML_SEPERATOR) || pSeeker->Text() >= end || start == pSeeker->Text())
        break;
      attributes.push_back(ctString(start, pSeeker->Text()));
      attrValues.push_back(ctString());
    }
  }

  for (int64_t attrIdx = 0; attrIdx < (int64_t)attributes.size(); ++attrIdx)
    pElem->SetAttribute(attributes[attrIdx], attrValues[attrIdx]);

  pSeeker->Seek(end - pSeeker->begin() + 1 + !hasBody, atSO_Start);

  // Find any content before any child elements
  int64_t nextTagPos = ctString::_find_first_of(pSeeker->Text(), '<');
  pElem->m_value = _FormatContent(pSeeker->GetString(nextTagPos));
  pSeeker->Seek(nextTagPos);

  if (pElem->m_tag == "br")
    return 1;

  pTagStack->push_back(pElem->m_tag);

  int64_t tagID = pTagStack->size();

  ctXML child;
  while (true)
  {
    int64_t result = BuildElement(pSeeker, &child, pTagStack, pEndTag);
    if (result == 0) // Finished parsing this element
      break;

    if (result == -1) // Error parsing this element
      continue;

    pElem->m_children.push_back(std::move(child));

    if (result == 2) // child did not have an end tag so move children
    {
      ctVector<ctXML> temp = std::move(pElem->m_children.back().m_children);
      pElem->m_children.push_back(std::move(temp));
    }

    // Find any content after this child
    int64_t nextTagPos = ctString::_find_first_of(pSeeker->Text(), '<');

    ctString extra = _FormatContent(pSeeker->GetString(nextTagPos));
    if (extra.length())
      pElem->m_value += (pElem->m_value.length() ? "\n": "") + extra;

    pSeeker->Seek(nextTagPos);
  }

  bool hasEndTag = false;
  if (pEndTag->length() && pElem->m_tag.compare(*pEndTag, atSCO_None))
  {
    *pEndTag = "";
    pSeeker->SeekTo('>', true);
    hasEndTag = true;
  }

  pTagStack->pop_back();

  return hasEndTag ? 1 : 2;
}

ctString ctToString(const ctXML &xml)
{
  ctString xmlTag = "<" + xml.GetTag();
  for (const ctKeyValue<ctString, ctString> &kvp : xml.GetAttributes())
    xmlTag += " " + kvp.m_key + "=\"" + kvp.m_val + "\"";
  
  ctString xmlContent = xml.AsString();
  for (int64_t childIdx = 0; childIdx < xml.GetChildCount(); ++childIdx)
    xmlContent += "\n" + ctToString(*xml.GetChild(childIdx));
  if (xmlContent.length())
  {
    xmlTag += ">\n  " + xmlContent.replace("\n", "\n  ") + "\n</" + xml.GetTag() + ">";
  }
  else
  {
    xmlTag + "/>";
  }

  return ctString();
}
