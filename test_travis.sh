#!/bin/sh
mkdir lib
make clean
make test
cd test
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/build/travis/AxiomMarketsLLC/ax_fast_log/lib/
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/home/build/travis/AxiomMarketsLLC/ax_fast_log/libs/
./ax_fast_log_test
