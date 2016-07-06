#ifndef __TRANSPORT__
#define __TRANSPORT__
#include <string>
#include <memory>
#define WRITE_SUCCESS (0)
#define WRITE_FAILURE (-1)

namespace LogEnums {
  enum Severity{INFO, DEBG, WARN, ERRO};
  enum TransportType{FILE,SCKT,CSKT,CNSL};
}

class TransportInterface {

 public:
 virtual int write(const std::string&, LogEnums::Severity) = 0;
 virtual ~TransportInterface() = 0;
};
inline TransportInterface::~TransportInterface(){};
#endif
