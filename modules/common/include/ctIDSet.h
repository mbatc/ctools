#ifndef ctIDSet_h__
#define ctIDSet_h__

#include "ctHashMap.h"
#include <random>

using ctID = uint64_t;

class ctIDSet
{
public:
  // Generate a new unique ID that is not in the set
  ctID Generate();
  // Check if an ID is used in the set
  bool Contains(ctID id) const;
  // Add an ID to the set. Fails if it is already used.
  bool Take(ctID id);
  // Remove an ID from the set. Fails if it is not used.
  bool Free(ctID id);
  // Get all IDs in the set
  ctVector<ctID> GetIDs() const;

private:
  ctID RandomID();

  std::uniform_int_distribution<int> m_generator;
  std::default_random_engine m_engine;
  ctHashMap<ctID, bool> m_ids;
};

#endif // ctIDSet_h__
