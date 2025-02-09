#! /bin/bash
set -e
rm -rf `pwd`/build/*
mkdir -p bin
mkdir -p build
cd `pwd`/build &&
    cmake .. &&
    make

cd ..
cp -r `pwd`/src/include `pwd`/lib