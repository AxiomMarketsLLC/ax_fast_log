TOPDIR   := $(PWD)/../
LIBDIR   := $(TOPDIR)lib/
LIBSDIR	 := $(TOPDIR)/libs/
TARGET   := ax_fast_log_perf
INCDIR	 := $(TOPDIR)include/
CXXFLAGS  = -Wall -O0 -std=c++11 -I$(INCDIR)
PFLAGS = -lpthread -L$(LIBSDIR) -lboost_system-mt -lboost_thread-mt -fopenmp -L$(LIBDIR) -lax_fast_log 

CXX      := g++-4.8 

SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

all:	$(TARGET)

$(TARGET):	$(OBJECTS)
	$(CXX) $(PFLAGS) -o $@ $^

$(SOURCES:.cpp=.d):%.d:%.cpp
	$(CXX) $(CXXFLAGS) -MM $< >$@
include $(SRCS:.cpp=.d)

.PHONY: clean
clean:
	rm -f $(OBJECTS) $(TARGET)
