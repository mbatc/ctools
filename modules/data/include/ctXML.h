#ifndef atXML_h__
#define atXML_h__

#include "ctString.h"
#include "ctSeek.h"
#include "ctHashMap.h"

class ctXML
{
public:
  ctXML(const ctString &xml = "");
  ctXML(const ctXML &copy);
  ctXML(ctXML &&move);

  ctXML& operator=(ctXML &&rhs);
  ctXML& operator=(const ctXML &rhs);

  bool Parse(const ctString &xml);

  // Tag
  void SetTag(const ctString &tag);
  const ctString& GetTag() const;

  // Attributes
  void SetAttribute(const ctString &name, const ctString &value);
  ctString GetAttribute(const ctString &name) const;
  void SetAttributes(const ctHashMap<ctString, ctString> &attribs);
  const ctHashMap<ctString, ctString>& GetAttributes() const;

  // Values
  double AsFloat() const;
  int64_t AsInt() const;
  bool AsBool() const;
  const ctString& AsString() const;

  void Set(const bool &value);
  void Set(const double &value);
  void Set(const int64_t &value);
  void Set(const ctString &value);

  // Children
  int64_t GetChildCount() const;

  ctXML* GetChild(const int64_t &index);
  const ctXML* GetChild(const int64_t &index) const;

  void AddChild(const ctXML &xml, const int64_t &index = INT64_MAX);
  void RemoveChild(const int64_t &index);

  friend ctString ctToString(const ctXML &xml);

protected:
  static int64_t BuildElement(ctStringSeeker *pSeeker, ctXML *pElem, ctVector<ctString> *pTagStack, ctString *pEndTag);

  ctString m_tag;
  ctString m_value;
  ctHashMap<ctString, ctString> m_attributes;
  ctVector<ctXML> m_children;
};

#endif // atXML_h__
