#include<iostream>    //cout
#include<stdio.h> //printf
#include<string.h>    //strlen
#include<string>  //string
#include<sys/socket.h>    //socket
#include<arpa/inet.h> //inet_addr
#include<netdb.h> //hostent
#include <sys/fcntl.h>
#include "debug.h"

#define BLOCKING_SOCKET (0)
#define NONBLOCK_SOCKET (1)
#define TCP_TIMEOUT_S (1)
#define INVALID_FD (-1)
//simple tcp client class used for testing SocketTransport.  source: http://www.binarytides.com/code-a-simple-socket-client-class-in-c/

using namespace std;
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
    bool conn(string, int, bool);
    bool send_data(string data);
    string receive(int);
};

TcpClient::TcpClient()
{
    sock = INVALID_FD;
    port = 0;
    address = "";
}

TcpClient::~TcpClient(){

   this->close_socket();
}

/**
    Connect to a host on a certain port number
*/
bool TcpClient::conn(string address , int port, bool noblock)
{
    this->noblock = noblock;
    //create socket if it is not already created
    if(sock == INVALID_FD)
    {
        //Create socket
        if(noblock) {
 	  sock = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
 	  } else {
          sock = socket(AF_INET , SOCK_STREAM , 0);
        }
        if (sock == INVALID_FD)
        {
            DBG("TcpClient: Could not create socket");
        }

        DBG("TcpClient: Socket created");
    }


    //setup address structure
    if(inet_addr(address.c_str()) == INADDR_NONE)
    {
        struct hostent *he;
        struct in_addr **addr_list;

        //resolve the hostname, its not an ip address
        if ( (he = gethostbyname( address.c_str() ) ) == NULL)
        {
            //gethostbyname failed
            herror("gethostbyname");
            DBG("TcpClient: Failed to resolve hostname");

            return false;
        }

        //Cast the h_addr_list to in_addr , since h_addr_list also has the ip address in long format only
        addr_list = (struct in_addr **) he->h_addr_list;

        for(int i = 0; addr_list[i] != NULL; i++)
        {
            server.sin_addr = *addr_list[i];
            DBG("TcpClient: %s resolved to %s", address.c_str(), inet_ntoa(*addr_list[i]));
            break;
        }
    }

    //plain ip address
    else
    {
        server.sin_addr.s_addr = inet_addr( address.c_str() );
    }

    server.sin_family = AF_INET;
    server.sin_port = htons( port );

    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        DBG("TcpClient: Connect failed, or nonblocking connect mode");
        return true;
    }

    DBG("TcpClient: Connected");

    if(!set_timeout_write()) {
	DBG("TcpClient: Warning - set timeout failed");
    }

    return true;
}

/**
    Send data to the connected host
*/
bool TcpClient::send_data(string data)
{
    if(noblock) {
   	set_blocking();
        if(!set_timeout_write()) {
          DBG("TcpClient: Warning - set write timeout failed");
        }
	noblock=false;
    }

    //Send some data
    if( send(sock , data.c_str() , strlen( data.c_str() ) , 0) < 0)
    {
        DBG("TcpClient: Send failed");
        return false;
    }
	DBG("TcpClient:Data send");

    return true;
}

/**
    Receive data from the connected host
*/
string TcpClient::receive(int size=512)
{
    char buffer[size];
    memset(buffer,0,sizeof(buffer));
    string reply;
    if(noblock) {
      set_blocking();
      noblock = false;
    }
    //Receive a reply from the server
    if( recv(sock , buffer , sizeof(buffer) , 0) < 0)
    {
        DBG("TcpClient: Receive failed");
    }

    reply = buffer;
    return reply;
}
void TcpClient::close_socket() {
   close(sock);
}


/*----------------------------------------------------------------------
 Portable function to set a socket into nonblocking mode.
 Calling this on a socket causes all future read() and write() calls on
 that socket to do only as much as they can immediately, and return
 without waiting.
 If no data can be read or written, they return -1 and set errno
 to EAGAIN (or EWOULDBLOCK).
 Thanks to Bjorn Reese for this code.
----------------------------------------------------------------------*/
int TcpClient::set_non_blocking()
{
    int flags;

    /* If they have O_NONBLOCK, use the Posix way to do it */
#if defined(O_NONBLOCK)
    /* Fixme: O_NONBLOCK is defined but broken on SunOS 4.1.x and AIX 3.2.5. */
    if (INVALID_FD == (flags = fcntl(sock, F_GETFL, 0)))
        flags = 0;
    return fcntl(sock, F_SETFL, flags | O_NONBLOCK);
#else
    /* Otherwise, use the old way of doing it */
    flags = 1;
    return ioctl(sock, FIOBIO, &flags);
#endif
}    

int TcpClient::set_blocking()
{
return fcntl(sock, F_SETFL, fcntl(sock, F_GETFL, 0) & ~O_NONBLOCK);
}


bool TcpClient::set_timeout_write() {
    struct timeval timeout;      
    timeout.tv_sec = TCP_TIMEOUT_S;
    timeout.tv_usec = 0;
    return setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout,
                sizeof(timeout)) >= 0;
       

}
