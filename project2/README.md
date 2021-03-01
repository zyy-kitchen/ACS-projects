# ACS-project2
## Overview
In this project, the team write code to read 4KB blocks from a file and try to use the strategy of multitreading to compressed each 4k file block with the help of zlib and write each compressed file block into a output file in sequence. After testing with the input data size of 10GB file, the team find out that with the help of multithreading, the compression time can resuce significantly.S

## Pthread
In nowadays shared memory multiprocessor architectures, parallelism can be achieved by using threads. In the past, hardware vendors always implemented their own proprietary versions of threads while in recent years, there is a standardized C language threads programming interface was created: POSIX threads, or Pthreads.


## Coding Structure
The project is written in C language and is comprised of 1 single compression.c file.

## Compiling & Running

In order to run the project, you can use the following command to compile the compile the compression.c code.
#### gcc -Wall compression.c -lz -pthread
#### ./a.out input.txt(The input file name) output.txt(The output file name) record.txt(The record file record compressed length for each 4k block)
#### ex: ./a.out test2 test2_1 record_1

## User Interface

Once you run the a.out file, the program will first check if you enter enough arguments on to the terminal, if the program detects that you do not enter enough file information, it will exit and return a error message. Once the argument detection is passed. The program will try to open the input file with the given file name. If the program detect the input file cannot be open.
