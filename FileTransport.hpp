#ifndef __FILETRANS__
#define __FILETRANS__
#include "TransportInterface.hpp"


class FileTransport:public TransportInterface{
  std::unique_ptr<std::ofstream> outStream;

  public:
  FileTransport(const std::string&);
  int write(const std::string&);
  ~FileTransport();

};
#endif