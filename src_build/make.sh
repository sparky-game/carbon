#!/bin/sh

IN="${0%.*}.cc"
OUT="${0%.*}"
CXX="c++ -std=c++20 -O3"

$CXX $IN -o $OUT && $OUT "$@"
rm $OUT
