#include "FileTransport.hpp"
#include <fstream>
#include <iostream>

FileTransport::FileTransport(const std::string& path){
  outStream = new std::ofstream ();
  outStream->open(path.c_str(),std::ios_base::out|std::ios_base::app);
  if(!outStream->is_open()){
    throw(std::runtime_error("LOGGER: Output stream could not be open"));
  }
}

FileTransport::write(const std::string& msg){
  //ofstream myfile (path);
  if (outStream->is_open())
  {
    *outStream << msg<<std::endl;
  }
  else{
     throw(std::runtime_error("LOGGER: Output stream is not open"));
     return -1;
  }
  return 0;
}

FileTransport::~FileTransport(){
  if( out_stream->is_open() )
   {
       out_stream->close();
   }
}
