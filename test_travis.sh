#!/bin/sh
mkdir lib
make clean
make test
cd test
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$(TRAVIS_BUILD_DIR)/lib/
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$(TRAVIS_BUILD_DIR)/libs/
./ax_fast_log_test
