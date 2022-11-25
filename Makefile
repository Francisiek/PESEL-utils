flags = -g -Wall -std=c17

all: pesel-tool

main.o: main.c pesel-functions.h pesel-options.h
	gcc $(flags) -c main.c -o $@

pesel-functions.o: pesel-functions.c
	gcc $(flags) -c pesel-functions.c -o $@ 

pesel-options.o: pesel-options.c
	gcc $(flags) -c pesel-options.c -o $@

pesel-tool: main.o pesel-functions.o pesel-options.o
	gcc $(flags) main.o pesel-functions.o pesel-options.o -o $@ -lm

install:

clean:
