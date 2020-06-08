# PLD-COMP
## Setup
You must install antlr4 runtime libraries.

Edit ./compile_platform.sh file to fix the lib location
## Compile

Run **./compile_(yourplatform).sh** to compile the debugger

## Run the compiler
```bash
$ ./ifcc your_program.c
```
It generates **output.s**
```bash
$ ./assemble_and_link
```
It generates **output.out**

You can run your program now!

```bash
./output.out
echo $?
```
