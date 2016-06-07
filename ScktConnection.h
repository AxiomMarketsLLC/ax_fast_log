#pragma once

#include "stdafx.h"

#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netdb.h>

class ScktConnection
{
private:
    int listenSocket, clientSocket;
     
public:
	ScktConnection(void);
	~ScktConnection(void);

	bool ClientConnected();
	bool Listen(int port);
	bool WaitForConnection();
    void SendData(std::string data);
};

