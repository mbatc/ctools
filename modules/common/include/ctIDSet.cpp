#include "ctIDSet.h"

ctID ctIDSet::Generate()
{
  ctID id;
  do
  {
    id = RandomID();
  } while (!Take(id));

  return id;
}

bool ctIDSet::Contains(ctID id) const
{
  return m_ids.Contains(id);
}

bool ctIDSet::Take(ctID id)
{
  return m_ids.TryAdd(id, false);
}

bool ctIDSet::Free(ctID id)
{
  return m_ids.Remove(id);
}

ctVector<ctID> ctIDSet::GetIDs() const
{
  return m_ids.GetKeys();
}

ctID ctIDSet::RandomID()
{
  union
  {
    struct
    {
      int a;
      int b;
    };
    ctID id;
  } data;

  data.a = m_generator(m_engine);
  data.b = m_generator(m_engine);

  return data.id;
}
