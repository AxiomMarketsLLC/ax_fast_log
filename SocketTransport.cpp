#include "SocketTransport.hpp"
#include <unistd.h>
#include <stdexcept>

SocketTransport::SocketTransport(const int port)
{
  if (port < 1025 || port > 65535){
      throw std::invalid_argument("Incorrect argument port: must enter an integer between 1025-65535");
  }
  listenSocket = -1;
  clientSocket= -1;
  if(!this->startListen(port)) {
    throw std::runtime_error("LOGGER: Couldn't start listen on port");
  }
  serveThread = std::unique_ptr<boost::thread>(new boost::thread(&SocketTransport::waitForConnection, this));
  
}

SocketTransport::~SocketTransport(void)
{
  if (clientSocket != -1)
  {
    close(clientSocket);
  }

  if(listenSocket != -1)
  {
    close(listenSocket);
  }
  
}

bool SocketTransport::clientConnected()
{
  return clientSocket > -1;
}

bool SocketTransport::startListen(const int port)
{
  struct sockaddr_in serv_addr;

  //create listen socket if it is not already created
  if(listenSocket == -1)
  {
    //Create socket
    listenSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(listenSocket == -1)
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
    //Here, we set the maximum size for the backlog queue to 5
    listen(listenSocket, 5);

    std::cout << "LOGGER: Listening ..." << std::endl;

    return true;
}

void SocketTransport::waitForConnection()
{
  struct sockaddr_in cli_addr;
  socklen_t clilen;
  clilen=sizeof(cli_addr);
  clientSocket = accept(listenSocket, (struct sockaddr *) &cli_addr, &clilen);

  if (clientSocket < 0)
  {
   std::cout << "LOGGER: Error during client connection." << std::endl;

  } else {
   std::cout << "LOGGER: Client is connected." << std::endl;
  }
}
int SocketTransport::write(const std:: string& msg, LogEnums::Severity sev){

 if (!serveThread->timed_join(boost::posix_time::milliseconds(8000))) {
	std::cout << "LOGGER: Connection timeout" << std::endl;
	}
  //Send some data
  if(send(clientSocket, msg.c_str(), strlen(msg.c_str()) , 0) < 0)
  {
    // Send failed : connection assumed to be lost
		close(clientSocket);
		clientSocket = -1;
		std::cout << "LOGGER: Error while sending data." << std::endl;
		return -1;
  }
  std::cout << "LOGGER: Data sent." << std::endl;

  return 0;

}

