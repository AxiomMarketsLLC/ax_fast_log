#include <stdio.h>
#include <sys/types.h>
#include <fstream>
#include <iostream>
#include <string>
#include <unistd.h>
#include "../AxFastLog.hpp"
#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE AxFastLog test
#include <boost/test/unit_test.hpp>
#include <boost/test/included/unit_test.hpp>



struct axFastLogVars {
std::string axFilePath;
std::string transFilePath;
std::string testString;
LogEnums::Severity testSev;
std::string expectedString;
std::string calculatedString;
std::string transExpectedStr;
std::string transCalcStr;
AxFastLog ax;

axFastLogVars() : axFilePath("data/axTest.txt"), transFilePath("data/transTest.txt"), transCalcStr(""), testString("TEST"), ax(LogEnums::FILE, axFilePath),
expectedString(testString), transExpectedStr(testString), testSev(LogEnums::INFO) {}

};


BOOST_FIXTURE_TEST_SUITE(axFastLogSuite, axFastLogVars);

BOOST_AUTO_TEST_CASE(axFastLogTest){
  
  ax.log(testString, testSev);
  usleep(100); //wait 100 microseconds, avoid race condition with AxFastLog::post()
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

BOOST_AUTO_TEST_SUITE_END()
