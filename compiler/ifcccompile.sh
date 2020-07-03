#!/bin/sh
/home/theo/shared/PLD-COMP/compiler/ifcc $1
as -o temp.o x86output.s
gcc -o output.out temp.o
echo "\n"
rm temp.o
