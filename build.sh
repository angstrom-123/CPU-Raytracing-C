#!/bin/bash

error_message() {
	echo 'build: invalid build specification'
	echo 'flags can be "--release" or "--debug" or "--test"'
	exit 1
}

build_release() {
	echo 'building release'
	clang `pkg-config --libs --cflags sdl3` -DRELEASE ./src/*.c -o ./target/ray-trace -lm -O3
	exit 0
}

build_debug() {
	echo 'building debug'
	clang `pkg-config --libs --cflags sdl3` -DDEBUG ./src/*.c -o ./target/ray-trace -lm -O0 -Wall -Wextra
	exit 0
}

build_test() {
	echo 'building test'
	clang `pkg-config --libs --cflags sdl3` -DUNIT_TEST ./src/*.c -o ./target/ray-trace -lm -O0 -Wall -Wextra
	exit 0
}

num=${#@} 
if [ $num -gt 1 ]; then
	error_message
elif [ $num -eq 0 ]; then 
	build_debug 
else 
	case "$@" in 
		--release)
			build_release
			;;
		-r)
			build_release
			;;
		--debug)
			build_debug
			;;
		-d)
			build_debug
			;;
		--test)
			build_test
			;;
		-t)
			build_test
			;;
		*)
			error_message
			;;
	esac
fi
