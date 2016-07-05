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
  this->closeSocket();
  
}

bool SocketTransport::clientConnected()
{
  return clientSocket != -1;
}

bool SocketTransport::startListen(const int port)
{
  struct sockaddr_in serv_addr;

  //create listen socket if it is not already created
  if(listenSocket == -1)
  {
    //Create socket
  // listenSocket = socket(AF_INET, SOCK_STREAM, 0);
   listenSocket = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
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

    DBG("Listening ...");

    return true;
}

void SocketTransport::waitForConnection()
{
  struct sockaddr_in cli_addr;
  socklen_t clilen;
  clilen=sizeof(cli_addr);
  try { 
  while(true) {
   boost::this_thread::interruption_point();
   clientSocket = accept(listenSocket, (struct sockaddr *) &cli_addr, &clilen);
  }
  } catch(boost::thread_interrupted const&){return;}
  if (clientSocket < 0)
  {
   DBG("Error during client connection.");

  } else {
   DBG("Client is connected.");
  }
}
int SocketTransport::write(const std:: string& msg, LogEnums::Severity sev){

 if (!serveThread->timed_join(boost::posix_time::milliseconds(SERVER_WAIT_TIMEOUT_MS))) {
	DBG("Connection timeout");
	return -1;
	}
  //Send some data
  if(send(clientSocket, msg.c_str(), strlen(msg.c_str()) , 0) < 0)
  {
    //Send failed : connection assumed to be lost
		close(clientSocket);
		clientSocket = -1;
		DBG("Error while sending data.");
		return -1;
  }
  DBG("Data sent.");

  return 0;

}

void SocketTransport::closeSocket(){
  serveThread->interrupt();
  if (clientSocket != -1)
  {
    close(clientSocket);
  }

  if(listenSocket != -1)
  {
    close(listenSocket);
  }
}

