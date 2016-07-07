#include "TcpClient.hpp"

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
bool TcpClient::conn(std::string address , int port)
{
    //create socket if it is not already created
    if(sock == INVALID_FD)
    {
      sock = socket(AF_INET , SOCK_STREAM , 0);
        if (sock == INVALID_FD)
        {
            DBG("TcpClient: Could not create socket");
            return false;
        }

        DBG("TcpClient: Socket created");
    }
    struct timeval timeout;      
    timeout.tv_sec = TCP_TIMEOUT_S;
    timeout.tv_usec = 0;
    fd_set set;
    FD_ZERO(&set);
    FD_SET(sock, &set);

    fcntl(sock, F_SETFL, O_NONBLOCK);
    
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
    int stat;
    //Connect to remote server
    if ( (stat = connect(sock , (struct sockaddr *)&server , sizeof(server))) < 0)
    {
        if(errno != EINPROGRESS){
          DBG("TcpClient: Connection failure");
          return false;   
        }
        
    }
    stat=select(sock+1, NULL, &set, NULL, &timeout); 
    if(stat==1) {
        socklen_t len = sizeof(stat);
        getsockopt(sock, SOL_SOCKET, SO_ERROR, &stat, &len);
        if (!stat) {
         set_timeout_write();
         fcntl(sock, F_SETFL, fcntl(sock, F_GETFL, 0) & ~O_NONBLOCK);
         DBG("TcpClient: Connected");
         return true; //successfully connected
        }
    } DBG("TcpClient: Connection failed");

    return false;
}

/**
    Send data to the connected host
*/
bool TcpClient::send_data(std::string data)
{

    //Send some data
    if( send(sock , data.c_str() , strlen( data.c_str() ) , 0) < 0)
    {
        DBG("TcpClient: Send failed");
        return false;
    }
	DBG("TcpClient: Data send");

    return true;
}

/**
    Receive data from the connected host
*/
std::string TcpClient::receive(int size=512)
{
    char buffer[size];
    memset(buffer,0,sizeof(buffer));
    std::string reply;

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
/*int TcpClient::set_non_blocking()
{
    int flags;

     If they have O_NONBLOCK, use the Posix way to do it */
#if defined(O_NONBLOCK)
    /* Fixme: O_NONBLOCK is defined but broken on SunOS 4.1.x and AIX 3.2.5. */
   /* if (INVALID_FD == (flags = fcntl(sock, F_GETFL, 0)))
        flags = 0;
    return fcntl(sock, F_SETFL, flags | O_NONBLOCK);*/
#else
    /* Otherwise, use the old way of doing it */
   /* flags = 1;
    return ioctl(sock, FIOBIO, &flags);
#endif
}    */
#endif


bool TcpClient::set_timeout_write() {
    struct timeval timeout;      
    timeout.tv_sec = TCP_TIMEOUT_S;
    timeout.tv_usec = 0;
    return setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout,
                sizeof(timeout)) >= 0;
       

}

