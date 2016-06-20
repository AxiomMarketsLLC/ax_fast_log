#include <stdio.h>
#include <sys/types.h>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include "../AxFastLog.hpp"
#include <future>
#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE AxFastLog test
#include <boost/test/unit_test.hpp>
#include <boost/test/included/unit_test.hpp>
#define CLI_CMD "ncat localhost "
#define PORT 8000

struct axFastLogVars {
FILE* console;
std::string axFilePath;
std::string transFilePath;
std::string consFilePath;
std::string sockFilePath;
std::string testString;
LogEnums::Severity testSev;
std::string calcString;
AxFastLog fileAx;
SafeQueue<std::string> testQueue;
size_t queueSize;
axFastLogVars() : axFilePath("data/axTest.txt"), transFilePath("data/transTest.txt"), consFilePath("data/consTest.txt"), sockFilePath("data/sockTest.txt"), calcString(""), testString("TEST"), fileAx(LogEnums::FILE, axFilePath), consoleAx(LogEnums: CNSL), socketAx(LogEnums: SCKT)
 testSev(LogEnums::INFO), testQueue() {}

};


BOOST_FIXTURE_TEST_SUITE(axFastLogSuite, axFastLogVars);

BOOST_AUTO_TEST_CASE(fileAxFastLogTest){
  calcString.erase();
  fileAx.log(testString, testSev);
  usleep(1000); //wait 1000 microseconds, avoid race condition with AxFastLog::post()
  //read from file and write to calculatedString
  std::ifstream myReadFile;
  myReadFile.open(axFilePath.c_str());
  if(myReadFile.is_open()){
    while (!myReadFile.eof()) {
      myReadFile>>calcString;
    }
  }
 myReadFile.close();
 BOOST_CHECK_MESSAGE(calcString.compare(testString) == 0, "ERROR: Expected string not equal to calculated string" );
}

BOOST_AUTO_TEST_CASE(consoleAxFastLogTest){
  calcString.erase();

  usleep(1000); //wait 1000 microseconds, avoid race condition with AxFastLog::post()
  //read from file and write to calculatedString
  std::streambuf *psbuf, *backup;
  std::ofstream myWriteFile;
  std::ifstream myReadFile;
  myWriteFile.open(consFilePath.c_str());

  backup = std::cout.rdbuf();     // back up cout's streambuf
  psbuf = myWriteFile.rdbuf();        // get file's streambuf
  std::cout.rdbuf(psbuf);         // assign psbuf to cout

  consoleAx.log(testString, testSev);
  std::cout.rdbuf(backup);        // restore cout's original streambuf
  myWriteFile.close();

  myReadFile.open(consFilePath.c_str());
  if(myReadFile.is_open()){
    while (!myReadFile.eof()) {
      myReadFile >> calcString;
    }
  }
  myReadFile.close();


 BOOST_CHECK_MESSAGE(calcString.compare(testString) == 0, "ERROR: Expected string not equal to calculated string" );
}

BOOST_AUTO_TEST_CASE(socketAxFastLogTest){
  calcString.erase();
  usleep(1000); //wait 1000 microseconds, avoid race condition with AxFastLog::post()
  //read from file and write to calculatedString
  std::ifstream myReadFile;


  socketAx.startListen(PORT);
  std::thread server(&SocketTransport::waitForConnection, &socketTransport);
  std::ostringstream cmdStream;
  cmdStream << CLI_CMD << PORT << " > " << sockFilePath << " &";
  system(cmdStream.str().c_str());
  server.join();
  socketAx.log(testString, testSev);

  myReadFile.open(sockFilePath.c_str());
  if(myReadFile.is_open()){
    while(!myReadFile.eof()) {
      myReadfile >> calcString;
    }
  }
  myReadFile.close();
  BOOST_CHECK_MESSAGE(calcString.compare(testString) ==0, "ERROR: Socket string incorrect")
}


BOOST_AUTO_TEST_CASE(fileTransportTester){
  calcString.erase();
  FileTransport fileTrans(transFilePath);
  fileTrans.write(testString);
  usleep(1000);
  std::ifstream myReadFile;
  myReadFile.open(transFilePath.c_str());
  if(myReadFile.is_open()){
    while (!myReadFile.eof()) {
      myReadFile >> calcString;
    }
  }
  myReadFile.close();
  BOOST_CHECK_MESSAGE(calcString.compare(testString)== 0, "ERROR: Expected string not equal to calculated string" );
}

BOOST_AUTO_TEST_CASE(consoleTransportTester){
  calcString.erase();
  ConsoleTransport consoleTrans;
  std::streambuf *psbuf, *backup;
  std::ofstream myWriteFile;
  std::ifstream myReadFile;
  myWriteFile.open(consFilePath.c_str());

  backup = std::cout.rdbuf();     // back up cout's streambuf
  psbuf = myWriteFile.rdbuf();        // get file's streambuf
  std::cout.rdbuf(psbuf);         // assign psbuf to cout

  consoleTrans.write(testString);
  std::cout.rdbuf(backup);        // restore cout's original streambuf
  myWriteFile.close();

  myReadFile.open(consFilePath.c_str());
  if(myReadFile.is_open()){
    while (!myReadFile.eof()) {
      myReadFile >> calcString;
    }
  }
  myReadFile.close();
  BOOST_CHECK_MESSAGE(calcString.compare(testString) == 0, "ERROR: Expected string not equal to calculated string");
}


BOOST_AUTO_TEST_CASE(safeQueueTester){
  calcString.erase();
  testQueue.enqueue(testString);
  queueSize= testQueue.size();
  BOOST_CHECK_MESSAGE(queueSize == 1, "ERROR:Item not enqueued.");
  calcString = testQueue.dequeue();
  BOOST_CHECK_MESSAGE(calcString.compare(testString) == 0,"ERROR: The dequeued string is incorrect");
}

BOOST_AUTO_TEST_CASE(socketTransportTester){
  //set up socketTranport object and sockets
  calcString.erase();
  SocketTransport socketTransport;
  std::ifstream myReadFile;


  socketTransport.startListen(PORT);
  std::thread server(&SocketTransport::waitForConnection, &socketTransport);
  std::ostringstream cmdStream;
  cmdStream << CLI_CMD << PORT << " > " << sockFilePath << " &";
  system(cmdStream.str().c_str());
  server.join();
  socketTransport.write(testString);

  myReadFile.open(sockFilePath.c_str());
  if(myReadFile.is_open()){
    while(!myReadFile.eof()) {
      myReadFile >> calcString;
    }
  }
  myReadFile.close();

  BOOST_CHECK_MESSAGE(calcString.compare(testString) ==0, "ERROR: Socket string is incorrect.");




}

BOOST_AUTO_TEST_SUITE_END()
