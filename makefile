TOPDIR   := $(HOME)/dev/ax_fast_log/
SRCDIR	 := $(TOPDIR)src/
LIBDIR   := $(TOPDIR)lib/
SONAME	 := libax_fast_log.so
SOTARGET := $(LIBDIR)$(SONAME)
ANAME	 := libax_fast_log.a
ATARGET  := $(LIBDIR)$(ANAME)
CXXFLAGS  = -fPIC -Wall -std=c++1y
SOLDFLAGS = -shared -lboost_thread	
ALDFLAGS = -lboost_thread

CXX      := g++

SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

all:	$(SOTARGET)

archive:	$(ATARGET)

$(SOTARGET):	$(OBJECTS)
	$(CXX) $(SOLDFLAGS) -o $@ $^

$(ATARGET):	$(OBJECTS)
	ar rcs -o $@ $^

$(SOURCES:.cpp=.d):%.d:%.cpp	
	$(CXX) $(CXXFLAGS) -MM $< >$@
include $(SRCS:.cpp=.d)

.PHONY: clean
clean:
	rm -f $(OBJECTS) $(TARGET)

	
