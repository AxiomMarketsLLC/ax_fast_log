#include <iostream>
#include <unistd.h>
#include "AxFastLog.hpp"

AxFastLog::AxFastLog(LogEnums::TransportType t, const std::string& address): safeQ() {
	 switch(t) {
		 case LogEnums::FILE:
		      transport = std::unique_ptr<FileTransport>(new FileTransport(address));
		 break;
		 case LogEnums::SCKT:
       		      transport = std::unique_ptr<SocketTransport>(new SocketTransport(std::stoi(address))); 
		 break;
		 case LogEnums::CNSL:
		       transport = std::unique_ptr<ConsoleTransport>(new ConsoleTransport());
		 break;
		 }

	postThread = std::unique_ptr<boost::thread>(new boost::thread(&AxFastLog::post, this));
}

AxFastLog::AxFastLog(LogEnums::TransportType t, const int port): safeQ() {

	postThread = std::unique_ptr<boost::thread>(new boost::thread(&AxFastLog::post, this));

	 switch(t) {
		 case LogEnums::FILE:
		      throw new std::runtime_error("Must pass valid file path for FileTransport");
		 break;
		 case LogEnums::SCKT:
       		 transport = std::unique_ptr<SocketTransport>(new SocketTransport(port)); 
                 break;
		 case LogEnums::CNSL:
		       transport = std::unique_ptr<ConsoleTransport>(new ConsoleTransport());
		 break;
		 }

}

AxFastLog::~AxFastLog(){
 postThread->interrupt();
};


void AxFastLog::log(const std::string& msg, LogEnums::Severity sev) {
	safeQ.enqueue(std::make_pair(msg, sev));
	//safeQ.push(std::make_pair(msg,sev));
}

void AxFastLog::post(){
		try {
			while(true){
				std::pair<std::string,LogEnums::Severity> sendPair = safeQ.dequeue();
				//if(!safeQ.empty()) {
				//	std::pair<std::string,LogEnums::Severity> sendPair = safeQ.pop();
					transport->write(sendPair.first,sendPair.second);
				//}
			}
		} catch (boost::thread_interrupted&) {return;}	
	
}
