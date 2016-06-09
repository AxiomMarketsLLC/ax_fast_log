#ifndef __AXFASTLOG__
#define __AXFASTLOG__
#include "SafeQueue.hpp"
#include "Transport.hpp"
#include "FileTransport.hpp"
#include <boost/thread.hpp>
#include <time.h>

class AxFastLog {

  enum Severity{INFO, DEBG, WARN, ERRO};
  enum TransportType{SCKT,CNSL};

  struct message{
    time_t time;
    Severity sev;
    std::string body;
  };


  private:
  SafeQueue<std::pair<std::string,Severity>> safeQ;
  std::unique_ptr<TransportInterface> transport;
  void post();
  boost::thread postThread;

  public:
  AxFastLog(TransportType, const std::string&);
  //void setTransportType(TransportType);
  //TransportType getTransportType();
  void log(std::string, Severity);

  ~AxFastLog()

};

#endif
