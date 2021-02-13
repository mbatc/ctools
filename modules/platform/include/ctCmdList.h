#ifndef ctCmdList_h__
#define ctCmdList_h__

#include "ctString.h"
#include "ctHashMap.h"

class ctCmdList
{
public:
  ctCmdList(const ctVector<ctString> &cmds);
  ctCmdList(int argc, char **argv);

  bool HasNext();
  ctString Next();;
  int64_t GetCount() const;
  int64_t GetCount(const ctString &cmd) const;

  // Get parameters for a cmd line switch
  ctString Get(const ctString &cmd, const int64_t &idx = 0);

  // Get parameters for a cmd line switch
  ctString operator[](const ctString &cmd);

protected:
  void Parse(const ctVector<ctString> &cmds);

  int64_t m_next = 0;
  ctVector<ctString> m_argList;

  ctHashMap<ctString, ctVector<ctString>> m_switches;
};

#endif // ctCmdList_h__
