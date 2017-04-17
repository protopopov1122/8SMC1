#!/usr/bin/env bash
# Run from project root

mkdir -p $1/old-build
make -f ./misc/old-build/Makefile stub BUILD=./build/old-build $2
make -f ./misc/old-build/Makefile all BUILD=./build/old-build $2 WX=$3 WXLIB=$4
rm *.o
rm *.png
rm *.res