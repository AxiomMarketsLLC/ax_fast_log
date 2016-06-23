TOPDIR   := $(PWD)/../
LIBDIR   := $(TOPDIR)lib/
TARGET   := ax_fast_log_perf
INCDIR	 := $(TOPDIR)include/
CXXFLAGS  = -Wall -std=c++1y -I$(INCDIR)
TFLAGS = -lpthread -lboost_system-mt -lboost_thread-mt -L$(LIBDIR) -lax_fast_log 

CXX      := g++ 

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
