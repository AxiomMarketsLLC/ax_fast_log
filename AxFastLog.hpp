#ifndef __AXFASTLOG__
#define __AXFASTLOG__



#include "SafeQueue.hpp"
#include "TransportInterface.hpp"
#include "FileTransport.hpp"
#include "ConsoleTransport.hpp"
#include "SocketTransport.hpp"
#include <boost/thread.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <time.h>
#include <memory>


class AxFastLog {

/*
private:

  struct message{
    time_t time;
    Severity sev;
    std::string body;
  };
*/

  public:
  AxFastLog(LogEnums::TransportType t, const std::string&, const int timeout_ms);
  AxFastLog(LogEnums::TransportType t, const int port, const int timeout_ms);
  void log(const std::string&, LogEnums::Severity);

  ~AxFastLog();

  private:
  SafeQueue<std::pair<std::string,LogEnums::Severity>> safeQ;
  void post();
  std::unique_ptr<TransportInterface> transport;
  std::unique_ptr<boost::thread> postThread;
  const int m_timeout_ms;


};

#endif
