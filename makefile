TOPDIR   := $(PWD)/
SRCDIR	 := $(TOPDIR)src/
INCDIR	 := $(TOPDIR)include
TESTDIR  := $(TOPDIR)test/
PERFDIR	 := $(TOPDIR)perf/
LIBDIR   := $(TOPDIR)lib/
SONAME	 := libax_fast_log.so
SOTARGET := $(LIBDIR)$(SONAME)
ANAME	 := libax_fast_log.a
ATARGET  := $(LIBDIR)$(ANAME)

#prod flags
#CXXFLAGS  = -fPIC -O2 -std=c++11 -Wall -I$(INCDIR) 
#dbg flags
CXXFLAGS =--coverage -fPIC -g -Wall -I$(INCDIR) -I . -DDEBUG -std=c++11
SOLDFLAGS =-shared --coverage -lpthread -lboost_system -lboost_thread -lboost_chrono
ALDFLAGS =-lboost_thread-mt

SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

all:	$(SOTARGET)

archive:	$(ATARGET)

test:	$(SOTARGET)	
	cd $(TESTDIR); make
perf:	$(SOTARGET)
	cd $(PERFDIR); make 
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
	cd $(TESTDIR); make clean
	cd $(PERFDIR); make clean
