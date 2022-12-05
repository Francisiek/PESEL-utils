flags = -g -Wall -std=c17

all: pesel-utils

obj/main.o: src/main.c
	gcc $(flags) -c $? -o $@

obj/pesel-functions.o: src/pesel-functions.c
	gcc $(flags) -c $? -o $@ 

obj/pesel-options.o: src/pesel-options.c
	gcc $(flags) -c $? -o $@

pesel-utils: obj/main.o obj/pesel-functions.o obj/pesel-options.o
	gcc $(flags) $? -o $@ -lm

install:

clean:
