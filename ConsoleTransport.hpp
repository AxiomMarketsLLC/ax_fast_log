#ifndef __CONSOLETRANS__
#define __CONSOLETRANS__
#include "TransportInterface.hpp"
#include "debug.h"
#include <iostream>
#include <stdio.h>
class ConsoleTransport: public TransportInterface{
  public:
    ConsoleTransport();
    int write(const std:: string&, LogEnums::Severity = LogEnums::INFO);
    ~ConsoleTransport();

};
#endif
