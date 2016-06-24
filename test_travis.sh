#!/bin/sh
mkdir lib
make clean
make test
cd test
export "LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$(1)/lib/"
export "LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$(1)/libs/"
./ax_fast_log_test
