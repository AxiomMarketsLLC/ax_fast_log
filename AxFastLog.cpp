#include <iostream>
#include "AxFastLog.hpp"

AxFastLog::AxFastLog(LogEnums::TransportType t, const std::string& address): safeQ() {
	 switch(t) {
		 case LogEnums::FILE:
		 transport = std::unique_ptr<FileTransport>(new FileTransport(address));
		 break;
		 case LogEnums::SCKT:
		 break;
		 case LogEnums::CNSL:
		 break;
		 }

	postThread = std::unique_ptr<boost::thread>(new boost::thread(&AxFastLog::post, this));
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
void AxFastLog::log(const std::string& msg, LogEnums::Severity sev){
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