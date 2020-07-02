#!/bin/sh
./compile.sh $1
./output.out
a=$?
echo "\n"
echo "Retour:\n"
echo $a
