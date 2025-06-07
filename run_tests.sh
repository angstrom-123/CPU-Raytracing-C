#!/bin/bash

rm test_summary.txt

valgrind --tool=memcheck --leak-check=full --track-origins=yes ./target/ray-trace \
	&> test_summary.txt
