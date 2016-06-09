#ifndef __TRANSPORT__
#define __TRANSPORT__
#include <string>

class TransportInterface {

 public:
 virtual void write(const std::string&);
};

#endif
