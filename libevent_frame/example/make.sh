#!/usr/bin/sh

mkdir build -p 
cd build 
#rm ./* -rf
cmake ..
make -j4
cd ..
