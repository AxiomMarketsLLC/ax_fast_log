#pragma once

#include<iostream>    //cout
#include<stdio.h> //printf
#include<string.h>    //strlen
#include<string>  //std::string
#include<sys/socket.h>    //socket
#include<arpa/inet.h> //inet_addr
#include<netdb.h> //hostent
#include <sys/fcntl.h>
#include <unistd.h>
#include "debug.h"

#define BLOCKING_SOCKET (0)
#define NONBLOCK_SOCKET (1)
#define TCP_TIMEOUT_S (1)
#define INVALID_FD (-1)
//simple tcp client class used for testing SocketTransport.  source: http://www.binarytides.com/code-a-simple-socket-client-class-in-c/

class TcpClient
{
private:
    int sock;
    std::string address;
    int port;
    struct sockaddr_in server;
    bool noblock;
    
    int set_non_blocking();
    int set_blocking();
    bool set_timeout_write();

public:
    TcpClient();
    ~TcpClient();
    void close_socket();
    bool conn(std::string, int, bool);
    bool send_data(std::string data);
    std::string receive(int);
};
