#!/bin/sh
make clean
make test
cd test
./ax_fast_log_test
