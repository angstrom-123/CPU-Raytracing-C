#!/bin/bash

clang `pkg-config --libs --cflags sdl3` ./src/*.c -o ./target/ray-trace -lm
