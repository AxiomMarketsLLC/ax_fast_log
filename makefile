TOPDIR   := $(HOME)/dev/ax_fast_log/
LIBDIR   := $(TOPDIR)lib/
NAME	 := libax_fast_log.so
TARGET	 := $(LIBDIR)$(NAME)
CXXFLAGS = -fPIC -Wall
LDFLAGS	 = -shared	


CXX      := g++

SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

$(TARGET):	$(OBJECTS)
	$(CXX) $(LDFLAGS) -o $@ $^
$(SOURCES:.cpp=.d):%.d:%.cpp	
	$(CXX) $(CXXFLAGS) -MM $< >$@
include $(SRCS:.cpp=.d)

.PHONY: clean
clean:
	rm -f $(OBJECTS) $(TARGET)

	
