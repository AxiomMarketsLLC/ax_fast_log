TOPDIR   := $(HOME)/dev/monitor_worker/
LIBDIR   := $(TOPDIR)lib/
NAME     := libax_fast_log.so
LIB		 := $(LIBDIR)$(NAME)
SOFLAG   := -fpic -Wall
SOLINK	 := -shared	
all:	so


CC       := g++

src = $(wildcard *.cpp)
objects = $(src:.cpp=.o)

so:		dep $(LIB)
$(LIB): $(objects)
	$(CC) $(SOFLAG) $(SOLINK) -o $@ $^
.PHONY: clean
clean:
	rm -f $(objects) $(LIB)

	
