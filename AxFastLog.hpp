#ifndef __AXFASTLOG__
#define __AXFASTLOG__



#include "ProducerConsumerQueue.hpp"
#include "TransportInterface.hpp"
#include "FileTransport.hpp"
#include "ConsoleTransport.hpp"
#include "SocketTransport.hpp"

#include <boost/thread.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>


#include "ClientSocketTransport.hpp"
#include <time.h>
#include <memory>

/*
*
*	OPTIONS
*
*
*
*/


#define TIMEOUT_US 200
#define DEFAULT_QUEUE_SZ 200

class AxFastLog {

  public:
  AxFastLog(LogEnums::TransportType , const std::string&);
  AxFastLog(LogEnums::TransportType , const int);
  AxFastLog(LogEnums::TransportType , const int, const std::string&);
  AxFastLog(LogEnums::TransportType);
  void log(const std::string&, LogEnums::Severity);
  ~AxFastLog();

  private:
  //using folly queue instead
  //SafeQueue<std::pair<std::string,LogEnums::Severity>> q;

  folly::ProducerConsumerQueue<std::pair<std::string,LogEnums::Severity>> safeQueue;
  void post();
  std::unique_ptr<TransportInterface> transport;
  std::unique_ptr<boost::thread> postThread;

};

#endif
