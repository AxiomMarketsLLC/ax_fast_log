#ifdef __CONSOLETRANS__
#define __CONSOLETRANS__
#include "TransportInterface.hpp"

class ConsoleTransport: public TransportInterface{
  std::ofstream outStream;
  public:
    ConsoleTransport(const std:: ofstream&);
    int write(const std:: string&);
    ~ConsoleTransport();


};
#endif
