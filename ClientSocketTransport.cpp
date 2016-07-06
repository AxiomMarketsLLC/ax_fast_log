#include "ClientSocketTransport.hpp"
#include "debug.h"
#define NOBLOCK_SOCK (true)

ClientSocketTransport::ClientSocketTransport(const int port,const std::string& address):cli(){
  if(!cli.conn(address,port,!NOBLOCK_SOCK)){
    throw std::runtime_error("LOGGER: Error while connecting to server.");
  }
}


int ClientSocketTransport::write(const std:: string& msg, LogEnums::Severity sev){
  if (!cli.send_data(msg)){
    DBG("LOGGER: Error while sending data.");
    return WRITE_FAILURE;
  }
  return WRITE_SUCCESS;
}
