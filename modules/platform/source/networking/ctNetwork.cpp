#include "networking/ctNetwork.h"

static const int64_t recvBlockSize = 512;

enum _atConnectionJobType
{
  _atCJT_Send,
  _atCJT_Host,
  _atCJT_Connect,
  _atCJT_Recieve,
  _atCJT_Disconnect
};

struct _atReceiveData {};
struct _atDisconnectData {};
struct _atSendData { ctVector<uint8_t> buffer; };
struct _atConnectData { ctString url; uint16_t port; };
struct _atHostData { uint16_t port; };

struct ConnectionJob
{
  _atConnectionJobType jobType;
  
  ctNetwork::Connection *pConnection = nullptr;
  void *pJobData = nullptr;

  bool done = false;
  bool failed = false;
  int64_t refs = 0;
};

bool _DoHostJob(ctNetwork::Connection *pConnection, _atHostData *pData);
bool _DoConnectJob(ctNetwork::Connection *pConnection, _atConnectData *pData);
bool _DoSendJob(ctNetwork::Connection *pConnection, _atSendData *pData);
void _DoRecieveJob(ctNetwork::Connection *pConnection, _atReceiveData *pData);
void _DoDisconnectJob(ctNetwork::Connection *pConnection, _atDisconnectData *pData);

ctNetwork::ctNetwork(const bool asyncJobs)
  : m_running(true)
{
  if (asyncJobs)
    m_pJobThread = ctNew(std::thread)(&ctNetwork::ProcessJobs, this);
}

ctNetwork::~ctNetwork()
{
  CloseAllConnections();
  m_running = false;
  if (m_pJobThread)
  {
    m_pJobThread->join();
    ctDelete(m_pJobThread);
    m_pJobThread = nullptr;
  }
}

ctNetwork::JobStatus ctNetwork::Host(const uint16_t &port)
{
  // Create host data
  _atHostData *pData = ctNew(_atHostData);
  pData->port = port;

  // Create Send Job
  ConnectionJob *pJob = ctNew(ConnectionJob);
  pJob->pConnection = ctNew(Connection);
  pJob->pJobData = (void*)pData;
  pJob->jobType = _atCJT_Host;

  ctConditionalScopeLock lock(m_connectionLock, m_pJobThread != nullptr);
  int64_t handle = GetNextHandle();
  m_connections.Add(handle, pJob->pConnection);

  return QueueJob(pJob, handle);
}

ctNetwork::JobStatus ctNetwork::Connect(const ctString &url, const uint16_t &port)
{
  // Create connect data
  _atConnectData *pData = ctNew(_atConnectData);
  pData->port = port;
  pData->url = url;

  // Create Send Job
  ConnectionJob *pJob = ctNew(ConnectionJob);
  pJob->pConnection = ctNew(Connection);
  pJob->pJobData = (void*)pData;
  pJob->jobType = _atCJT_Connect;

  ctConditionalScopeLock lock(m_connectionLock, m_pJobThread != nullptr);
  int64_t handle = GetNextHandle();
  m_connections.Add(handle, pJob->pConnection);
  return QueueJob(pJob, handle);
}

ctNetwork::JobStatus ctNetwork::Receive(const ctConnectionHandle &handle)
{
  ctConditionalScopeLock lock(m_connectionLock, m_pJobThread != nullptr);
  Connection **ppCon = m_connections.TryGet(handle);
  if (!ppCon)
    return JobStatus(nullptr, -1);

  // Create receive data
  _atReceiveData *pData = ctNew(_atReceiveData);

  // Create Send Job
  ConnectionJob *pJob = ctNew(ConnectionJob);
  pJob->pConnection = *ppCon;
  pJob->pJobData = (void*)pData;
  pJob->jobType = _atCJT_Recieve;
  return QueueJob(pJob, handle);
}

ctNetwork::JobStatus ctNetwork::Send(const ctConnectionHandle &handle, const ctVector<uint8_t> &data)
{
  ctConditionalScopeLock lock(m_connectionLock, m_pJobThread != nullptr);
  Connection **ppCon = m_connections.TryGet(handle);
  if (!ppCon)
    return JobStatus(nullptr, -1);

  // Create send data
  _atSendData *pData = ctNew(_atSendData);
  pData->buffer = data;

  // Create Send Job
  ConnectionJob *pJob = ctNew(ConnectionJob);
  pJob->pConnection = *ppCon;
  pJob->pJobData = (void*)pData;
  pJob->jobType = _atCJT_Send;
  return QueueJob(pJob, handle);
}

ctNetwork::JobStatus ctNetwork::Disconnect(const ctConnectionHandle &handle)
{
  ctConditionalScopeLock lock(m_connectionLock, m_pJobThread != nullptr);
  Connection **ppCon = m_connections.TryGet(handle);
  if (!ppCon)
    return JobStatus(nullptr, -1);

  // Create disconnect data
  _atDisconnectData *pData = ctNew(_atDisconnectData);

  // Create Send Job
  ConnectionJob *pJob = ctNew(ConnectionJob);
  pJob->pConnection = *ppCon;
  pJob->pJobData = (void*)pData;
  pJob->jobType = _atCJT_Disconnect;

  // Remove from connection list
  m_connections.Remove(handle);
  
  return QueueJob(pJob, handle);
}

void ctNetwork::CloseAllConnections()
{
  for (ctConnectionHandle &handle : Connections())
    Disconnect(handle);
}

bool ctNetwork::IsConnected(const ctConnectionHandle &handle)
{
  ctConditionalScopeLock lock(m_connectionLock, m_pJobThread != nullptr);
  Connection **ppCon = m_connections.TryGet(handle);
  return ppCon ? (*ppCon)->socket.IsValid() : false;
}

bool ctNetwork::CanRecieve(const ctConnectionHandle &handle, int64_t *pNumBytes)
{
  ctConditionalScopeLock lock(m_connectionLock, m_pJobThread != nullptr);
  Connection **ppCon = m_connections.TryGet(handle);
  return ppCon ? (*ppCon)->socket.CanRead(pNumBytes) : false;
}

ctVector<ctConnectionHandle> ctNetwork::Connections()
{
  ctConditionalScopeLock lock(m_connectionLock, m_pJobThread != nullptr);
  return m_connections.GetKeys();
}

ctVector<uint8_t> ctNetwork::GetRecieve(const ctConnectionHandle &handle)
{
  ctConditionalScopeLock lock(m_connectionLock, m_pJobThread != nullptr);
  Connection **ppCon = m_connections.TryGet(handle);
  return ppCon ? std::move((*ppCon)->recv) : ctVector<uint8_t>();
}

void ctNetwork::ProcessJobs()
{
  while (m_running)
  {
    if (m_jobQueue.size() == 0)
    {
      ctSleep(10);
      continue;
    }

    m_jobLock.lock();
    JobStatus stat = m_jobQueue.front();
    DoJob(stat.m_pJob);
    m_jobQueue.erase(0);
    m_jobLock.unlock();
  }
}

void ctNetwork::DoJob(ConnectionJob *pJob)
{
  pJob->failed = false;
  switch (pJob->jobType)
  {
  case _atCJT_Host: pJob->failed = !_DoHostJob(pJob->pConnection, (_atHostData*)pJob->pJobData); break;
  case _atCJT_Connect: pJob->failed = !_DoConnectJob(pJob->pConnection, (_atConnectData*)pJob->pJobData); break;
  case _atCJT_Recieve: _DoRecieveJob(pJob->pConnection, (_atReceiveData*)pJob->pJobData); break;
  case _atCJT_Send: pJob->failed = !_DoSendJob(pJob->pConnection, (_atSendData*)pJob->pJobData); break;
  case _atCJT_Disconnect: _DoDisconnectJob(pJob->pConnection, (_atDisconnectData*)pJob->pJobData); break;
  }
  pJob->done = true;
}

ctNetwork::JobStatus ctNetwork::QueueJob(ConnectionJob *pJob, const ctConnectionHandle &handle)
{
  if (m_pJobThread)
  { // If there is a worker thread then queue the job
    ctScopeLock lock(m_jobLock);
    m_jobQueue.push_back(JobStatus(pJob, handle));
    return m_jobQueue.back();
  }

  // If there is no worker thread, do the job now and return the JobStatus
  JobStatus localStat = m_jobQueue.back();
  DoJob(localStat.m_pJob);
  return localStat;
}

void ctNetwork::DeleteJob(ConnectionJob *pJob)
{
  switch (pJob->jobType)
  {
  case _atCJT_Host: ctDelete((_atHostData*)pJob->pJobData); break;
  case _atCJT_Connect: ctDelete((_atConnectData*)pJob->pJobData); break;
  case _atCJT_Recieve: ctDelete((_atReceiveData*)pJob->pJobData); break;
  case _atCJT_Send: ctDelete((_atSendData*)pJob->pJobData); break;
  case _atCJT_Disconnect: ctDelete((_atDisconnectData*)pJob->pJobData); break;
  }
  pJob->pJobData = nullptr;
}

int64_t ctNetwork::GetNextHandle()
{
  static int64_t nextHandle = 0;
  return nextHandle++;
}

bool _DoHostJob(ctNetwork::Connection *pConnection, _atHostData *pData)
{
  pConnection->socket = ctSocket::Host(ctString(pData->port));
  return pConnection->socket.IsValid();
}

bool _DoConnectJob(ctNetwork::Connection *pConnection, _atConnectData *pData)
{
  pConnection->socket = ctSocket::Connect(pData->url, ctString(pData->port));
  return pConnection->socket.IsValid();
}

bool _DoSendJob(ctNetwork::Connection *pConnection, _atSendData *pData)
{
  int64_t sent = pConnection->socket.Write(pData->buffer.data(), pData->buffer.size());
  return pData->buffer.size() == sent;
}

void _DoRecieveJob(ctNetwork::Connection *pConnection, _atReceiveData *pData)
{
  ctVector<uint8_t> &recv = pConnection->recv;
  int64_t bytesRead = 0;
  uint8_t buffer[recvBlockSize] = { 0 };
  while ((bytesRead = pConnection->socket.Read(buffer, recvBlockSize)) > 0)
  {
    int64_t offset = recv.size();
    recv.resize(recv.size() + bytesRead);
    memcpy(recv.data() + offset, buffer, bytesRead);
  }
}

void _DoDisconnectJob(ctNetwork::Connection *pConnection, _atDisconnectData *pData)
{
  pConnection->socket = ctSocket();
  pConnection->recv.clear();
  ctDelete(pConnection);
}

ctNetwork::JobStatus::JobStatus(ConnectionJob *pJob, const ctConnectionHandle &handle)
  : m_pJob(pJob)
  , m_handle(handle)
{ m_pJob->refs++; }

ctNetwork::JobStatus::JobStatus(const JobStatus &copy) : JobStatus(copy.m_pJob, copy.m_handle) {}

ctNetwork::JobStatus::JobStatus(JobStatus &&move)
{
  m_pJob = move.m_pJob;
  m_handle = move.m_handle;
  move.m_handle = -1;
  move.m_pJob = 0;
}

ctNetwork::JobStatus::~JobStatus() { Clear(); }

bool ctNetwork::JobStatus::Done() const { return m_pJob && m_pJob->done; }
bool ctNetwork::JobStatus::Failed() const { return m_pJob && m_pJob->failed; }
ctConnectionHandle ctNetwork::JobStatus::Handle() const { return m_handle; }

const ctNetwork::JobStatus& ctNetwork::JobStatus::operator=(const JobStatus &copy)
{
  Clear();
  m_pJob = copy.m_pJob;
  m_handle = copy.m_handle;
  m_pJob->refs++;
  return *this;
}

const ctNetwork::JobStatus& ctNetwork::JobStatus::operator=(JobStatus &&move)
{
  Clear();
  m_pJob = move.m_pJob;
  m_handle = move.m_handle;
  move.m_pJob = nullptr;
  move.m_handle = -1;
  return *this;
}

void ctNetwork::JobStatus::Clear()
{
  if (m_pJob && --m_pJob->refs == 0)
    DeleteJob(m_pJob);
  m_pJob = nullptr;
  m_handle = -1;
}
