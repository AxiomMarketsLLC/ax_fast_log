#ifndef __CONSOLETRANS__
#define __CONSOLETRANS__
#include "TransportInterface.hpp"
#include <iostream>

class ConsoleTransport: public TransportInterface{
  std::ostream& outStream;
  public:
    ConsoleTransport();
    int write(const std:: string&, LogEnums::Severity = LogEnums::INFO);
    ~ConsoleTransport();


};
#endif