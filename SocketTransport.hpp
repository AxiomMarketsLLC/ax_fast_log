#ifndef __SOCKETTRANS__
#define __SOCKETTRANS__
#include "TransportInterface.hpp"
#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netdb.h>
#include <iostream>

class SocketTransport: public TransportInterface{
  int listenSocket, clientSocket;

public:
  SocketTransport(void);
  ~SocketTransport(void);

  bool clientConnected();
  bool startListen(int port);
  bool waitForConnection();
  int write(const std:: string&);
};
#endif
