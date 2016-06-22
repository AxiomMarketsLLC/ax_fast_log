#!/bin/sh
#make clean
make test
cd test
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/malrazd/ax_fast_log/lib
./ax_fast_log_test
