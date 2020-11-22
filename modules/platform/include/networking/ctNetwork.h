#ifndef atNetwork_h__
#define atNetwork_h__

#include "ctSocket.h"
#include "ctHashMap.h"
#include "ctThreading.h"

typedef int64_t ctConnectionHandle;

struct ConnectionJob;

class ctNetwork
{
public:
  struct Connection
  {
    ctSocket socket;
    ctVector<uint8_t> recv;
  };

  class JobStatus
  {
    friend ctNetwork;

  public:
    JobStatus(ConnectionJob* pJob, const ctConnectionHandle &handle);
    JobStatus(const JobStatus &copy);
    JobStatus(JobStatus &&move);
    ~JobStatus();

    bool Done() const;
    bool Failed() const;
    ctConnectionHandle Handle() const;

    const JobStatus& operator=(const JobStatus &copy);
    const JobStatus& operator=(JobStatus &&copy);

  protected:
    void Clear();

    ctConnectionHandle m_handle;
    ConnectionJob* m_pJob;
  };

  ctNetwork(const bool asyncJobs = true);
  ~ctNetwork();

  //******************
  // Asynchronous Jobs
  //******************

  // Start a send Job. Use the JobStatus object to query the host job status
  JobStatus Host(const uint16_t &port);

  // Start a send Job. Use the JobStatus object to query the connect job status
  JobStatus Connect(const ctString &url, const uint16_t &port);
  
  // Start a send Job. Use the JobStatus object to query the send job status
  JobStatus Send(const ctConnectionHandle &handle, const ctVector<uint8_t> &data);
  
  // Start a receive Job. Use the JobStatus object to query the receive job status
  // Bytes received from a send job can be retrieved via GetRecieved()
  JobStatus Receive(const ctConnectionHandle &handle);

  // Start a disconnect Job. Use the JobStatus object to query the disconnect job status
  JobStatus Disconnect(const ctConnectionHandle &handle);

  //******************
  // Synchronous Jobs
  //******************

  // Close all active connections. Invalidates all open connection handles.
  void CloseAllConnections();

  //******************
  // Connection Status
  //******************

  // Check if a atConnectionHandle is actively connected
  bool IsConnected(const ctConnectionHandle &handle);

  // Check if a atConnectionHandle a call to Receive() will successfully retrieve some data.
  bool CanRecieve(const ctConnectionHandle &handle, int64_t *pNumBytes = nullptr);

  // Returns a list of all active connection handles.
  ctVector<ctConnectionHandle> Connections();

  //******************
  // Connection Data
  //******************

  // Return all bytes that have been received from a connection.
  // Clears the receive buffer.
  ctVector<uint8_t> GetRecieve(const ctConnectionHandle &handle);

protected:
  void ProcessJobs();
  JobStatus QueueJob(ConnectionJob *pJob, const ctConnectionHandle &handle);

  static void DoJob(ConnectionJob *pJob);
  static void DeleteJob(ConnectionJob *pJob);

  static int64_t GetNextHandle();

  ctHashMap<ctConnectionHandle, Connection*> m_connections;
  
  bool m_running;
  std::thread *m_pJobThread;
  std::mutex m_connectionLock;
  std::mutex m_jobLock;
  ctVector<JobStatus> m_jobQueue;
};

#endif // atNetwork_h__
