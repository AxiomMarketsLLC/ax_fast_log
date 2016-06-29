#include <stdio.h>
#include <sys/types.h>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include "../AxFastLog.hpp"
#include "TcpClient.hpp"
#include "test.hpp"

#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE AxFastLog test

#include <boost/test/unit_test.hpp>
#include <boost/test/included/unit_test.hpp>


#define HOST "localhost"
#define PORT 63000
#define TIMEOUT_MS 1
static const int DEFAULT_QUEUE_TEST_SIZE = 4;



struct axFastFileLogVars{
std:: string axFilePath;
AxFastLog fileAx;
std::string testString;
LogEnums::Severity testSev;
std::string calcString;
axFastFileLogVars():axFilePath("data/axTest.txt"),fileAx(LogEnums::FILE, axFilePath),testString("TEST"), testSev(LogEnums::INFO), calcString(""){}
};

struct axFastConsLogVars{
std::string consAxFilePath;
AxFastLog consoleAx;
std::string testString;
LogEnums::Severity testSev;
std::string calcString;
axFastConsLogVars():consAxFilePath("data/axTransTest.txt"),consoleAx(LogEnums::CNSL), testString("TEST"), testSev(LogEnums::INFO),calcString(""){}
};

struct axFastSockLogVars{
AxFastLog socketAx;
std::string testString;
LogEnums::Severity testSev;
std::string calcString;
axFastSockLogVars():socketAx(LogEnums::SCKT, PORT), testString("TEST"), testSev(LogEnums::INFO), calcString(""){}
};

struct invalidAxFastVars{
AxFastLog* invldFileAx;
AxFastLog* invldConsAx;
AxFastLog* invldSockAx;
std::string calcString;
std::string testFilePath;
invalidAxFastVars():calcString(""), testFilePath("fileTest.txt"){}
};

struct producerConsumerQueueVars{
  NotTrivial nonTriv, nonTriv_1, nonTriv_2;
  folly::ProducerConsumerQueue<NotTrivial> nonTrivQueue;
  producerConsumerQueueVars():nonTriv(), nonTriv_1(), nonTriv_2(), nonTrivQueue(DEFAULT_QUEUE_TEST_SIZE){}
};

struct axFastLogVars{
int writeResult;
std::string transFilePath;
std::string transFileErrorPath;
std::string consFilePath;
std::string consErroFilePath;
std::string consDbugFilePath;
std::string testString;
LogEnums::Severity testSevInfo;
LogEnums::Severity testSevErro;
LogEnums::Severity testSevDbug;
std::string calcString;
SafeQueue<std::string> testQueue;
size_t queueSize;
axFastLogVars():transFilePath("data/transTest.txt"),transFileErrorPath("/test.txt"), consFilePath("data/consTest.txt"), consErroFilePath("data/consErroTest.txt"), consDbugFilePath("data/consDbugTest.txt"), testQueue(), testString("TEST"), testSevInfo(LogEnums::INFO),testSevErro(LogEnums::ERRO), testSevDbug(LogEnums::DEBG),calcString(""){}
};


BOOST_FIXTURE_TEST_SUITE(fileAxFastLogSuite, axFastFileLogVars);

BOOST_AUTO_TEST_CASE(fileAxFastLogTest){

  calcString.erase();
  fileAx.log(testString,testSev);
  usleep(TIMEOUT_MS*1000); //wait 1000 microseconds
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

  //read from file and write to calculatedString
  std::streambuf *psbuf, *backup;
  std::ofstream myWriteFile;
  std::ifstream myReadFile;
  myWriteFile.open(consAxFilePath.c_str());

  backup = std::cout.rdbuf();     // back up cout's streambuf
  psbuf = myWriteFile.rdbuf();        // get file's streambuf
  std::cout.rdbuf(psbuf);         // assign psbuf to cout

  consoleAx.log(testString,testSev);
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
  TcpClient cli;
  cli.conn(HOST, (PORT), BLOCKING_SOCKET);
  socketAx.log(testString,testSev);
  calcString = cli.receive(1024);
  BOOST_CHECK_MESSAGE(calcString.compare(testString)==0, "ERROR: Socket string incorrect");
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_FIXTURE_TEST_SUITE(InvalidAxConstructorsSuite,invalidAxFastVars);

BOOST_AUTO_TEST_CASE(InvalidFileAxTester){
 calcString.erase();
 try{
   invldFileAx = new AxFastLog(LogEnums::SCKT,testFilePath);
 }catch(...){
    calcString = "exception";
  }

  BOOST_CHECK_MESSAGE(calcString.compare("exception")== 0, "ERROR: Expected exception not thrown for writing to a closed stream" );
}

BOOST_AUTO_TEST_CASE(InvalidConsAxTester){
 calcString.erase();
 try{
   invldConsAx = new AxFastLog(LogEnums::FILE);
 }catch(...){
    calcString = "exception";
  }

  BOOST_CHECK_MESSAGE(calcString.compare("exception")== 0, "ERROR: Expected exception not thrown for writing to a closed stream" );
}

BOOST_AUTO_TEST_CASE(InvalidSockAxTester){
 calcString.erase();
 try{
   invldSockAx = new AxFastLog(LogEnums::CNSL,PORT+5);
 }catch(...){
    calcString = "exception";
  }

  BOOST_CHECK_MESSAGE(calcString.compare("exception")== 0, "ERROR: Expected exception not thrown for writing to a closed stream" );
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

BOOST_AUTO_TEST_CASE(fileTransportPathErrorTester){
  calcString.erase();
  try{
    FileTransport fileTrans(transFileErrorPath);
  }
  catch(...){
    calcString = "exception";
  }
  BOOST_CHECK_MESSAGE(calcString.compare("exception")== 0, "ERROR: Expected exception not thrown");
}

BOOST_AUTO_TEST_CASE(fileTransportWriteErrorTester){
  calcString.erase();
  FileTransport fileTrans(transFilePath);
  fileTrans.closePath();
  try{
    fileTrans.write(testString);
  }catch(...){
    calcString = "exception";
  }

  BOOST_CHECK_MESSAGE(calcString.compare("exception")== 0, "ERROR: Expected exception not thrown for writing to a closed stream" );
}



BOOST_AUTO_TEST_SUITE_END()

BOOST_FIXTURE_TEST_SUITE(consoleTransportSuite, axFastLogVars);

BOOST_AUTO_TEST_CASE(consoleTransportTesterDefault){
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
BOOST_AUTO_TEST_CASE(consoleTransportTesterErro){
  calcString.erase();
  ConsoleTransport consoleTrans;
  std::streambuf *psbuf, *backup;
  std::ofstream myWriteFile;
  std::ifstream myReadFile;
  myWriteFile.open(consErroFilePath.c_str());

  backup = std::cerr.rdbuf();     // back up cerr's streambuf
  psbuf = myWriteFile.rdbuf();        // get file's streambuf
  std::cerr.rdbuf(psbuf);         // assign psbuf to cerr

  consoleTrans.write(testString,testSevErro);
  std::cerr.rdbuf(backup);        // restore cerr's original streambuf
  myWriteFile.close();

  myReadFile.open(consErroFilePath.c_str());
  if(myReadFile.is_open()){
    while (!myReadFile.eof()) {
      myReadFile >> calcString;
    }
  }
  myReadFile.close();
  BOOST_CHECK_MESSAGE(calcString.compare(testString) == 0, "ERROR: Expected string not equal to calculated string");
}

BOOST_AUTO_TEST_CASE(consoleTransportTesterDbug){
  calcString.erase();
  ConsoleTransport consoleTrans;
  std::streambuf *psbuf, *backup;
  std::ofstream myWriteFile;
  std::ifstream myReadFile;
  myWriteFile.open(consDbugFilePath.c_str());

  backup = std::clog.rdbuf();     // back up clog's streambuf
  psbuf = myWriteFile.rdbuf();        // get file's streambuf
  std::clog.rdbuf(psbuf);         // assign psbuf to clog

  consoleTrans.write(testString,testSevDbug);
  std::clog.rdbuf(backup);        // restore clog's original streambuf
  myWriteFile.close();

  myReadFile.open(consDbugFilePath.c_str());
  if(myReadFile.is_open()){
    while (!myReadFile.eof()) {
      myReadFile >> calcString;
    }
  }
  myReadFile.close();
  BOOST_CHECK_MESSAGE(calcString.compare(testString) == 0, "ERROR: Expected string not equal to calculated string");
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_FIXTURE_TEST_SUITE(socketTransportSuite, axFastLogVars);

BOOST_AUTO_TEST_CASE(socketTransportTester){
  //set up socketTranport object and sockets

  TcpClient cli;
  SocketTransport socketTransport(PORT+1);
  cli.conn(HOST, (PORT+1), BLOCKING_SOCKET);
  usleep(TIMEOUT_MS*1000);
  BOOST_CHECK_MESSAGE(socketTransport.clientConnected(), "ERROR: Client connection.");
  socketTransport.write(testString);
  calcString = cli.receive(1024);
  BOOST_CHECK_MESSAGE(calcString.compare(testString)==0, "ERROR: Socket string is incorrect.");

}

  BOOST_AUTO_TEST_CASE(socketPortTester){
    calcString.erase();
    try{
      SocketTransport socketTransport(1);
    }
    catch(...){
      calcString = "exception";
    }
    BOOST_CHECK_MESSAGE(calcString.compare("exception") ==0, "ERROR: Exception not thrown as expected");
  }

  BOOST_AUTO_TEST_CASE(socketClientTest){
    calcString.erase();
    SocketTransport socketTransport(PORT+2);
    try{
      SocketTransport socketTransport2(PORT+2);
    }
    catch(...){
      calcString = "exception";
    }
    BOOST_CHECK_MESSAGE(calcString.compare("exception") == 0, "ERROR: Exception not thrown as expected connecting client");
  }

  BOOST_AUTO_TEST_CASE(socketTimeoutTest){
    calcString.erase();
    SocketTransport socketTransport(PORT+3);
    writeResult = socketTransport.write(testString);

    BOOST_CHECK_MESSAGE(writeResult == -1, "ERROR: Return value not as expected");
  }

  BOOST_AUTO_TEST_CASE(socketSendDataTest){
    TcpClient cli;
    SocketTransport socketTransport(PORT+4);
    cli.conn(HOST, (PORT+4), BLOCKING_SOCKET);
    usleep(TIMEOUT_MS*1000);
    BOOST_CHECK_MESSAGE(socketTransport.clientConnected(), "ERROR: Client connection.");
    socketTransport.closeSocket();
    writeResult = socketTransport.write(testString);


    BOOST_CHECK_MESSAGE(writeResult == -1, "ERROR: Return value not as expected");
  }

BOOST_AUTO_TEST_SUITE_END()

BOOST_FIXTURE_TEST_SUITE(safeQueueSuite, axFastLogVars);

BOOST_AUTO_TEST_CASE(safeQueueTester){
  calcString.erase();
  testQueue.enqueue(testString);
  queueSize= testQueue.size();
  BOOST_CHECK_MESSAGE(queueSize == 1, "ERROR:Item not enqueued.");
  calcString = testQueue.dequeue();
  BOOST_CHECK_MESSAGE(calcString.compare(testString) == 0,"ERROR: The dequeued string is incorrect");
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_FIXTURE_TEST_SUITE(producerConsumerQueueSuite,producerConsumerQueueVars);

BOOST_AUTO_TEST_CASE(nonTrivialQueueTest){
  BOOST_CHECK_MESSAGE(nonTrivQueue.isEmpty(), "ERROR: Queue not empty as expected.");
  nonTrivQueue.write(nonTriv);
  nonTrivQueue.write(nonTriv_1);
  BOOST_CHECK_MESSAGE(!nonTrivQueue.isFull(), "ERROR: Queue should not be full.");
  nonTrivQueue.write(nonTriv_2);
  BOOST_CHECK_MESSAGE(nonTrivQueue.isFull(), "ERROR: Queue should be full");
  nonTrivQueue.popFront();
  std::cout<<nonTrivQueue.sizeGuess()<<std::endl;
  BOOST_CHECK_MESSAGE(nonTrivQueue.sizeGuess() == 2, "ERROR: Queue size should be 2.");
}

BOOST_AUTO_TEST_SUITE_END()