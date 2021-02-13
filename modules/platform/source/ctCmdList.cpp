#include "ctCmdList.h"

ctCmdList::ctCmdList(int argc, char **argv)
{
  ctVector<ctString> argList(argc);
  for (int64_t i = 0; i < argc; ++i)
    argList.push_back(argv[i]);
  Parse(argList);
}

ctCmdList::ctCmdList(const ctVector<ctString> &cmds)
{
  Parse(cmds);
}

bool ctCmdList::HasNext()
{
  return m_next < m_argList.size();
}

ctString ctCmdList::Next()
{
  return HasNext() ? m_argList[m_next++] : "";
}

int64_t ctCmdList::GetCount(const ctString &cmd) const
{
  const ctVector<ctString> *pArgs = m_switches.TryGet(cmd);
  return pArgs ? pArgs->size() : 0;
}

int64_t ctCmdList::GetCount() const
{
  return m_argList.size();
}

ctString ctCmdList::Get(const ctString &cmd, const int64_t &idx /*= 0*/)
{
  const ctVector<ctString> *pArgs = m_switches.TryGet(cmd);
  return pArgs && idx < pArgs->size() ? pArgs->at(idx) : "";
}

ctString ctCmdList::operator[](const ctString &cmd)
{
  return Get(cmd);
}

void ctCmdList::Parse(const ctVector<ctString> &cmds)
{
  ctString curSwitch = "";
  m_switches.Add("", {});
  for (int i = 0; i < cmds.size(); ++i)
  {
    m_argList.push_back(cmds[i]);

    if (m_argList.back().starts_with("-"))
    {
      curSwitch = m_argList.back().trim_start("-");
      m_switches.TryAdd(curSwitch, {});
    }
    else
    {
      m_switches[curSwitch].push_back(m_argList.back());
    }
  }
}
