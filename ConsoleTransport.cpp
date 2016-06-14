#include "ConsoleTransport.hpp"
#include <iostream>

ConsoleTranport::ConsoleTransport(std::ofstream& stream): outStream(stream){
  if(!outStream->is_open()){
      throw(std::runtime_error("LOGGER: Output stream could not be opened"));
    }
}

int ConsoleTransport::write(const std::string& msg){
  if (outStream->is_open())
  {
    outStream << msg<<std::endl;
  }
  else{
     throw(std::runtime_error("LOGGER: Output stream is not open"));
     return -1;
  }
  return 0;
}

FileTransport::~FileTransport(){
  if( outStream->is_open() )
   {
       outStream->close();
   }
}
