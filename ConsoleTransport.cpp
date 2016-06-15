#include "ConsoleTransport.hpp"
#include <iostream>

ConsoleTransport::ConsoleTransport(){}

int ConsoleTransport::write(const std::string& msg){
  try
  {
    std::cout<<msg<<std::endl;
  }catch(...)
  {  
     throw(std::runtime_error("LOGGER: Could not write to console stream!"));
     return -1;
  }
  return 0;
}

ConsoleTransport::~ConsoleTransport(){}
