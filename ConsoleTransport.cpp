#include "ConsoleTransport.hpp"

ConsoleTransport::ConsoleTransport(std::ostream& stream):outStream(stream){
}

int ConsoleTransport::write(const std::string& msg){
  try
  {
    outStream<<msg<<std::endl;
  }catch(...)
  {
     throw(std::runtime_error("LOGGER: Could not write to console stream!"));
     return -1;
  }
  return 0;
}

ConsoleTransport::~ConsoleTransport(){}
