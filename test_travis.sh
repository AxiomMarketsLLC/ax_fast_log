#!/bin/bash
mkdir lib
make clean
make test
cd test
export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:$(PWD)/../lib/"
export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:$(PWD)/../libs/"
./ax_fast_log_test
