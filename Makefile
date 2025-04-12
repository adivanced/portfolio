main: main.o func.o
	gcc main.o func.o -ggdb -o main

main.o: main.c read.h
	gcc main.c -ggdb -c

func.o: func.asm
	nasm -f elf64 -g -F dwarf func.asm -o func.o