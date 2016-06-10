#ifndef __TRANSPORT__
#define __TRANSPORT__
#include <string>
#include <memory>

class TransportInterface {

 public:
 virtual int write(const std::string&) = 0;
 virtual ~TransportInterface() = 0;
};
inline TransportInterface::~TransportInterface() { }
#endif
