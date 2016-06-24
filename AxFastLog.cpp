#include <iostream>
#include <unistd.h>
#include "AxFastLog.hpp"

AxFastLog::AxFastLog(LogEnums::TransportType t, const std::string& address): safeQ(DEFAULT_QUEUE_SZ) {
	if (t != LogEnums::FILE) {
		throw std::runtime_error("Illegal arguments to AxFastLog(File) constructor");
 	}
        transport = std::unique_ptr<FileTransport>(new FileTransport(address));
	postThread = std::unique_ptr<boost::thread>(new boost::thread(&AxFastLog::post, this));
}

AxFastLog::AxFastLog(LogEnums::TransportType t, const int port): safeQ(DEFAULT_QUEUE_SZ) {
	if (t != LogEnums::SCKT) {
		throw std::runtime_error("Illegal arguments to AxFastLog(Socket) constructor");
 	}
  	transport = std::unique_ptr<SocketTransport>(new SocketTransport(port)); 
	postThread = std::unique_ptr<boost::thread>(new boost::thread(&AxFastLog::post, this));
}

AxFastLog::AxFastLog(LogEnums::TransportType t): safeQ(DEFAULT_QUEUE_SZ) {
	if (t != LogEnums::CNSL) {
		throw std::runtime_error("Illegal arguments to AxFastLog(Console) constructor");
 	}
  	transport = std::unique_ptr<ConsoleTransport>(new ConsoleTransport()); 
	postThread = std::unique_ptr<boost::thread>(new boost::thread(&AxFastLog::post, this));

}

AxFastLog::~AxFastLog(){
 postThread->interrupt();
 postThread->timed_join(boost::posix_time::microseconds(TIMEOUT_US));
};


void AxFastLog::log(const std::string& msg, LogEnums::Severity sev) {
	//safeQ.enqueue(std::make_pair(msg, sev));
	  safeQ.write(std::make_pair(msg,sev));
}

void AxFastLog::post(){
		try {
			while(true){
				boost::this_thread::interruption_point();
				std::pair<std::string,LogEnums::Severity> sendPair;
				if(safeQ.read(sendPair)) {
				//std::pair<std::string,LogEnums::Severity> sendPair = safeQ.dequeue();
				transport->write(sendPair.first,sendPair.second);
				} 
			}			
		}
		catch (boost::thread_interrupted&) {	}
}
