#/bin/bash

mkdir build
mkdir build/makefiles
cd build/makefiles
conan install ../../src --build=missing
cmake ../../src -G"Unix Makefiles"

#qtbase5-dev
