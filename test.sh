#!/bin/bash

valgrind --tool=memcheck --leak-check=full ./target/raytrace
