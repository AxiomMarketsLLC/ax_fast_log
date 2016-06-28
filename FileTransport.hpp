#ifndef __FILETRANS__
#define __FILETRANS__
#include "TransportInterface.hpp"
#include "debug.h"

class FileTransport:public TransportInterface{
  std::unique_ptr<std::ofstream> outStream;

  public:
  FileTransport(const std::string&);
  int write(const std::string&, LogEnums::Severity = LogEnums::INFO);
  void closePath();
  ~FileTransport();

};
#endif
