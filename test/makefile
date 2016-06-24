TOPDIR   := $(PWD)/../
LIBDIR   := $(TOPDIR)lib/
LIBSDIR  := $(TOPDIR)/libs/
INCDIR	 := $(TOPDIR)include/
TARGET   := ax_fast_log_test
CXXFLAGS  = -Wall -g -std=c++11 -I$(INCDIR)
TFLAGS = -lpthread -L$(LIBSDIR) -l:libboost_system-mt.so.1.53.0 -l:libboost_thread-mt.so.1.53.0 -l:libboost_unit_test_framework-mt.so.1.53.0 -L$(LIBDIR) -lax_fast_log 
 

SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

all:	$(TARGET)

$(TARGET):	$(OBJECTS)
	$(CXX) $(TFLAGS) -o $@ $^

$(SOURCES:.cpp=.d):%.d:%.cpp
	$(CXX) $(CXXFLAGS) -MM $< >$@
include $(SRCS:.cpp=.d)

.PHONY: clean
clean:
	rm -f $(OBJECTS) $(TARGET)
