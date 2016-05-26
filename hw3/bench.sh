#!/bin/bash
set -e

PARALLEL=4
NINJA_EXE=ninja-build
if ! which $NINJA_EXE >/dev/null 2>&1; then
	NINJA_EXE=ninja
fi
if ! which $NINJA_EXE >/dev/null 2>&1; then
	echo "Install ninja pls!"
	exit 1
fi

if [ ! -d build ]; then
	mkdir build
fi

cd build
cmake -DCMAKE_BUILD_TYPE=Release -GNinja ..
$NINJA_EXE

echo
echo =======================
echo Benchmarking C version
echo =======================
echo
mpirun -np 4 ./bubble_sort_c

echo
echo =======================
echo Benchmarking C++ version
echo =======================
echo
mpirun -np 4 ./bubble_sort_cpp
