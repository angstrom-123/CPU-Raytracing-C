#!/bin/bash

clang -o ./target/ray-trace ./src/*.c `pkg-config --libs --cflags sdl3` -Weverything
