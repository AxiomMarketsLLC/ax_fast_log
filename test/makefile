SHELL = /bin/sh
CC    = g++

FLAGS       = -lpthread -Wl,--no-as-needed,--allow-multiple-definition -lrt
CFLAGS      = -w -g -Ilibprod -Isrc/h -Iboost/include -DVERSION=\"$(GIT_VERSION)\"
CFLAGS_T    = $(CFLAGS) --coverage

BUILD_DATE  = -Xlinker --defsym -Xlinker __BUILD_DATE=$$(date +'%Y%m%d')
GIT_VERSION = $(shell git describe --abbrev=4 --dirty --always --tags)

BOO_32		= boost/lib32/
BOO_64	    = boost/lib64/
BOOST_32    = $(BOO_32)libboost_system.a $(BOO_32)libboost_thread.a $(BOO_32)libboost_chrono.a $(BOO_32)libboost_log.a $(BOO_32)libboost_log_setup.a $(BOO_32)libboost_filesystem.a
BOOST_64    = $(BOO_64)libboost_system.a $(BOO_64)libboost_thread.a $(BOO_64)libboost_chrono.a $(BOO_64)libboost_log.a $(BOO_64)libboost_log_setup.a $(BOO_64)libboost_filesystem.a
BOOST_32_T  = $(BOOST_32) $(BOO_32)libboost_unit_test_framework.a
BOOST_64_T  = $(BOOST_64) $(BOO_64)libboost_unit_test_framework.a

TARGEET = axlog.out
SOURCES = $(shell echo src/*.cpp)
OBJECTS = $(SOURCES:.c=.o)

TARGET_T   = axlogtests
SOURCES_T  = $(shell echo tests/*.cpp) $(SOURCES)

OBJECTS_T  = $(SOURCES_T:.c=.o)

PREFIX = $(DESTDIR)/usr/local
BINDIR = $(PREFIX)/bin

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(LARGE_FILE) $(BUILD_DATE) -o $(TARGET) $(OBJECTS) $(FLAGS) $(BOOST_$(ARCH))

release: $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) $(LARGE_FILE) $(BUILD_DATE) -o $(TARGET) $(SOURCES) $(FLAGS) $(BOOST_$(ARCH))

test: $(OBJECTS_T)
	$(CC) $(CFLAGS_T) $(LARGE_FILE) $(BUILD_DATE) -o $(TARGET_T) $(SOURCES_T) $(FLAGS) $(BOOST_$(ARCH)_T)

profile: $(TARGET)

clean:
	rm -f *.o *.gcov *.gcda *.gcno
distclean: clean
	rm -f $(TARGET) $(TARGET_T)
