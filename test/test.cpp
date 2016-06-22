#include <stdio.h>
#include <sys/types.h>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include "../AxFastLog.hpp"
#include "tcp_client.hpp"
#include <boost/test/unit_test.hpp>
#include <boost/test/included/unit_test.hpp>

#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE AxFastLog test

#define HOST "localhost"
#define PORT 63000
#define TIMEOUT_MS 1

struct axFastFileLogVars{
std:: string axFilePath;
AxFastLog fileAx;
std::string testString;
LogEnums::Severity testSev;
std::string calcString;
axFastFileLogVars():axFilePath("data/axTest.txt"),fileAx(LogEnums::FILE, axFilePath, TIMEOUT_MS),testString("TEST"), testSev(LogEnums::INFO),calcString(""){}
};

struct axFastConsLogVars{
std::string consAxFilePath;
AxFastLog consoleAx;
std::string testString;
LogEnums::Severity testSev;
std::string calcString;
axFastConsLogVars():consAxFilePath("data/axTransTest.txt"),consoleAx(LogEnums::CNSL, 0, TIMEOUT_MS), testString("TEST"), testSev(LogEnums::INFO),calcString(""){}
};

struct axFastSockLogVars{
std::string sockAxFilePath;
AxFastLog socketAx;
std::string testString;
LogEnums::Severity testSev;
std::string calcString;
axFastSockLogVars():sockAxFilePath("data/axSockTest.txt"),socketAx(LogEnums::SCKT, PORT, TIMEOUT_MS), testString("TEST"), testSev(LogEnums::INFO),calcString(""){}
};

struct axFastLogVars{
std::string transFilePath;
std::string consFilePath;
std::string sockFilePath;
std::string testString;
LogEnums::Severity testSev;
std::string calcString;
SafeQueue<std::string> testQueue;
size_t queueSize;
axFastLogVars():transFilePath("data/transTest.txt"),consFilePath("data/consTest.txt"),sockFilePath("data/sockTest.txt"),testQueue(), testString("TEST"), testSev(LogEnums::INFO),calcString(""){}
};


BOOST_FIXTURE_TEST_SUITE(fileAxFastLogSuite, axFastFileLogVars);

BOOST_AUTO_TEST_CASE(fileAxFastLogTest){

  calcString.erase();
  fileAx.log(testString,testSev);
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

BOOST_AUTO_TEST_SUITE_END();

BOOST_FIXTURE_TEST_SUITE(consoleAxLogSuite, axFastConsLogVars);

BOOST_AUTO_TEST_CASE(consoleAxFastLogTest){
  calcString.erase();
  usleep(1000); //wait 1000 microseconds, avoid race condition with AxFastLog::post()
  //read from file and write to calculatedString
  std::streambuf *psbuf, *backup;
  std::ofstream myWriteFile;
  std::ifstream myReadFile;
  myWriteFile.open(consAxFilePath.c_str());

  backup = std::cout.rdbuf();     // back up cout's streambuf
  psbuf = myWriteFile.rdbuf();        // get file's streambuf
  std::cout.rdbuf(psbuf);         // assign psbuf to cout

  consoleAx.log(testString, testSev);
  usleep(1000);
  std::cout.rdbuf(backup);        // restore cout's original streambuf
  myWriteFile.close();

  myReadFile.open(consAxFilePath.c_str());
  if(myReadFile.is_open()){
    while (!myReadFile.eof()) {
      myReadFile >> calcString;
    }
  }
  myReadFile.close();


 BOOST_CHECK_MESSAGE(calcString.compare(testString) == 0, "ERROR: Expected string not equal to calculated string" );
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_FIXTURE_TEST_SUITE(socketAxFastLogSuite, axFastSockLogVars);


BOOST_AUTO_TEST_CASE(socketAxFastLogTest){

  std::string testStringCpy = testString;
  tcp_client cli; 
  cli.conn(HOST, (PORT));
  socketAx.log(testString,testSev);
  calcString = cli.receive(1024);
  usleep(TIMEOUT_MS*1000); //wait 1000 microseconds
  BOOST_CHECK_MESSAGE(calcString.compare(testStringCpy)==0, "ERROR: Socket string incorrect");
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_FIXTURE_TEST_SUITE(fileTransportSuite, axFastLogVars);


BOOST_AUTO_TEST_CASE(fileTransportTester){
  calcString.erase();
  FileTransport fileTrans(transFilePath);
  fileTrans.write(testString);
  usleep(TIMEOUT_MS*1000);
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

BOOST_AUTO_TEST_SUITE_END()

BOOST_FIXTURE_TEST_SUITE(consoleTransportSuite, axFastLogVars);

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

BOOST_AUTO_TEST_SUITE_END()

BOOST_FIXTURE_TEST_SUITE(safeQueueSuite, axFastLogVars);


BOOST_AUTO_TEST_CASE(safeQueueTester){
  calcString.erase();
  testQueue.enqueue(testString);
  queueSize= testQueue.size();
  BOOST_CHECK_MESSAGE(queueSize == 1, "ERROR:Item not enqueued.");
  calcString = testQueue.dequeue(TIMEOUT_MS);
  BOOST_CHECK_MESSAGE(calcString.compare(testString) == 0,"ERROR: The dequeued string is incorrect");
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_FIXTURE_TEST_SUITE(socketTransportSuite, axFastLogVars);

BOOST_AUTO_TEST_CASE(socketTransportTester){
  //set up socketTranport object and sockets
  std::string testStringCpy = testString;
  calcString.erase();
  tcp_client cli;
  int err=0;
  SocketTransport socketTransport(PORT+1);
  cli.conn(HOST, (PORT+1));
  
  while(!socketTransport.clientConnected() && err < 3) {
   usleep(TIMEOUT_MS*1000);
   err++;
  }
   
  socketTransport.write(testString);
  calcString = cli.receive(1024);
  BOOST_CHECK_MESSAGE(calcString.compare(testStringCpy) ==0, "ERROR: Socket string is incorrect.");

}

BOOST_AUTO_TEST_SUITE_END()
