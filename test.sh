#!/bin/sh
make clean
make test
cd test
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$(/usr/bin/pwd)/../lib/
./ax_fast_log_test
