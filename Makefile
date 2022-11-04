all: pesel-tool

main.o: main.c pesel-functions.h
	gcc -c main.c -g -o $@

pesel-functions.o: pesel-functions.c
	gcc -c pesel-functions.c -g -o $@

pesel-tool: main.o pesel-functions.o
	gcc main.o pesel-functions.o -g -o $@

install:

clean:
