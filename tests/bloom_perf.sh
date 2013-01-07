#!/bin/bash

BITS="16"

cc -I../src  -O3 -Wall   -m64    bloom_perf.c   -o bloom_perf.none
for bits in $BITS
do
cc -I../src  -DHASH_BLOOM=$bits -O3 -Wall   -m64    bloom_perf.c   -o bloom_perf.$bits
done

for bits in none $BITS
do
echo
echo "using $bits-bit filter:"
./bloom_perf.$bits 10
done

