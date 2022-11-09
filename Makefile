all: pesel-tool

main.o: main.c pesel-functions.h
	gcc -Wall -c main.c -g -o $@

pesel-functions.o: pesel-functions.c
	gcc -Wall -c pesel-functions.c -g -o $@ 

pesel-tool: main.o pesel-functions.o
	gcc -Wall main.o pesel-functions.o -g -o $@ -lm

install:

clean:
