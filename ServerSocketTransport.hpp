#ifndef __SERVSOCKETTRANS__
#define __SERVSOCKETTRANS__
#include "TransportInterface.hpp"
#include "debug.h"
#include <stdio.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/fcntl.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <iostream>
#include <boost/thread.hpp>
#include <boost/chrono.hpp>

#define SERVER_WAIT_TIMEOUT_MS (4000)
#define SERVER_BACKLOG_SZ (2)
#define INVALID_FD (-1)


class ServerSocketTransport: public TransportInterface{
  int listenSocket, clientSocket;
  std::unique_ptr<boost::thread> serveThread;
public:
  ServerSocketTransport(const int port);
  ~ServerSocketTransport(void);
  bool clientConnected();
  bool startListen(const int port);
  void waitForConnection();
  int write(const std:: string&, LogEnums::Severity = LogEnums::INFO);
  void closeSocket();
};
#endif
