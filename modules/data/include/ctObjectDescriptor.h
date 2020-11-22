#ifndef atObjectDescriptor_h__
#define atObjectDescriptor_h__

#include "ctXML.h"
#include "ctJSON.h"
#include "ctPool.h"
#include "ctScan.h"

class ctObjectDescriptor
{
public:
  enum ObjectType
  {
    OT_Null,
    OT_Object,
    OT_Array,
    OT_Value,
    OT_Count
  };

  enum ValueType
  {
    VT_None,
    VT_String,
    VT_Bool,
    VT_Int,
    VT_Float,
    VT_Count
  };

protected:
  struct NodeData
  {
    ctString name;
    ctString value;
    ctVector<int64_t> children;

    ObjectType type = OT_Null;
    ValueType valueType = VT_None;
  };

  struct NodeTree
  {
    int64_t refCount = 1;
    ctPool<NodeData> nodes;
  };

  ctObjectDescriptor(NodeTree *pTree, int64_t nodeID = CT_INVALID_ID);

  ctObjectDescriptor(const ctString &name, const ObjectType &type, NodeTree *pTree);

public:
  ctObjectDescriptor(const ObjectType &type = OT_Value);

  ctObjectDescriptor(ctObjectDescriptor &&move);
  ctObjectDescriptor(const ctObjectDescriptor &copy);

  ctObjectDescriptor(const ctXML &xml);
  ctObjectDescriptor(const ctJSON &json);
  
  template<typename T> explicit ctObjectDescriptor(const T &o);
  template<typename T> ctObjectDescriptor operator=(const T &o);

  ctObjectDescriptor &operator=(ctObjectDescriptor &&move);
  ctObjectDescriptor &operator=(const ctObjectDescriptor &copy);


  ~ctObjectDescriptor();

  void Import(const ctXML &xml);
  void Import(const ctJSON &json);

  void Export(ctXML *pXML) const;
  void Export(ctJSON *pJSON) const;

  ctString GetName() const;

  void SetType(const ObjectType &type);

  ctObjectDescriptor Add(const ctString &name, const ObjectType &type = OT_Value);
  ctObjectDescriptor Add(const ctObjectDescriptor &obj);

  ctObjectDescriptor Set(const bool &value);
  ctObjectDescriptor Set(const int64_t &value);
  ctObjectDescriptor Set(const double &value);
  ctObjectDescriptor Set(const ctString &value);
  ctObjectDescriptor Set(const ctObjectDescriptor &value);
  ctObjectDescriptor SetFromString(const ctString &value);

  ctObjectDescriptor Get(const ctString &name) const;
  ctObjectDescriptor Get(const int64_t &index) const;
  ctObjectDescriptor GetOrAdd(const ctString &name);

  bool Remove(const int64_t &index);
  bool Remove(const ctString &name);

  int64_t Find(const ctString &name) const;

  int64_t GetMemberCount() const;
  ctVector<ctString> GetMemberNames() const;
  ctVector<ctObjectDescriptor> GetMembers() const;

  bool AsBool(const bool &defaultVal = false) const;
  int64_t AsInt(const int64_t &defaultVal = 0) const;
  double AsFloat(const double &defaultVal = 0) const;
  ctString AsString(const ctString &defaultVal = "") const;

  bool IsNull() const;
  bool IsValue() const;
  bool IsArray() const;
  bool IsObject() const;

  const ValueType& GetValueType() const;
  const ObjectType& GetObjectType() const;

  void Clear();

  // Serialize 'value' into this object descriptor
  template<typename T> ctObjectDescriptor Serialize(const T &value);

  // Deserialize this object descriptor into 'pValue'
  template<typename T> T Deserialize() const;
  template<typename T> void Deserialize(T *pValue) const;

  ctObjectDescriptor operator[](const int64_t &idx);
  ctObjectDescriptor operator[](const ctString &name);

  ctObjectDescriptor operator[](const int64_t &idx) const;
  ctObjectDescriptor operator[](const ctString &name) const;

  bool operator==(const ctObjectDescriptor &rhs) const;
  bool operator!=(const ctObjectDescriptor &rhs) const;

  friend int64_t ctStreamRead(ctReadStream *pStream, ctObjectDescriptor *pData, const int64_t count);
  friend int64_t ctStreamWrite(ctWriteStream *pStream, ctObjectDescriptor *pData, const int64_t count);

protected:
  ctObjectDescriptor SetValue(const ctString &value, ValueType type);

  NodeData& GetNode();
  const NodeData& GetNode() const;

  void AquireNode(NodeTree *pTree, const int64_t &nodeID);

  void ReleaseTree();

  int64_t m_nodeID = CT_INVALID_ID;
  NodeTree *m_pTree = nullptr;
};

ctTrivialStreamRead(ctObjectDescriptor::ObjectType);
ctTrivialStreamRead(ctObjectDescriptor::ValueType);
ctTrivialStreamWrite(ctObjectDescriptor::ObjectType);
ctTrivialStreamWrite(ctObjectDescriptor::ValueType);

#include "ctObjectDescriptor.inl"
#endif // atObjectDescriptor_h__
