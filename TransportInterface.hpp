#ifndef __TRANSPORT__
#define __TRANSPORT__
#include <string>
#include <memory>


namespace LogEnums {
  enum Severity{INFO, DEBG, WARN, ERRO};
  enum TransportType{FILE,SCKT,CNSL};
}

class TransportInterface {

 public:
 virtual int write(const std::string&, LogEnums::Severity) = 0;
 virtual ~TransportInterface() = 0;
};
inline TransportInterface::~TransportInterface() { }
#endif
