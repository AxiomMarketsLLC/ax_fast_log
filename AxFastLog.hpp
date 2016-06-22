#ifndef __AXFASTLOG__
#define __AXFASTLOG__



#include "SafeQueue.hpp"
#include "TransportInterface.hpp"
#include "FileTransport.hpp"
#include "ConsoleTransport.hpp"
#include "SocketTransport.hpp"
#include <boost/thread.hpp>
#include <boost/lockfree/queue.hpp>
#include <time.h>
#include <memory>
#define DEFAULT_QUEUE_SZ 8

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
  AxFastLog(LogEnums::TransportType, const std::string&);
  AxFastLog(LogEnums::TransportType t, const int port=8000);
  //void setTransportType(TransportType);
  //TransportType getTransportType();
  void log(const std::string&, LogEnums::Severity);

  ~AxFastLog();

  private:
  SafeQueue<std::pair<std::string,LogEnums::Severity>> safeQ;
  //boost::lockfree::queue<std::pair<std::string,LogEnums::Severity>> safeQ{DEFAULT_QUEUE_SZ};
  std::unique_ptr<TransportInterface> transport;
  void post();
  std::unique_ptr<boost::thread> postThread;


};

#endif
