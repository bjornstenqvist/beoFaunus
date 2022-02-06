#!/bin/bash

rm -fr CMakeCache.txt CMakeFiles/
module purge
module add GCC/6.3.0-2.27
module add CMake/3.5.2
CXX=c++ CC=gcc cmake .
