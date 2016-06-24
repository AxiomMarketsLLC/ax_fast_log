#include "ConsoleTransport.hpp"

ConsoleTransport::ConsoleTransport():outStream(std::cout){
}

int ConsoleTransport::write(const std::string& msg, LogEnums::Severity sev){

  try
  {
    switch(sev) {
    	case LogEnums::DEBG:
    	case LogEnums::INFO:
      		std::cout << msg << std::endl;
      		break;
   	case LogEnums::WARN:
    	case LogEnums::ERRO:
            	std::cerr <<  msg << std::endl;
		break;
  }
    
    
  }catch(...)
  {
     DBG("LOGGER: Could not write to console stream!");
     return -1;
  }
  return 0;
}
ConsoleTransport::~ConsoleTransport(){}
