#ifndef __SOCKETTRANS__
#define __SOCKETTRANS__
#include "TransportInterface.hpp"
#include<stdio.h>
#include<string.h>
#include <sys/socket.h>
#include <sys/fcntl.h>
#include<arpa/inet.h>
#include<netdb.h>
#include <iostream>
#include <boost/thread.hpp>

class SocketTransport: public TransportInterface{
  int listenSocket, clientSocket;
  std::unique_ptr<boost::thread> serveThread;
  int setNonBlocking(int fd);
public:
  SocketTransport(const int port);
  ~SocketTransport(void);

  bool clientConnected();
  bool startListen(const int port);
  void waitForConnection();
  int write(const std:: string&, LogEnums::Severity = LogEnums::INFO);
};
#endif
