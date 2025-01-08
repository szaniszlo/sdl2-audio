#!/bin/bash

set -xe

CFLAGS="-Wall -Werror -pedantic -std=c++11 -ggdb `pkg-config --cflags sdl2`"
LIBS="`pkg-config --libs sdl2`"
# g++ -L/opt/homebrew/Cellar/sdl2/2.30.9/lib -lSDL2 -I/opt/homebrew/include -o sdl-audio src/main.cpp
g++ $CFLAGS -o sdl-audio src/main.cpp $LIBS
