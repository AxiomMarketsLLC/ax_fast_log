#include "ConsoleTransport.hpp"

ConsoleTransport::ConsoleTransport(){}

int ConsoleTransport::write(const std::string& msg, LogEnums::Severity sev){
   switch(sev) {
    	case LogEnums::DEBG:
		std::clog << msg << std::endl;
		break;
    	case LogEnums::INFO:
     		std::cout << msg << std::endl;
      		break;
   	case LogEnums::WARN:
    	case LogEnums::ERRO:
        	std::cerr << msg << std::endl;
  }
  return WRITE_SUCCESS;
}
ConsoleTransport::~ConsoleTransport(){}
