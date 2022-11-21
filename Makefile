all: pesel-tool

main.o: main.c pesel-functions.h pesel-options.h
	gcc -Wall -c main.c -g -o $@

pesel-functions.o: pesel-functions.c
	gcc -Wall -c pesel-functions.c -g -o $@ 

pesel-options.o: pesel-options.c
	gcc -Wall -c pesel-options.c -g -o $@

pesel-tool: main.o pesel-functions.o pesel-options.o
	gcc -Wall main.o pesel-functions.o pesel-options.o -g -o $@ -lm

install:

clean:
