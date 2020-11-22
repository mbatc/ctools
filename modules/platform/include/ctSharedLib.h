#ifndef atDLL_h__
#define atDLL_h__

#include "file/ctFilename.h"
#include "ctPtr.h"
#include <type_traits>

class ctSharedLib
{
  typedef void(*NullFunc)();
public:
  ctSharedLib() = default;
  ctSharedLib(ctSharedLib &&o) = default;
  ctSharedLib(const ctSharedLib &o) = default;
  ctSharedLib(const ctFilename &path);

  ctSharedLib& operator=(ctSharedLib &&o) = default;
  ctSharedLib& operator=(const ctSharedLib & o) = default;

  // Get a function in the DLL as a callable object
  // The signature specified must match the exported function in the DLL
  template<typename FuncSig> std::function<FuncSig> Get(const ctString &name);

  // Call a function in the DLL.
  // The return type and arguments must match the exported function in the DLL.
  // This function is not safe to call if the DLL does not export a function with 'name'
  // If the functions existence is not certain, use Get(name), check the result and call the return function instead.
  template<typename ReturnType, typename... Args> ReturnType Call(const ctString &name, Args&&... args);

  // Check if the loaded DLL contains a function with the given name
  bool HasFunction(const ctString &name);

  // Returns true if the DLL was successfully loaded
  bool IsLoaded() const;

  // Return the path of the DLL this object refers to
  ctFilename GetDLLPath() const;

protected:
  class Module
  {
  public:
    Module(const ctFilename &path);
    ~Module();

    NullFunc GetFunction(const ctString &name);
    bool IsLoaded() const;
    const ctFilename& GetPath() const;

  protected:
    NullFunc LoadFunction(const ctString &name);

    void *pHandle = nullptr;
    ctHashMap<ctString, NullFunc> functions;
    ctFilename m_dllPath;
  };

  ctPtr<Module> m_module;
};

template<typename FuncSig>
inline std::function<FuncSig> ctSharedLib::Get(const ctString &name)
{
  if (!m_module)
    return nullptr;

  std::function<FuncSig> callable = (FuncSig*)m_module->GetFunction(name);
  return callable;
}

template<typename ReturnType, typename ...Args>
inline ReturnType ctSharedLib::Call(const ctString &name, Args&&... args)
{
  using FuncSig = std::decay_t<ReturnType>(std::decay_t<Args>...);
  std::function<FuncSig> callable = Get<FuncSig>(name);
  return callable(std::forward<Args>(args)...);
}

#endif // atDLL_h__
