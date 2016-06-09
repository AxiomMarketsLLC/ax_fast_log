#ifndef __TRANSPORT__
#define __TRANSPORT__
#include <string>
#include <memory>

class TransportInterface {

 public:
 virtual int write(const std::string&);
 virtual ~TransportInterface();
};

#endif
