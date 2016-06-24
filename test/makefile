TOPDIR   := $(PWD)/../
LIBDIR   := $(TOPDIR)lib/
LIBSDIR  := $(TOPDIR)libs/
INCDIR	 := $(TOPDIR)include/
TARGET   := ax_fast_log_test
CXXFLAGS  = -Wall -g -std=c++11 -I$(INCDIR)
TFLAGS = -Wl,--no-as-needed,--allow-multiple-definition,--start-group -L$(LIBSDIR) -L$(LIBDIR) -lboost_system-mt -lboost_thread-mt -lboost_unit_test_framework-mt -lax_fast_log -Wl,--end-group
 

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
