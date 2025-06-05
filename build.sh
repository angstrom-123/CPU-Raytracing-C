#!/bin/bash

# clang -o ./target/ray-trace ./src/*.c `pkg-config --libs --cflags sdl3` -Weverything
clang `pkg-config --libs --cflags sdl3` ./src/*.c -o ./target/ray-trace -lm -Weverything
