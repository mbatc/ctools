
template<typename T> void ctSerialize(ctObjectDescriptor *pSerialized, const ctVector2<T> &src)
{
  pSerialized->Add("x") = src.x;
  pSerialized->Add("y") = src.y;
}

template<typename T> void ctSerialize(ctObjectDescriptor *pSerialized, const ctVector3<T> &src)
{
  pSerialized->Add("x") = src.x;
  pSerialized->Add("y") = src.y;
  pSerialized->Add("z") = src.z;
}

template<typename T> void ctSerialize(ctObjectDescriptor *pSerialized, const ctVector4<T> &src)
{
  pSerialized->Add("x") = src.x;
  pSerialized->Add("y") = src.y;
  pSerialized->Add("z") = src.z;
  pSerialized->Add("w") = src.w;
}

template<typename T> void ctSerialize(ctObjectDescriptor *pSerialized, const ctMatrix4<T> &src)
{
  pSerialized->SetType(ctObjectDescriptor::OT_Array);
  ctSerialize(pSerialized, &src.m, ctArraySize(src.m));
}

template<typename T> void ctSerialize(ctObjectDescriptor *pSerialized, const ctQuaternion<T> &src)
{
  pSerialized->Add("x") = src.x;
  pSerialized->Add("y") = src.y;
  pSerialized->Add("z") = src.z;
  pSerialized->Add("w") = src.w;
}

template<typename T> void ctSerialize(ctObjectDescriptor *pSerialized, const ctMatrix<T> &src)
{
  pSerialized->SetType(ctObjectDescriptor::OT_Array);
  for (int64_t r = 0; r < src.Rows(); ++r)
  {
    ctObjectDescriptor row = pSerialized->Add("", ctObjectDescriptor::OT_Array);
    for (int64_t c = 0; c < src.Columns(); ++c)
      row.Add("").Serialize(src(r, c));
  }
}

template<typename T> void ctDeserialize(const ctObjectDescriptor &serialized, ctVector2<T> *pDst)
{
  serialized.Get("x").Deserialize(&pDst->x);
  serialized.Get("y").Deserialize(&pDst->y);
  serialized.Get("z").Deserialize(&pDst->z);
}

template<typename T> void ctDeserialize(const ctObjectDescriptor &serialized, ctVector3<T> *pDst)
{
  serialized.Get("x").Deserialize(pDst->x);
  serialized.Get("y").Deserialize(pDst->y);
  serialized.Get("z").Deserialize(pDst->z);
}

template<typename T> void ctDeserialize(const ctObjectDescriptor &serialized, ctVector4<T> *pDst)
{
  serialized.Get("x").Deserialize(pDst->x);
  serialized.Get("y").Deserialize(pDst->y);
  serialized.Get("z").Deserialize(pDst->z);
  serialized.Get("w").Deserialize(pDst->w);
}

template<typename T> void ctDeserialize(const ctObjectDescriptor &serialized, ctMatrix4<T> *pDst)
{
  ctDeserialize(serialized, &src.m, ctArraySize(src.m));
}

template<typename T> void ctDeserialize(const ctObjectDescriptor &serialized, ctQuaternion<T> *pDst)
{
  serialized.Get("x").Deserialize(pDst->x);
  serialized.Get("y").Deserialize(pDst->y);
  serialized.Get("z").Deserialize(pDst->z);
  serialized.Get("w").Deserialize(pDst->w);
}

template<typename T> void ctDeserialize(const ctObjectDescriptor &serialized, ctMatrix<T> *pDst)
{
  int64_t nRows = serialized.GetMemberCount();
  int64_t nCols = 0;
  for (int64_t r = 0; r < nRows; ++r)
    nCols = ctMax(serialized.Get(r).GetMemberCount(), nCols);

  *pDst = ctMatrix<T>(nCols, nRows);
  for (int64_t r = 0; r < nRows; ++r)
  {
    ctObjectDescriptor row = serialized.Get(r);
    int64_t memberCount = row.GetMemberCount();
    for (int64_t c = 0; c < memberCount; ++c)
      row.Get(c).Deserialize(&(*pDst)(r, c));
  }
}

template<typename T> void ctSerialize(ctObjectDescriptor *pSerialized, const T *pSrc, int64_t count)
{
  pSerialized->SetType(ctObjectDescriptor::OT_Array);
  for (const T &val : ctIterate(pSrc, count))
    pSerialized->Add("") = val;
}

// Containers
template<typename T> void ctSerialize(ctObjectDescriptor *pSerialized, const ctVector<T> &src) { ctSerialize(pSerialized, src.data(), src.size()); }

template<typename Key, typename Value> void ctSerialize(ctObjectDescriptor *pSerialized, const ctHashMap<Key, Value> &src)
{
  pSerialized->SetType(ctObjectDescriptor::OT_Object);
  for (const ctKeyValue<Key, Value> &kvp : src)
    pSerialized->Add(ctToString(kvp.m_key)) = kvp.m_val;
}

template<typename T> void ctDeserialize(const ctObjectDescriptor &serialized, T *pDst, int64_t count)
{
  if (!serialized.IsArray())
    return;

  for (int64_t i = 0; i < count; ++i)
    serialized.Get(i).Deserialize(pDst + i);
}

template<typename T> void ctDeserialize(const ctObjectDescriptor &serialized, ctVector<T> *pDst)
{
  pDst->resize(serialized.GetMemberCount());
  ctDeserialize(serialized, pDst->data(), pDst->size());
}

template<typename Key, typename Value> void ctDeserialize(const ctObjectDescriptor &serialized, ctHashMap<Key, Value> *pDst)
{
  if (serialized.GetValueType() != ctObjectDescriptor::OT_Object)
    return;

  for (const ctObjectDescriptor &member : serialized.GetMembers())
    pDst->TryAdd(ctFromString<Key>(member.GetName()), member.Deserialize<Value>());
}
