#!/bin/sh
make clean
make perf
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$(/usr/bin/pwd)/lib
cd perf
./ax_fast_log_perf
