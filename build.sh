#!/bin/bash

clang -o ./target/ray-trace ./src/main.c ./src/camera.c `pkg-config --libs --cflags sdl3` -Weverything
