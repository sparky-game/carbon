#!/bin/sh

IN="${0%.*}.cc"
OUT="${0%.*}"
CXX="c++ -std=c++20 -O3"

trap "rm -f $OUT" EXIT
$CXX $IN -o $OUT && $OUT "$@"
