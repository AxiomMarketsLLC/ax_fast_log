#ifndef __CLISOCKETTRANS__
#define __CLISOCKETTRANS__
#include "TransportInterface.hpp"
#include "TcpClient.hpp"
#include <string>

class ClientSocketTransport: public TransportInterface{
private:
    TcpClient cli;


public:
    ClientSocketTransport(int,std::string):cli();
    int write(const std:: string&, LogEnums::Severity = LogEnums::INFO);

};
