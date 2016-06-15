#include <stdio.h>
#include <sys/types.h>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>
#include <unistd.h>
#include "../AxFastLog.hpp"
#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE AxFastLog test
#include <boost/test/unit_test.hpp>
#include <boost/test/included/unit_test.hpp>



struct axFastLogVars {
FILE* console;
std::string axFilePath;
std::string transFilePath;
std::string consFilePath;
std::string testString;
LogEnums::Severity testSev;
std::string expectedString;
std::string calculatedString;
std::string transExpectedStr;
std::string transCalcStr;
std::string consCalcStr;
AxFastLog ax;
SafeQueue<std::string> testQueue;
size_t queueSize;
std::string dequeuedString;
axFastLogVars() : axFilePath("data/axTest.txt"), transFilePath("data/transTest.txt"), consFilePath("data/consTest.txt"), transCalcStr(""), consCalcStr(""), testString("TEST"), ax(LogEnums::FILE, axFilePath),
expectedString(testString), transExpectedStr(testString), dequeuedString(""), testSev(LogEnums::INFO), testQueue() {}

};


BOOST_FIXTURE_TEST_SUITE(axFastLogSuite, axFastLogVars);

BOOST_AUTO_TEST_CASE(axFastLogTest){

  ax.log(testString, testSev);
  usleep(1000); //wait 1000 microseconds, avoid race condition with AxFastLog::post()
  //read from file and write to calculatedString
  std::ifstream myReadFile;
  myReadFile.open(axFilePath.c_str());
  if(myReadFile.is_open()){
    while (!myReadFile.eof()) {
      myReadFile>>calculatedString;
    }
  }
 myReadFile.close();
 BOOST_CHECK_MESSAGE(expectedString.compare(calculatedString) == 0, "ERROR: Expected string not equal to calculated string" );
}


BOOST_AUTO_TEST_CASE(fileTransportTester){
  FileTransport fileTrans(transFilePath);
  fileTrans.write(testString);
  usleep(1000);
  std::ifstream myReadFile;
  myReadFile.open(transFilePath.c_str());
  if(myReadFile.is_open()){
    while (!myReadFile.eof()) {
      myReadFile >> transCalcStr;
    }
  }
  myReadFile.close();
  BOOST_CHECK_MESSAGE(transCalcStr.compare(transExpectedStr)== 0, "ERROR: Expected string not equal to calculated string" );
}

BOOST_AUTO_TEST_CASE(consoleTransportTester){
  ConsoleTransport consoleTrans(std::cout);
  std::streambuf *psbuf, *backup;
  std::ofstream myWriteFile;
  std::ifstream myReadFile;
  myWriteFile.open(consFilePath.c_str());

  backup = std::cout.rdbuf();     // back up cout's streambuf
  psbuf = myWriteFile.rdbuf();        // get file's streambuf
  std::cout.rdbuf(psbuf);         // assign streambuf to cout

  consoleTrans.write(testString);
  std::cout.rdbuf(backup);        // restore cout's original streambuf
  myWriteFile.close();

  myReadFile.open(consFilePath.c_str());
  if(myReadFile.is_open()){
    while (!myReadFile.eof()) {
      myReadFile >> consCalcStr;
    }
  }
  myReadFile.close();
  BOOST_CHECK_MESSAGE(testString.compare(consCalcStr) == 0, "ERROR: Expected string not equal to calculated string");
}


BOOST_AUTO_TEST_CASE(safeQueueTester){
  testQueue.enqueue(testString);
  queueSize= testQueue.size();
  BOOST_CHECK_MESSAGE(queueSize == 1, "ERROR:Item not enqueued.");
  dequeuedString = testQueue.dequeue();
  BOOST_CHECK_MESSAGE(dequeuedString.compare(expectedString) == 0,"ERROR: The dequeued string is incorrect");
}

BOOST_AUTO_TEST_SUITE_END()
