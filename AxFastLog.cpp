#include <stdio.h>
#include "AxFastLog.hpp"

AxFastLog::AxFaceLog(TransportType t, const std::string& address): safeQ() {
	 switch(t) {
		 case FileTransport:
		 transport = new FileTransport(address);
		 break;
		 case SocketTransport:
		 break;
		 case ConsoleTransport:
		 break;
		 }

		 postThread(boost::thread(&(this->post), this));
}
/*
AxFastLog::setTransportType(TransportType t){
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
AxFastLog::log(std::string msg, Severity sev){
	pair<std::string,Severity> msgPair = std::make_pair(msg,sev);
	safeQ.enqueue(msgPair);
}

AxFastLog::post(){
	while(true){
		while(!safeQ.empty()){
			pair<std::string,Severity> sendPair = safeQ.dequeue();
			transport->write(sendPair.first);
		}
		std::cout << "LOGGER: Queue is empty" << << endl;
	}
}
