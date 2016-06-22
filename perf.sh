#!/bin/sh
#make clean
make perf
cd perf
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/tmcdaniel/dev/ax_fast_log/lib
./ax_fast_log_perf
