#include <iostream>
#include "AxFastLog.hpp"

AxFastLog::AxFastLog(LogEnums::TransportType t, const std::string& address): safeQ() {
	 switch(t) {
		 case LogEnums::FILE:
		 transport = new FileTransport(address);
		 break;
		 case LogEnums::SCKT:
		 break;
		 case LogEnums::CNSL:
		 break;
		 }

		 //postThread(boost::thread(&(this->post), this));
}
AxFastLog::~AxFastLog() {

	delete transport;
}
/*
AxFastLog::setTransportType(LogEnums::TransportType t){
	if(safeQ.empty()){
		switch(t) {
 		 case FileTransport:
 		 transport = new FileTransport();
 		 break;
 		 case SocketTransport:
 		 break;
 		 case ConsoleTransport:
 		 break;
 		 }
	}
	else{
		post();

		//while loop to empty queue and call setTransportType again
	}
}
*/
void AxFastLog::log(std::string msg, LogEnums::Severity sev){
	std::pair<std::string,LogEnums::Severity> msgPair = std::make_pair(msg,sev);
	safeQ.enqueue(msgPair);
}

void AxFastLog::post(){
	while(true){
		while(!safeQ.empty()){
			std::pair<std::string,LogEnums::Severity> sendPair = safeQ.dequeue();
			transport->write(sendPair.first);
		}
		std::cout << "LOGGER: Queue is empty" << std::endl;
	}
}
