#ifndef  __SOCKETTRANS__
#define __SOCKETTRANS__
#include "TransportInterface.hpp"
#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netdb.h>

class SocketTransport: public TransportInterface{
  int listenSocket, clientSocket;

public:
  ScktConnection(void);
  ~ScktConnection(void);

  bool ClientConnected();
  bool Listen(int port);
  bool WaitForConnection();
  int write(const std:: string&);
};
#endif
