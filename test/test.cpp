#include <stdio.h>
#include <sys/types.h>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include "../AxFastLog.hpp"
#include "test.hpp"

#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE AxFastLog test

#include <boost/test/unit_test.hpp>
#include <boost/test/included/unit_test.hpp>


#define HOST "localhost"
#define PORT (62000)
#define TIMEOUT_MS (1)

static const int DEFAULT_QUEUE_TEST_SIZE = 4;
static const std::string TEST_STRING = "TEST";

struct axFastFileLogVars{
std::string axFilePath, calcString;
AxFastLog fileAx;
LogEnums::Severity testSev;
axFastFileLogVars():axFilePath("data/axTest.txt"),calcString(""),fileAx(LogEnums::FILE, axFilePath), testSev(LogEnums::INFO){}
};

struct axFastConsLogVars{
std::string consAxFilePath, calcString;
AxFastLog consoleAx;
LogEnums::Severity testSev;
axFastConsLogVars():consAxFilePath("data/axTransTest.txt"), calcString(""), consoleAx(LogEnums::CNSL), testSev(LogEnums::INFO){}
};

struct axFastServSockLogVars{
std::string calcString;
AxFastLog servSocketAx;
LogEnums::Severity testSev;
axFastServSockLogVars():calcString(""), servSocketAx(LogEnums::SSKT, PORT),testSev(LogEnums::INFO){}
};

struct axFastCliSockLogVars{
  std::string cliSockAxFilePath, calcString;
  AxFastLog* cliSocketAx;
  LogEnums::Severity testSev;
  axFastCliSockLogVars():cliSockAxFilePath("data/axCliSockTest.txt"),calcString(""),testSev(LogEnums::INFO){}
};

struct invalidAxFastVars{
AxFastLog* invldFileAx, *invldConsAx, *invldServSockAx, *invldCliSockAx;
std::string calcString, testFilePath;
invalidAxFastVars():calcString(""), testFilePath("data/fileTest.txt"){}
};

struct producerConsumerQueueVars{
  NotTrivial nonTriv, nonTriv_1, nonTriv_2;
  folly::ProducerConsumerQueue<NotTrivial> nonTrivQueue;
  producerConsumerQueueVars():nonTriv(), nonTriv_1(), nonTriv_2(), nonTrivQueue(DEFAULT_QUEUE_TEST_SIZE){}
};

struct tcpClientTestVars {
  TcpClient cli;
  std::string calcString, testFilePath;
  tcpClientTestVars():cli(), calcString(""), testFilePath("data/clientTest.txt"){}
};


struct axFastLogVars{
int writeResult;
std::string transFilePath, transFileErrorPath, consFilePath, consErroFilePath, consDbugFilePath,cliSockFilePath, calcString;
LogEnums::Severity testSevInfo, testSevErro, testSevDbug;
size_t queueSize;
axFastLogVars():transFilePath("data/transTest.txt"),transFileErrorPath("/test.txt"), consFilePath("data/consTest.txt"), consErroFilePath("data/consErroTest.txt"), consDbugFilePath("data/consDbugTest.txt"),cliSockFilePath("data/cliSockTest"),calcString(""), testSevInfo(LogEnums::INFO),testSevErro(LogEnums::ERRO), testSevDbug(LogEnums::DEBG){}
};


BOOST_FIXTURE_TEST_SUITE(fileAxFastLogSuite, axFastFileLogVars);

BOOST_AUTO_TEST_CASE(fileAxFastLogTest){
  fileAx.log(TEST_STRING,testSev);

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
 BOOST_CHECK_MESSAGE(calcString.compare(TEST_STRING) == 0, "ERROR: Expected string not equal to calculated string" );
}

BOOST_AUTO_TEST_SUITE_END();

BOOST_FIXTURE_TEST_SUITE(consoleAxLogSuite, axFastConsLogVars);

BOOST_AUTO_TEST_CASE(consoleAxFastLogTest){

  //read from file and write to calculatedString
  std::streambuf *psbuf, *backup;
  std::ofstream myWriteFile;
  std::ifstream myReadFile;
  myWriteFile.open(consAxFilePath.c_str());

  backup = std::cout.rdbuf();     // back up cout's streambuf
  psbuf = myWriteFile.rdbuf();        // get file's streambuf
  std::cout.rdbuf(psbuf);         // assign psbuf to cout

  consoleAx.log(TEST_STRING,testSev);
  usleep(TIMEOUT_MS*1000);
  std::cout.rdbuf(backup);        // restore cout's original streambuf
  myWriteFile.close();

  myReadFile.open(consAxFilePath.c_str());
  if(myReadFile.is_open()){
    while (!myReadFile.eof()) {
      myReadFile >> calcString;
    }
  }
  myReadFile.close();


 BOOST_CHECK_MESSAGE(calcString.compare(TEST_STRING) == 0, "ERROR: Expected string unequal to calculated string" );
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_FIXTURE_TEST_SUITE(servSocketAxFastLogSuite, axFastServSockLogVars);


BOOST_AUTO_TEST_CASE(servSocketAxFastLogTest){
  TcpClient cli;
  cli.conn(HOST, (PORT));
  servSocketAx.log(TEST_STRING,testSev);
  calcString = cli.receive(1024);
  BOOST_CHECK_MESSAGE(calcString.compare(TEST_STRING)==0, "ERROR: Expected string unequal to calculated string");
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_FIXTURE_TEST_SUITE(cliSocketAxFastLogSuite, axFastCliSockLogVars);

BOOST_AUTO_TEST_CASE(cliSocketAxFastLogTest){
  char cmd[256];
  snprintf(cmd, sizeof(cmd),"nc -l %d > %s &", PORT+11, cliSockAxFilePath.c_str());
  system(cmd);
  memset(cmd, 0, sizeof(cmd));
  usleep(5000*TIMEOUT_MS);
  std::ifstream myReadFile;

  cliSocketAx =  new AxFastLog(LogEnums::CSKT, PORT+11, HOST);
  cliSocketAx->log(TEST_STRING,testSev);
  usleep(1000*TIMEOUT_MS);
  myReadFile.open(cliSockAxFilePath.c_str());
  if(myReadFile.is_open()){
    while (!myReadFile.eof()) {
      myReadFile >> calcString;
    }
  }
  myReadFile.close();
  BOOST_CHECK_MESSAGE(calcString.compare(TEST_STRING) == 0, "ERROR: Expected string not equal to calculated string");
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_FIXTURE_TEST_SUITE(InvalidAxConstructorsSuite,invalidAxFastVars);

BOOST_AUTO_TEST_CASE(InvalidFileAxTester){
 try{
   invldFileAx = new AxFastLog(LogEnums::SSKT,testFilePath);
 }catch(...){
    calcString = "exception";
  }

  BOOST_CHECK_MESSAGE(calcString.compare("exception")== 0, "ERROR: Expected exception not thrown for writing to a closed stream" );
}

BOOST_AUTO_TEST_CASE(InvalidConsAxTester){
 try{
   invldConsAx = new AxFastLog(LogEnums::FILE);
 }catch(...){
    calcString = "exception";
  }

  BOOST_CHECK_MESSAGE(calcString.compare("exception")== 0, "ERROR: Expected exception not thrown for writing to a closed stream" );
}

BOOST_AUTO_TEST_CASE(InvalidServSockAxTester){
 try{
   invldServSockAx = new AxFastLog(LogEnums::CNSL,PORT+5);
 }catch(...){
    calcString = "exception";
  }

  BOOST_CHECK_MESSAGE(calcString.compare("exception")== 0, "ERROR: Expected exception not thrown for writing to a closed stream" );
}

BOOST_AUTO_TEST_CASE(InvalidCliSockAxTester){
 try{
   invldCliSockAx = new AxFastLog(LogEnums::CNSL,PORT+5,HOST);
 }catch(...){
    calcString = "exception";
  }

  BOOST_CHECK_MESSAGE(calcString.compare("exception")== 0, "ERROR: Expected exception not thrown for writing to a closed stream" );
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_FIXTURE_TEST_SUITE(fileTransportSuite, axFastLogVars);


BOOST_AUTO_TEST_CASE(fileTransportTester){
  FileTransport fileTrans(transFilePath);
  fileTrans.write(TEST_STRING);
  usleep(TIMEOUT_MS*1000);
  std::ifstream myReadFile;

  myReadFile.open(transFilePath.c_str());
  if(myReadFile.is_open()){
    while (!myReadFile.eof()) {
      myReadFile >> calcString;
    }
  }
  myReadFile.close();
  BOOST_CHECK_MESSAGE(calcString.compare(TEST_STRING)== 0, "ERROR: Expected string not equal to calculated string" );
}

BOOST_AUTO_TEST_CASE(fileTransportPathErrorTester){
  try{
    FileTransport fileTrans(transFileErrorPath);
  }
  catch(...){
    calcString = "exception";
  }
  BOOST_CHECK_MESSAGE(calcString.compare("exception")== 0, "ERROR: Expected exception not thrown");
}

BOOST_AUTO_TEST_CASE(fileTransportWriteErrorTester){
  FileTransport fileTrans(transFilePath);
  fileTrans.closePath();
  try{
    fileTrans.write(TEST_STRING);
  }catch(...){
    calcString = "exception";
  }

  BOOST_CHECK_MESSAGE(calcString.compare("exception")== 0, "ERROR: Expected exception not thrown for writing to a closed stream" );
}



BOOST_AUTO_TEST_SUITE_END()

BOOST_FIXTURE_TEST_SUITE(consoleTransportSuite, axFastLogVars);

BOOST_AUTO_TEST_CASE(consoleTransportTesterDefault){
  ConsoleTransport consoleTrans;
  std::streambuf *psbuf, *backup;
  std::ofstream myWriteFile;
  std::ifstream myReadFile;
  myWriteFile.open(consFilePath.c_str());

  backup = std::cout.rdbuf();     // back up cout's streambuf
  psbuf = myWriteFile.rdbuf();        // get file's streambuf
  std::cout.rdbuf(psbuf);         // assign psbuf to cout

  consoleTrans.write(TEST_STRING);
  std::cout.rdbuf(backup);        // restore cout's original streambuf
  myWriteFile.close();

  myReadFile.open(consFilePath.c_str());
  if(myReadFile.is_open()){
    while (!myReadFile.eof()) {
      myReadFile >> calcString;
    }
  }
  myReadFile.close();
  BOOST_CHECK_MESSAGE(calcString.compare(TEST_STRING) == 0, "ERROR: Expected string not equal to calculated string");
}
BOOST_AUTO_TEST_CASE(consoleTransportTesterErro){
  ConsoleTransport consoleTrans;
  std::streambuf *psbuf, *backup;
  std::ofstream myWriteFile;
  std::ifstream myReadFile;
  myWriteFile.open(consErroFilePath.c_str());

  backup = std::cerr.rdbuf();     // back up cerr's streambuf
  psbuf = myWriteFile.rdbuf();        // get file's streambuf
  std::cerr.rdbuf(psbuf);         // assign psbuf to cerr

  consoleTrans.write(TEST_STRING,testSevErro);
  std::cerr.rdbuf(backup);        // restore cerr's original streambuf
  myWriteFile.close();

  myReadFile.open(consErroFilePath.c_str());
  if(myReadFile.is_open()){
    while (!myReadFile.eof()) {
      myReadFile >> calcString;
    }
  }
  myReadFile.close();
  BOOST_CHECK_MESSAGE(calcString.compare(TEST_STRING) == 0, "ERROR: Expected string not equal to calculated string");
}

BOOST_AUTO_TEST_CASE(consoleTransportTesterDbug){
  ConsoleTransport consoleTrans;
  std::streambuf *psbuf, *backup;
  std::ofstream myWriteFile;
  std::ifstream myReadFile;
  myWriteFile.open(consDbugFilePath.c_str());

  backup = std::clog.rdbuf();     // back up clog's streambuf
  psbuf = myWriteFile.rdbuf();        // get file's streambuf
  std::clog.rdbuf(psbuf);         // assign psbuf to clog

  consoleTrans.write(TEST_STRING,testSevDbug);
  std::clog.rdbuf(backup);        // restore clog's original streambuf
  myWriteFile.close();

  myReadFile.open(consDbugFilePath.c_str());
  if(myReadFile.is_open()){
    while (!myReadFile.eof()) {
      myReadFile >> calcString;
    }
  }
  myReadFile.close();
  BOOST_CHECK_MESSAGE(calcString.compare(TEST_STRING) == 0, "ERROR: Expected string not equal to calculated string");
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_FIXTURE_TEST_SUITE(serverSocketTransportSuite, axFastLogVars);

BOOST_AUTO_TEST_CASE(serverSocketTransportTester){
  //set up socketTranport object and sockets

  TcpClient cli;
  ServerSocketTransport serverSocketTransport(PORT+1);
  usleep(TIMEOUT_MS*1000);
  cli.conn(HOST, (PORT+1));
  usleep(TIMEOUT_MS*1000);
  BOOST_CHECK_MESSAGE(serverSocketTransport.clientConnected(), "ERROR: Unexpected return from clientConnected method.");
  serverSocketTransport.write(TEST_STRING);
  calcString = cli.receive(1024);
  BOOST_CHECK_MESSAGE(calcString.compare(TEST_STRING)==0, "ERROR: Server Socket string is incorrect.");

}

  BOOST_AUTO_TEST_CASE(socketPortTester){
    try{
      ServerSocketTransport serverSocketTransport(1);
    }
    catch(...){
      calcString = "exception";
    }
    BOOST_CHECK_MESSAGE(calcString.compare("exception") ==0, "ERROR: Exception not thrown as expected");
  }

  BOOST_AUTO_TEST_CASE(socketClientTest){
    ServerSocketTransport serverSocketTransport(PORT+2);
    try{
      ServerSocketTransport serverSocketTransport2(PORT+2);
    }
    catch(...){
      calcString = "exception";
    }
    BOOST_CHECK_MESSAGE(calcString.compare("exception") == 0, "ERROR: Exception not thrown as expected when listening twice on same port");
  }

  BOOST_AUTO_TEST_CASE(socketTimeoutTest){
    ServerSocketTransport serverSocketTransport(PORT+3);
    writeResult = serverSocketTransport.write(TEST_STRING);

    BOOST_CHECK_MESSAGE(writeResult == -1, "ERROR: Unexpected return from ServerSocketTransport write method");
  }

  BOOST_AUTO_TEST_CASE(socketSendDataTest){
    TcpClient cli;
    ServerSocketTransport serverSocketTransport(PORT+4);
    cli.conn(HOST, (PORT+4));
    usleep(TIMEOUT_MS*1000);
    BOOST_CHECK_MESSAGE(serverSocketTransport.clientConnected(), "ERROR: Client connection.");
    serverSocketTransport.closeSocket();
    writeResult = serverSocketTransport.write(TEST_STRING);


    BOOST_CHECK_MESSAGE(writeResult == -1, "ERROR: Unexpected return from ServerSocketTransport write when writing to a closed socket");
  }

BOOST_AUTO_TEST_SUITE_END()

BOOST_FIXTURE_TEST_SUITE(clientSocketTransportSuite, axFastLogVars);

BOOST_AUTO_TEST_CASE(clientSocketTransportTester){
  char cmd[256];
  snprintf(cmd, sizeof(cmd),"nc -l %d > %s &", PORT+8, cliSockFilePath.c_str());
  system(cmd);
  memset(cmd, 0, sizeof(cmd));
  usleep(4000*TIMEOUT_MS);
  std::ifstream myReadFile;

  ClientSocketTransport clientSocketTransport(PORT+8, HOST);
  clientSocketTransport.write(TEST_STRING);
  usleep(2000*TIMEOUT_MS);
  myReadFile.open(cliSockFilePath.c_str());
  if(myReadFile.is_open()){
    while (!myReadFile.eof()) {
      myReadFile >> calcString;
    }
  }
  myReadFile.close();
  BOOST_CHECK_MESSAGE(calcString.compare(TEST_STRING) == 0, "ERROR: Expected string not equal to calculated string");
}

BOOST_AUTO_TEST_CASE(cliSocketLogFailTest){
  char cmd[256];
  snprintf(cmd, sizeof(cmd),"nc -l %d > %s &", PORT+9, cliSockFilePath.c_str());
  system(cmd);
  memset(cmd, 0, sizeof(cmd));
  usleep(4000*TIMEOUT_MS);
  ClientSocketTransport clientSocketTransport(PORT+9, HOST); 
  clientSocketTransport.closeClient(); 
  if(clientSocketTransport.write(TEST_STRING)== WRITE_FAILURE){ 
    calcString = "failed";
  }


  BOOST_CHECK_MESSAGE(calcString.compare("failed") == 0, "ERROR:Return value not as expected");
}

BOOST_AUTO_TEST_CASE(cliSocketConnFailTest){
  try{
   ClientSocketTransport clientSocketTransport(PORT+10, HOST);
  }catch(...){
    calcString = "exception";
  }

  BOOST_CHECK_MESSAGE(calcString.compare("exception") == 0, "ERROR:Exception not thrown as expected");
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
  BOOST_CHECK_MESSAGE(nonTrivQueue.sizeGuess() == 2, "ERROR: Queue size should be 2.");
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_FIXTURE_TEST_SUITE(tcpClientTestSuite,tcpClientTestVars);

BOOST_AUTO_TEST_CASE(tcpClientTest){

char cmd[256];
snprintf(cmd, sizeof(cmd),"nc -l %d > %s &", PORT+6, testFilePath.c_str());
system(cmd);
memset(cmd, 0, sizeof(cmd));

usleep(4000*TIMEOUT_MS);

std::ifstream myReadFile;

cli.conn(HOST, PORT+6);
cli.send_data(TEST_STRING);

usleep(3000*TIMEOUT_MS);
myReadFile.open(testFilePath.c_str());

if(myReadFile.is_open()){
    while (!myReadFile.eof()) {
      myReadFile >> calcString;
    }
}
  myReadFile.close();
  BOOST_CHECK_MESSAGE(calcString.compare(TEST_STRING) == 0, "ERROR: Expected string not equal to calculated string");

}

/*BOOST_AUTO_TEST_CASE(tcpClientPlainIPTest){

char cmd[256];
snprintf(cmd, sizeof(cmd),"nc -l %d > %s &", PORT+12, testFilePath.c_str());
system(cmd);
memset(cmd, 0, sizeof(cmd));

usleep(4000*TIMEOUT_MS);

std::ifstream myReadFile;

cli.conn("127.0.0.1", PORT+12);
cli.send_data(TEST_STRING);

usleep(2000*TIMEOUT_MS);
myReadFile.open(testFilePath.c_str());

if(myReadFile.is_open()){
    while (!myReadFile.eof()) {
      myReadFile >> calcString;
    }
}
  myReadFile.close();
  BOOST_CHECK_MESSAGE(calcString.compare(TEST_STRING) == 0, "ERROR: Expected string not equal to calculated string");

}
*/
BOOST_AUTO_TEST_SUITE_END()
