#include "FileTransport.hpp"
#include <fstream>
#include <iostream>

FileTransport::FileTransport(const std::string& path) : outStream(new std::ofstream){
  outStream->open(path.c_str(),std::ios_base::out|std::ios_base::trunc);
  if(!outStream->is_open()){
    throw(std::runtime_error("LOGGER: Output stream could not be opened"));
  }
  outStream->exceptions ( std::ofstream::failbit | std::ofstream::badbit );
}

int FileTransport::write(const std::string& msg, LogEnums::Severity sev){
  if (outStream->is_open())
  {
    try {
      *outStream << msg<<std::endl;
    } catch (std::ofstream::failure e) {
      DBG("LOGGER: Output stream in error state");
    }
  } else {
     throw(std::runtime_error("LOGGER: Output stream is not open"));
     return WRITE_FAILURE;
  }
  return WRITE_SUCCESS;
}

void FileTransport::closePath(){

  if( outStream->is_open() )
   {
       outStream->close();
   }
}

FileTransport::~FileTransport(){
  
  this->closePath();
}
