#ifndef atThreading_h__
#define atThreading_h__

#include <thread>
#include <mutex>

class ctScopeLock
{
public:
  ctScopeLock(std::mutex &_mutex);
  ~ctScopeLock();

private:
  std::mutex &m_mutex;
};

class ctConditionalScopeLock
{
public:
  ctConditionalScopeLock(std::mutex &_mutex, const bool &shouldLock);
  ~ctConditionalScopeLock();

private:
  bool m_isLocked;
  std::mutex &m_mutex;
};

class ctRecursiveScopeLock
{
public:
  ctRecursiveScopeLock(std::recursive_mutex &_mutex);
  ~ctRecursiveScopeLock();

private:
  std::recursive_mutex &m_mutex;
};

class ctConditionalRecursiveScopeLock
{
public:
  ctConditionalRecursiveScopeLock(std::recursive_mutex &_mutex, const bool &shouldLock);
  ~ctConditionalRecursiveScopeLock();

private:
  bool m_isLocked;
  std::recursive_mutex &m_mutex;
};

class ctThreadedObject
{
public:
  enum ControlOption
  {
    CO_Continue,
    CO_Sleep,
    CO_Pause,
    CO_Stop,
  };

  ctThreadedObject(void *pUserData = nullptr);
  ~ctThreadedObject();

  // Cause the thread to start executing
  void Start();

  // Cause the thread to pause as soon as possible
  void Pause();

  // Cause the thread to exit as soon as possible
  void Stop();

  // Cause the thread to sleep for the specified duration before the next call
  // to Process. The duration passed to Sleep is accumulated until it the
  // threads next attempt to sleep. This means if for example, Sleep(10) then Sleep(20) is
  // called before the thread gets a chance to sleep, it will sleep for 30 milliseconds.
  void Sleep(const int64_t duration);

  // Returns true if the thread is still running.
  // Will be false once the thread has exited after a call to Stop()
  bool IsRunning();

  // Returns true if the thread is still active.
  // Will be false once the thread has paused after a call to Pause()
  bool IsPaused();

  // Returns true if the thread is currently asleep.
  bool IsSleeping();

  // Returns the hash of std::thread::id object accosiated with this thread
  uint64_t ID() const;

  // Returns the std::thread::id object accosiated with this thread
  std::thread::id ThreadID() const;

  // This will be called repeatedly until one of the following occurs.
  //  1. CO_Stop is returned (causing the thread to exit)
  //  2. Stop() is called.
  // The return value of this function determines how the thread shall continue
  virtual ControlOption Execute(void *pUserData) = 0;

  // Overload this function to specify the time for the thread to sleep
  // when OC_Sleep is returned from Process()
  // Default is 0 (resulting in a Yield)
  virtual int64_t SleepTime() { return 1; }

private:
  void* m_pUserData;
  
  volatile int64_t m_nextSleepTime;
  volatile bool m_shouldPause;
  volatile bool m_shouldStop;
  volatile bool m_isPaused;
  volatile bool m_isRunning;
  volatile bool m_isSleeping;

  int64_t m_threadID;

  std::mutex m_writeLock;

  void Sleep();

  static void Run(ctThreadedObject *pBase);
  
  // NOTE: The thread should be the last member to be initialized
  //       so it is defined here to be safe
  std::thread m_thread;
};

void ctSleep(const int64_t milliseconds);
void ctYield();

#endif // atThreading_h__
