#!/bin/sh
make clean
make test
cd test
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$(CLI_HOME)/lib/
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$(CLI_HOME)/libs/
./ax_fast_log_test
