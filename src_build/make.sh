#!/bin/sh

IN="${0%.*}.cc"
OUT="${0%.*}"
CMD="${CXX:-c++} -std=c++20 ${CXX:+-DCXX=\"$CXX\"} -O3 $IN -o $OUT"

trap "rm -f $OUT" EXIT
$CMD && $OUT "$@"
