#include "ctObjectDescriptor.h"

static ctString _objectTypeName[ctObjectDescriptor::OT_Count] =
{
  "null",
  "object",
  "array",
  "value"
};

static ctString _valueTypeName[ctObjectDescriptor::VT_Count] =
{
  "none",
  "string",
  "bool",
  "int",
  "float"
};

static ctObjectDescriptor::ValueType _GetValueType(const ctString &name)
{
  for (int64_t i = 0; i < ctObjectDescriptor::VT_Count; ++i)
    if (_valueTypeName[i].compare(name, atSCO_None))
      return ctObjectDescriptor::ValueType(i);
  return ctObjectDescriptor::VT_None;
}

static ctObjectDescriptor::ObjectType _GetObjectType(const ctString &name)
{
  for (int64_t i = 0; i < ctObjectDescriptor::OT_Count; ++i)
    if (_objectTypeName[i].compare(name, atSCO_None))
      return ctObjectDescriptor::ObjectType(i);
  return ctObjectDescriptor::OT_Value;
}

ctObjectDescriptor &ctObjectDescriptor::operator=(const ctObjectDescriptor &copy)
{
  if (&copy == this)
    return *this;

  ReleaseTree();
  AquireNode(copy.m_pTree, copy.m_nodeID);
  return *this;
}

ctObjectDescriptor &ctObjectDescriptor::operator=(ctObjectDescriptor &&move)
{
  if (&move == this)
    return *this;

  ReleaseTree();
  AquireNode(move.m_pTree, move.m_nodeID);
  move.ReleaseTree();
  return *this;
}

ctObjectDescriptor::ctObjectDescriptor(const ctObjectDescriptor &copy)
{
  *this = copy;
}

ctObjectDescriptor::ctObjectDescriptor(const ctXML &xml) : ctObjectDescriptor(OT_Null) { Import(xml); }
ctObjectDescriptor::ctObjectDescriptor(const ctJSON &json) : ctObjectDescriptor(OT_Null) { Import(json); }

ctObjectDescriptor::ctObjectDescriptor(ctObjectDescriptor &&move) { *this = std::move(move); }

ctObjectDescriptor::ctObjectDescriptor(const ObjectType &type /*= OT_Value*/)
  : m_pTree(ctNew(NodeTree))
{
  m_nodeID = m_pTree->nodes.emplace();
  SetType(type);
}

ctObjectDescriptor::ctObjectDescriptor(const ctString &name, const ObjectType &type, NodeTree *pTree)
{
  NodeData nodeData;
  nodeData.type = type;
  nodeData.name = name;
  int64_t nodeID = pTree->nodes.Add(nodeData);
  AquireNode(pTree, nodeID);
}

ctObjectDescriptor::ctObjectDescriptor(NodeTree *pTree, int64_t nodeID /*= AT_INVALID_ID*/) { AquireNode(pTree, nodeID); }
ctObjectDescriptor::~ctObjectDescriptor() { ReleaseTree(); }

void ctObjectDescriptor::Import(const ctJSON &json)
{
  if (json.IsArray())
  {
    SetType(OT_Array);
    for (const ctJSON &member : json.Array())
      Add("").Import(member);
  }
  else if (json.IsObject())
  {
    SetType(OT_Object);
    for (const ctKeyValue<ctString, ctJSON> &member : json.Object())
      Add(member.m_key).Import(member.m_val);
  }
  else if (json.IsValue())
  {
    SetFromString(json.Value());
  }
  else if (json.IsString())
  {
    Set(json.Value());
  }
  else if (json.IsNull())
  {
    SetType(OT_Null);
  }
}

void ctObjectDescriptor::Import(const ctXML &xml)
{
  if (xml.GetChildCount() > 0)
  { // Object or array
    ObjectType ot = _GetObjectType(xml.GetAttribute("type"));
    if (ot != OT_Object && ot != OT_Array)
      ot = OT_Object;

    SetType(ot);
    for (int64_t i = 0; i < xml.GetChildCount(); ++i)
    {
      const ctXML *pXml = xml.GetChild(i);
      Add(ot == OT_Array ? "" : pXml->GetTag()).Import(*pXml);
    }
  }
  else
  {
    ValueType vt = _GetValueType(xml.GetAttribute("data-type"));
    ctString value = xml.AsString();
    if (vt == VT_None)
    {
      if (value.length() > 0)
        SetFromString(value);
      else
        SetType(OT_Null);
    }
    else
    {
      SetValue(value, vt);
    }
  }
}

void ctObjectDescriptor::Export(ctJSON *pJSON) const
{
  ObjectType type = GetObjectType();
  switch (type)
  {
  case OT_Value:
    pJSON->SetValue(AsString(), GetValueType() == VT_String);
    break;
  case OT_Array:
    pJSON->MakeArray();
    for (int64_t i = 0; i < GetMemberCount(); ++i)
    {
      ctJSON childJson;
      Get(i).Export(&childJson);
      pJSON->SetElement(i, childJson);
    }
    break;
  case OT_Object:
    pJSON->MakeObject();
    for (const ctObjectDescriptor &member : GetMembers())
    {
      ctJSON childJson;
      member.Export(&childJson);
      pJSON->SetMember(member.GetName(), childJson);
    }
    break;
  }
}

void ctObjectDescriptor::Export(ctXML *pXML) const
{
  ObjectType type = GetObjectType();
  if (type != OT_Value)
    pXML->SetAttribute("type", _objectTypeName[type]);
 
  switch (type)
  {
  case OT_Value:
    pXML->SetAttribute("data-type", _valueTypeName[GetValueType()]);
    pXML->Set(AsString());
  break;
  case OT_Array:
    pXML->SetAttribute("count", ctString(GetMemberCount()));
    for (const ctObjectDescriptor &member : GetMembers())
    {
      ctXML childXml;
      childXml.SetTag("i");
      member.Export(&childXml);
      pXML->AddChild(childXml);
    }
    break;
  case OT_Object:
    for (const ctObjectDescriptor &member : GetMembers())
    {
      ctXML childXml;
      childXml.SetTag(member.GetName());
      member.Export(&childXml);
      pXML->AddChild(childXml);
    }
    break;
  }
  ctUnused(pXML);
}

ctString ctObjectDescriptor::GetName() const { return GetNode().name; }

void ctObjectDescriptor::SetType(const ObjectType &type)
{
  NodeData &data = GetNode();
  if (data.type == type)
    return;
  Clear();
  data.type = type;
}

ctObjectDescriptor ctObjectDescriptor::Add(const ctString &name, const ObjectType &type /*= OT_Value*/)
{
  if (GetObjectType() == OT_Value)
    SetType(name.length() > 0 ? OT_Object : OT_Array);
  int64_t idx = Find(name);
  if (idx >= 0)
  {
    ctObjectDescriptor o = Get(idx);
    o.SetType(type);
    return o;
  }

  ctObjectDescriptor o(name, type, m_pTree);
  GetNode().children.push_back(o.m_nodeID);
  return o;
}

ctObjectDescriptor ctObjectDescriptor::Add(const ctObjectDescriptor &obj) { return Add(obj.GetName(), obj.GetObjectType()).Set(obj); }

ctObjectDescriptor ctObjectDescriptor::Set(const ctString &value) { return SetValue(value, VT_String); }
ctObjectDescriptor ctObjectDescriptor::Set(const double &value) { return SetValue(ctString(value), VT_Float); }
ctObjectDescriptor ctObjectDescriptor::Set(const int64_t &value) { return SetValue(ctString(value), VT_Int); }
ctObjectDescriptor ctObjectDescriptor::Set(const bool &value) { return SetValue(value ? "true" : "false", VT_Bool); }

ctObjectDescriptor ctObjectDescriptor::Set(const ctObjectDescriptor &value)
{
  SetType(value.GetObjectType());
  if (IsValue())
  {
    SetValue(value.AsString(), value.GetValueType());
  }
  else
  {
    for (const ctObjectDescriptor &member : value.GetMembers())
      Add(member);
  }

  return *this;
}

ctObjectDescriptor ctObjectDescriptor::SetFromString(const ctString &value)
{
  int64_t len = 0;

  // Try Int
  ctScan::Int(value, &len);
  if (len == value.length())
    return SetValue(value, VT_Int);
  
  // Try as float
  ctScan::Float(value, &len);
  if (len == value.length())
    return SetValue(value, VT_Float);

  // Try as bool
  ctScan::Bool(value, &len);
  if (len == value.length())
    return SetValue(value, VT_Bool);

  // Default to string
  return Set(value);
}

ctObjectDescriptor ctObjectDescriptor::Get(const int64_t &index) const
{
  const NodeData &node = GetNode();
  if (index < 0 || index >= node.children.size())
    return ctObjectDescriptor();
  return ctObjectDescriptor(m_pTree, node.children[index]);
}

ctObjectDescriptor ctObjectDescriptor::GetOrAdd(const ctString &name)
{
  int64_t idx = Find(name);
  return idx == CT_INVALID_ID ? Add(name) : Get(idx);
}

ctObjectDescriptor ctObjectDescriptor::Get(const ctString &name) const { return Get(Find(name)); }

bool ctObjectDescriptor::Remove(const ctString &name)
{
  return Remove(Find(name));
}

bool ctObjectDescriptor::Remove(const int64_t &index)
{
  if (index < 0 || index >= GetMemberCount())
    return false;
  ctObjectDescriptor child = Get(index);
  child.Clear();

  int64_t id = child.m_nodeID;
  child.ReleaseTree();
  m_pTree->nodes.erase(id);
  return true;
}

int64_t ctObjectDescriptor::Find(const ctString &name) const
{
  if (name.length() == 0)
    return -1;

  for (int64_t i = 0; i < GetMemberCount(); ++i)
    if (Get(i).GetName() == name)
      return i;
  return -1;
}

int64_t ctObjectDescriptor::GetMemberCount() const
{
  return GetNode().children.size();
}

ctVector<ctString> ctObjectDescriptor::GetMemberNames() const
{
  ctVector<ctString> names;
  for (int64_t i = 0; i < GetMemberCount(); ++i)
    names.push_back(Get(i).GetName());
  return names;
}

ctVector<ctObjectDescriptor> ctObjectDescriptor::GetMembers() const
{
  ctVector<ctObjectDescriptor> members;
  for (int64_t i = 0; i < GetMemberCount(); ++i)
    members.push_back(Get(i));
  return members;
}

bool ctObjectDescriptor::AsBool(const bool &defaultVal) const { return IsValue() ? ctScan::Bool(GetNode().value) : defaultVal; }
int64_t ctObjectDescriptor::AsInt(const int64_t &defaultVal) const { return IsValue() ? ctScan::Int(GetNode().value) : defaultVal; }
double ctObjectDescriptor::AsFloat(const double &defaultVal) const { return IsValue() ? ctScan::Float(GetNode().value) : defaultVal; }
ctString ctObjectDescriptor::AsString(const ctString &defaultVal) const { return IsValue() ? GetNode().value : defaultVal; }

bool ctObjectDescriptor::IsNull() const { return GetObjectType() == OT_Null; }
bool ctObjectDescriptor::IsValue() const { return GetObjectType() == OT_Value; }
bool ctObjectDescriptor::IsArray() const { return GetObjectType() == OT_Array; }
bool ctObjectDescriptor::IsObject() const { return GetObjectType() == OT_Object; }

const ctObjectDescriptor::ValueType &ctObjectDescriptor::GetValueType() const { return GetNode().valueType; }
const ctObjectDescriptor::ObjectType &ctObjectDescriptor::GetObjectType() const { return GetNode().type; }

void ctObjectDescriptor::Clear()
{
  while (int64_t count = GetMemberCount() > 0)
    Remove(count - 1);

  NodeData &node = GetNode();
  node.value = "";
  node.type = OT_Null;
  node.valueType = VT_None;
}

ctObjectDescriptor ctObjectDescriptor::operator[](const ctString &name) const { return Get(name); }

bool ctObjectDescriptor::operator==(const ctObjectDescriptor &rhs) const
{
  if (GetObjectType() != rhs.GetObjectType()) return false;
  if (GetValueType() != rhs.GetValueType()) return false;
  if (GetMemberCount() != rhs.GetMemberCount()) return false;

  if (IsArray())
  { // We care about the order if it is an array
    for (int64_t i = 0; i < GetMemberCount(); ++i)
      if (Get(i) != rhs.Get(i))
        return false;
  }
  else if (IsObject())
  { // We care about the names if it is an object
    for (const ctString &member : GetMemberNames())
      if (Get(member) != rhs.Get(member))
        return false;
  }
  else if (AsString() != rhs.AsString())
      return false;
  return true;
}

bool ctObjectDescriptor::operator!=(const ctObjectDescriptor &rhs) const { return !(*this == rhs); }

ctObjectDescriptor ctObjectDescriptor::operator[](const int64_t &idx) const { return Get(idx); }

ctObjectDescriptor ctObjectDescriptor::operator[](const ctString &name) { return Get(name); }
ctObjectDescriptor ctObjectDescriptor::operator[](const int64_t &idx) { return Get(idx); }

ctObjectDescriptor ctObjectDescriptor::SetValue(const ctString &value, ValueType type)
{
  SetType(OT_Value);
  NodeData &node = GetNode();
  node.value = value;
  node.valueType = type;
  return *this;
}

const ctObjectDescriptor::NodeData &ctObjectDescriptor::GetNode() const { return m_pTree->nodes[m_nodeID]; }
ctObjectDescriptor::NodeData &ctObjectDescriptor::GetNode() { return m_pTree->nodes[m_nodeID]; }

void ctObjectDescriptor::AquireNode(NodeTree *pTree, const int64_t &nodeID)
{
  m_nodeID = CT_INVALID_ID;
  if (!pTree)
    return;

  ++pTree->refCount;
  m_pTree = pTree;
  m_nodeID = nodeID;
}

void ctObjectDescriptor::ReleaseTree()
{
  if (!m_pTree)
    return;

  if (--m_pTree->refCount == 0)
    ctDelete(m_pTree);

  m_pTree = nullptr;
  m_nodeID = CT_INVALID_ID;
}

int64_t ctStreamRead(ctReadStream *pStream, ctObjectDescriptor *pData, const int64_t count)
{
  int64_t size = 0;
  for (int64_t i = 0; i < count; ++i)
  {
    pData[i].Clear();
    ctObjectDescriptor::NodeData &node = pData[i].GetNode();
    size += ctStreamRead(pStream, &node.name, 1);
    size += ctStreamRead(pStream, &node.type, 1);
    size += ctStreamRead(pStream, &node.valueType, 1);
    size += ctStreamRead(pStream, &node.value, 1);

    int64_t childCount = 0;
    size += ctStreamRead(pStream, &childCount, 1);
    for (int64_t i = 0; i < childCount; ++i)
    {
      ctObjectDescriptor child = pData[i].Add("");
      ctStreamRead(pStream, &child, 1);
    }
  }
  return size;
}

int64_t ctStreamWrite(ctWriteStream *pStream, ctObjectDescriptor *pData, const int64_t count)
{
  int64_t size = 0;
  for (int64_t i = 0; i < count; ++i)
  {
    ctObjectDescriptor::NodeData &node = pData[i].GetNode();
    size += ctStreamWrite(pStream, &node.name, 1);
    size += ctStreamWrite(pStream, &node.type, 1);
    size += ctStreamWrite(pStream, &node.valueType, 1);
    size += ctStreamWrite(pStream, &node.value, 1);

    size += ctStreamWrite(pStream, &node.children.size(), 1);
    for (int64_t i = 0; i < pData[i].GetMemberCount(); ++i)
    {
      ctObjectDescriptor child = pData[i].Get(i);
      size += ctStreamWrite(pStream, &child, 1);
    }
  }

  return size;
}
