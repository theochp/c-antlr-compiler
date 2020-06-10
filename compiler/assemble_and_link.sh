#!/bin/sh
as -o temp.o output.s
gcc -o output.out temp.o
rm temp.o
