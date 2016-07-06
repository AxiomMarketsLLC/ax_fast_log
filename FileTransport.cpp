#include "FileTransport.hpp"
#include <fstream>
#include <iostream>

FileTransport::FileTransport(const std::string& path) : outStream(new std::ofstream){
  outStream->open(path.c_str(),std::ios_base::out|std::ios_base::trunc);
  if(!outStream->is_open()){
    throw(std::runtime_error("LOGGER: Output stream could not be opened"));
  }
}

int FileTransport::write(const std::string& msg, LogEnums::Severity sev){
  if (outStream->is_open())
  {
    *outStream << msg<<std::endl;
  }
  else{
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
