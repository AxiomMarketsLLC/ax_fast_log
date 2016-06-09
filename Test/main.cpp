#include "AxFastLog.hpp"
#include <iostream>


std::string testString = "TEST";

Severity testSev = Severity::INFO;

AxFastLog::testAxFastLog(LogEnums:: FILE, "STRING");

TEST_LOG(testString, testSev){
  testAxFastLog.log(testString, testSev);
}
