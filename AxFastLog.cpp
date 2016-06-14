#include <iostream>
#include <unistd.h>
#include "AxFastLog.hpp"

AxFastLog::AxFastLog(LogEnums::TransportType t, const std::string& address): safeQ() {
	 switch(t) {
		 case LogEnums::FILE:
		      transport = std::unique_ptr<FileTransport>(new FileTransport(address));
		 break;
		 case LogEnums::SCKT:
       		 throw(std::runtime_error("LOGGER: Socket Transport not yet implemented"));
		 break;
		 case LogEnums::CNSL:
		       transport = std::unique_ptr<ConsoleTransport>(new ConsoleTranport());
		 break;
		 }

	postThread = std::unique_ptr<boost::thread>(new boost::thread(&AxFastLog::post, this));
}

//AxFastLog::~AxFastLog(){};

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

void AxFastLog::log(const std::string& msg, LogEnums::Severity sev) {
	safeQ.enqueue(std::make_pair(msg, sev));
}

void AxFastLog::post(){
	while(true){
<<<<<<< HEAD
			std::pair<std::string,LogEnums::Severity> sendPair = safeQ.dequeue();
			transport->write(sendPair.first);
=======
		std::pair<std::string,LogEnums::Severity> sendPair = safeQ.dequeue();
		transport->write(sendPair.first);
>>>>>>> 2129887f7d6e96dd50d934b4209e9de28861e4a7
	}
}
