#include "ctSharedLib.h"

#ifdef ctPLATFORM_LINUX
#include <dlfcn.h>

ctSharedLib::ctSharedLib(const ctFilename &path)
  : m_module(ctNew(Module)(path))
{}

bool ctSharedLib::HasFunction(const ctString &name) { return m_module && m_module->GetFunction(name) != nullptr; }

bool ctSharedLib::IsLoaded() const { return m_module && m_module->IsLoaded(); }

ctFilename ctSharedLib::GetDLLPath() const { return m_module ? m_module->GetPath() : ""; }

ctSharedLib::Module::Module(const ctFilename &path)
  : m_dllPath(path)
{
  pHandle = dlopen(path.Path().replace('/', '\\'), RTLD_NOW);
}

ctSharedLib::Module::~Module()
{
  dlclose(pHandle);
}

ctSharedLib::NullFunc ctSharedLib::Module::GetFunction(const ctString &name)
{
  if (!IsLoaded())
    return nullptr;

  NullFunc *pFunc = functions.TryGet(name);
  if (!pFunc)
  {
    NullFunc newFunc = LoadFunction(name);
    functions.Add(name, newFunc);
    return newFunc;
  }

  return *pFunc;
}

bool ctSharedLib::Module::IsLoaded() const { return pHandle != nullptr; }

const ctFilename &ctSharedLib::Module::GetPath() const { return m_dllPath; }

ctSharedLib::NullFunc ctSharedLib::Module::LoadFunction(const ctString &name)
{
  return IsLoaded() ? (NullFunc)dlsym(pHandle, name.c_str()): nullptr;
}

#endif
