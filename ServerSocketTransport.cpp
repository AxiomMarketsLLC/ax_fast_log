#include "ServerSocketTransport.hpp"
#include <unistd.h>
#include <stdexcept>

ServerSocketTransport::ServerSocketTransport(const int port)
{
  if (port < 1025 || port > 65535){
      throw std::invalid_argument("LOGGER: Incorrect argument port: must enter an integer between 1025-65535");
  }
  listenSocket = INVALID_FD;
  clientSocket = INVALID_FD;
  if(!this->startListen(port)) {
    throw std::runtime_error("LOGGER: Couldn't start listen on port");
  }
  serveThread = std::unique_ptr<boost::thread>(new boost::thread(&ServerSocketTransport::waitForConnection, this));

}

ServerSocketTransport::~ServerSocketTransport(void)
{
  this->closeSocket();

}

bool ServerSocketTransport::clientConnected()
{
  return clientSocket != INVALID_FD;
}

bool ServerSocketTransport::startListen(const int port)
{
  struct sockaddr_in serv_addr;

  //create listen socket if it is not already created
  if(listenSocket == INVALID_FD)
  {
    //Create socket
    listenSocket = socket(AF_INET, SOCK_STREAM, 0);

    if(listenSocket == INVALID_FD)
    {
      return false;
    }
  }

  //clear address structure (copies zeros into serv_addr)
  bzero((char *) &serv_addr, sizeof(serv_addr));

  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons( port );

    if (bind(listenSocket, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) <0)
    {
      return false;
    }

    listen(listenSocket, SERVER_BACKLOG_SZ);

    DBG("Listening ...");
    return true;
}

void ServerSocketTransport::waitForConnection()
{
  struct sockaddr_in cli_addr;
  socklen_t clilen;
  clilen=sizeof(cli_addr);
  while (true) {
  	boost::this_thread::interruption_point();
  	clientSocket = accept4(listenSocket, (struct sockaddr *) &cli_addr, &clilen, SOCK_NONBLOCK);
  	if (clientSocket != INVALID_FD ) {
  		break;
  	}
  }
}

int ServerSocketTransport::write(const std:: string& msg, LogEnums::Severity sev){

 if (!serveThread->try_join_for(boost::chrono::milliseconds(SERVER_WAIT_TIMEOUT_MS))) {
	DBG("Connection timeout");
	return WRITE_FAILURE;
	}
  //Send some data
  if(send(clientSocket, msg.c_str(), strlen(msg.c_str()) , 0) < 0)
  {
    //Send failed : connection assumed to be lost
		close(clientSocket);
		clientSocket = INVALID_FD;
		DBG("Error while sending data.");
		return WRITE_FAILURE;
  }
  DBG("Data sent.");

  return WRITE_SUCCESS;

}

void ServerSocketTransport::closeSocket(){
  serveThread->interrupt();
  if (clientSocket != INVALID_FD)
  {
    close(clientSocket);
  }

  if(listenSocket != INVALID_FD)
  {
    close(listenSocket);
  }
}
