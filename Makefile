.PHONY: clean

main: main.o matlib.o
	gcc main.o matlib.o -o main -lm -O3 -ffast-math -funsafe-math-optimizations -fassociative-math -freciprocal-math -mfpmath=sse -mavx2 -ffinite-math-only -fno-signed-zeros

matlib.o: matlib.asm
	nasm -f elf64 matlib.asm -o matlib.o

main.o: main.c parsecsv.h
	gcc main.c -c

clean:
	touch csvs/asm.csv
	rm csvs/asm.csv
	touch csvs/asm.csv
	echo time,iterations > csvs/asm.csv
	touch csvs/C.csv
	rm csvs/C.csv
	touch csvs/C.csv
	echo time,iterations > csvs/C.csv