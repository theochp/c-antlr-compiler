#!/bin/sh
as -o temp.o x86output.s
gcc -o output.out temp.o
rm temp.o
