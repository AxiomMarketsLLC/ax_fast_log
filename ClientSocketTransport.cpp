#include "ClientSocketTransport.hpp"
#include "debug.h"

ClientSocketTransport::ClientSocketTransport(const int port,const std::string& address){
  if(!cli.conn(port,address)){
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
