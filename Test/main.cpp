#include "AxFastLog.hpp"
#include <boost/test/unit_test.hpp>
#define BOOST_TEST_MODULE AxFastLog test


struct axFastLogVars{
std:: string axFilePath = "../Desktop/axTest.txt"
std:: string  transFilePath= "../Desktop/transTest.txt"
std::string testString = "TEST";
Severity testSev = Severity::INFO;
std:: string expectedString = testString;
std:: string calculatedString;
std:: string  transCalcStr;
std:: string transExpectedStr = testString;

};


BOOST_FIXTURE_TEST_SUITE(axFastLogSuite, axFastLogVars);

BOOST_AUTO_TEST_CASE(axFastLogTest){
  AxFastLog ax(LogEnums:: FILE, axFilePath);
  ax.log(testString, testSev);

  //read from file and write to calculatedString
  ifstream myReadFile;
  myReadFile.open(axFilePath);
  if(myReadFile.is_open()){
    while (!myReadFile.eof()) {
      myReadFile >> std:: calculatedString;
    }
  }
  myReadFile.close();
  BOOST_CHECK_MESSAGE(std::strcmp(expectedString,calculatedString) == 0, "ERROR: Expected string not equal to calculated string" );

}


BOOST_AUTO_TEST_CASE(fileTransportTester){
  FileTransport fileTrans(transFilePath);
  fileTrans.write(testString);

  ifstream myReadfile;
  myReadFile.open(transFilePath);
  if(myReadFile.is_open()){
    while (!myReadFile.eof()) {
      myReadFile >> std:: transCalcStr;
    }
  }
  myReadFile.close();
  BOOST_CHECK_MESSAGE(std::strncmp(transCalcStr, transExpectedStr) == 0, "ERROR: Expected string not equal to calculated string" );

}

BOOST_AUTO_TEST_SUITE_END()
