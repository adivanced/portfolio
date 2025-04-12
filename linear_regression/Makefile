main: main.o matlib.o func.o
	gcc -no-pie main.o func.o matlib.o -o main 

func.o: func.asm
	nasm -f elf64 func.asm -p func.o

matlib.o: matlib.asm
	nasm -f elf64 matlib.asm -o  matlib.o

main.o: main.c parsecsv.h 
	gcc main.c -c
