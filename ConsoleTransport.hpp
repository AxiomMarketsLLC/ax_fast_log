#ifndef __CONSOLETRANS__
#define __CONSOLETRANS__
#include "TransportInterface.hpp"
#include <iostream>
class ConsoleTransport: public TransportInterface{
  std::ostream& outStream;
  public:
    ConsoleTransport(std::ostream&);
    int write(const std:: string&);
    ~ConsoleTransport();


};
#endif
