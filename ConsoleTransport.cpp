#include "ConsoleTransport.hpp"

ConsoleTransport::ConsoleTransport():outStream(std::cout){
}

int ConsoleTransport::write(const std::string& msg, LogEnums::Severity sev){

  switch(sev) {
    case LogEnums::DEBG:
    case LogEnums::INFO:
      outStream.rdbuf(std::cout.rdbuf());
      break;
    case LogEnums::WARN:
    case LogEnums::ERRO:
      outStream.rdbuf(std::cerr.rdbuf());
  }
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
