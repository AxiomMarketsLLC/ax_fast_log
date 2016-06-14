#ifdef __CONSOLETRANS__
#define __CONSOLETRANS__
#include "TransportInterface.hpp"

class ConsoleTransport: public TransportInterface{
  public:
    ConsoleTransport();
    int write(const std:: string&);
    ~ConsoleTransport();


};
#endif
