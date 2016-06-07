#include "ScktConnection.h"
#include <unistd.h>

ScktConnection::ScktConnection(void)
{
    listenSocket = -1;
    clientSocket = -1;
 }


ScktConnection::~ScktConnection(void)
{
	if (clientSocket != -1)
	{
		close(clientSocket);
	}

	if (listenSocket != -1)
	{
		close(listenSocket);
	}
}

bool ScktConnection::ClientConnected()
{
	return clientSocket > -1;
}

bool ScktConnection::Listen(int port)
{
	struct sockaddr_in serv_addr;

    //create listen socket if it is not already created
    if(listenSocket == -1)
    {
        //Create socket
        listenSocket = socket(AF_INET , SOCK_STREAM , 0);
        if (listenSocket == -1)
        {
            return false;
        }
    }

	// clear address structure
    bzero((char *) &serv_addr, sizeof(serv_addr));

	serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons( port );

	if (bind(listenSocket, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
	{
		return false;
	}

	// Here, we set the maximum size for the backlog queue to 5.
    listen(listenSocket, 5);

	std::cout << "Listening..." << std::endl;

	return true;
}

bool ScktConnection::WaitForConnection()
{
	struct sockaddr_in cli_addr;
	socklen_t clilen;
	clilen = sizeof(cli_addr);
	clientSocket = accept(listenSocket, (struct sockaddr *) &cli_addr, &clilen);

	if (clientSocket < 0)
	{
		std::cout << "Error during client connection." << std::endl;
		return false;
	}
	std::cout << "Client is connected." << std::endl;
	return true;
}
 
void ScktConnection::SendData(std::string data)
{
    //Send some data
    if( send(clientSocket , data.c_str() , strlen( data.c_str() ) , 0) < 0)
    {
		// Send failed : connection assumed to be lost
		close(clientSocket);
		clientSocket = -1;
		std::cout << "Error while sending data." << std::endl;
    }
	std::cout << "Data sent." << std::endl;
}
