#include "ctThreading.h"

ctScopeLock::ctScopeLock(std::mutex &_mutex) : m_mutex(_mutex) { m_mutex.lock(); }
ctScopeLock::~ctScopeLock() { m_mutex.unlock(); }
ctRecursiveScopeLock::ctRecursiveScopeLock(std::recursive_mutex &_mutex) : m_mutex(_mutex) { m_mutex.lock(); }
ctRecursiveScopeLock::~ctRecursiveScopeLock() { m_mutex.unlock(); }

void ctSleep(const int64_t milliseconds)
{
  if (milliseconds == 0)
    ctYield();
  else
    std::this_thread::sleep_for(std::chrono::duration<int64_t, std::milli>(milliseconds));
}

void ctYield() { std::this_thread::yield(); }

ctConditionalScopeLock::ctConditionalScopeLock(std::mutex &_mutex, const bool &shouldLock)
  : m_mutex(_mutex)
  , m_isLocked(shouldLock)
{
  if (shouldLock)
    m_mutex.lock();
}

ctConditionalScopeLock::~ctConditionalScopeLock()
{
  if (m_isLocked)
    m_mutex.unlock();
}

ctConditionalRecursiveScopeLock::ctConditionalRecursiveScopeLock(std::recursive_mutex &_mutex, const bool &shouldLock)
  : m_mutex(_mutex)
  , m_isLocked(shouldLock)
{
  if (shouldLock)
    m_mutex.lock();
}

ctConditionalRecursiveScopeLock::~ctConditionalRecursiveScopeLock()
{
  if (m_isLocked)
    m_mutex.unlock();
}

ctThreadedObject::ctThreadedObject(void *pUserData)
  : m_isPaused(true)
  , m_pUserData(pUserData)
  , m_shouldPause(true)
  , m_isRunning(true)
  , m_shouldStop(false)
  , m_nextSleepTime(0)
  , m_threadID(0)
  , m_thread(&ctThreadedObject::Run, this)
{
  m_threadID = (uint64_t)std::hash<std::thread::id>()(m_thread.get_id());
}

ctThreadedObject::~ctThreadedObject()
{
  Stop();
  while (IsRunning())
    ctSleep(1);
  m_thread.join();
}

void ctThreadedObject::Start()
{
  m_shouldPause = false;
  m_shouldStop = false;
}

void ctThreadedObject::Sleep(const int64_t duration)
{
  ctScopeLock lock(m_writeLock);
  m_nextSleepTime += duration;
}

void ctThreadedObject::Run(ctThreadedObject *pBase)
{
  while (pBase->IsRunning())
  {
    if (!pBase->IsPaused())
    {
      switch (pBase->Execute(pBase->m_pUserData))
      {
      case CO_Pause: pBase->Pause(); break;
      case CO_Sleep: pBase->Sleep(); break;
      case CO_Stop: pBase->Stop(); break;
      case CO_Continue: default: break;
      }
    }
    else
    {
      pBase->Sleep();
    }

    {
      ctScopeLock lock(pBase->m_writeLock);
      pBase->m_isPaused = pBase->m_shouldPause;
      pBase->m_isRunning = !pBase->m_shouldStop;
    }
  }
}

void ctThreadedObject::Sleep()
{
  m_isSleeping = true;
  ctSleep(SleepTime() + m_nextSleepTime);
  
  m_writeLock.lock();
  m_nextSleepTime = 0; // Lock here as two threads could write to this variable
  m_writeLock.unlock();

  m_isSleeping = false;
}

void ctThreadedObject::Pause() { m_shouldPause = true; }
void ctThreadedObject::Stop() { m_shouldStop = true; }

bool ctThreadedObject::IsRunning()
{
  ctScopeLock lock(m_writeLock);
  return m_isRunning;
}

bool ctThreadedObject::IsPaused()
{
  ctScopeLock lock(m_writeLock);
  return m_isPaused;
}
bool ctThreadedObject::IsSleeping()
{
  ctScopeLock lock(m_writeLock);
  return m_isSleeping;
}

uint64_t ctThreadedObject::ID() const { return m_threadID; }
std::thread::id ctThreadedObject::ThreadID() const { return m_thread.get_id(); }
