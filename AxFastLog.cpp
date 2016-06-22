#include <iostream>
#include <unistd.h>
#include "AxFastLog.hpp"

AxFastLog::AxFastLog(LogEnums::TransportType t, const std::string& address, const int timeout_ms):  m_timeout_ms(timeout_ms),safeQ() {
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

AxFastLog::AxFastLog(LogEnums::TransportType t, const int port, const int timeout_ms): m_timeout_ms(timeout_ms), safeQ() {

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
 postThread->timed_join(boost::posix_time::milliseconds(m_timeout_ms));
};


void AxFastLog::log(const std::string& msg, LogEnums::Severity sev) {
	safeQ.enqueue(std::make_pair(msg, sev));
}

void AxFastLog::post(){
		try {
			while(true){
				boost::this_thread::interruption_point();
				std::pair<std::string,LogEnums::Severity> sendPair = safeQ.dequeue(m_timeout_ms);
				transport->write(sendPair.first,sendPair.second);
			}
		} catch (boost::thread_interrupted&) {
			std::cout<<"LOGGER: POST THREAD INTERRUPTED" << std::endl; return;}	
	
}
