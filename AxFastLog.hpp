#ifndef __AXFASTLOG__
#define __AXFASTLOG__
#include "SafeQueue.hpp"
#include "TransportInterface.hpp"
#include "FileTransport.hpp"
#include <boost/thread.hpp>
#include <time.h>
#include <memory>

namespace LogEnums {
  enum Severity{INFO, DEBG, WARN, ERRO};
  enum TransportType{FILE,SCKT,CNSL};
}
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
  //void setTransportType(TransportType);
  //TransportType getTransportType();
  void log(const std::string&, LogEnums::Severity);

  //~AxFastLog();

  private:
  SafeQueue<std::pair<std::string,LogEnums::Severity>> safeQ;
  std::unique_ptr<TransportInterface> transport;
  void post();
  std::unique_ptr<boost::thread> postThread;

 
};

#endif
