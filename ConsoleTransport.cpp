#include "ConsoleTransport.hpp"
#include <iostream>

ConsoleTranport::ConsoleTransport(){}

int ConsoleTransport::write(const std::string& msg){
  if (cout.is_open())
  {
    cout<<msg<<std::endl;
  }
  else{
     throw(std::runtime_error("LOGGER: Output stream is not open"));
     return -1;
  }
  return 0;
}

ConsoleTransport::~ConsoleTransport(){}
