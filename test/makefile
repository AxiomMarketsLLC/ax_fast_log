TOPDIR   := $(PWD)/../
LIBDIR   := $(TOPDIR)lib/
#LIBSDIR  := $(TOPDIR)libs/
INCDIR	 := $(TOPDIR)include/
TARGET   := ax_fast_log_test
#prod flags
#CXFLAGS  = -std=c++11 -Wall -I$(INCDIR) 
#dbg flags
CXXFLAGS =--coverage -DDEBUG -Wall -I$(INCDIR) -std=c++11

TFLAGS = --coverage -Wl,--no-as-needed,--allow-multiple-definition,--start-group -L$(LIBDIR) -lboost_system -lboost_thread -lboost_unit_test_framework -lax_fast_log -Wl,--end-group
 

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
