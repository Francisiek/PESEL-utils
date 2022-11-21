#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

#include "pesel-functions.h"
#include "pesel-options.h"

int main(int argc, char* argv[]) {
	int c;
	while ((c = getopt_long(argc, argv, optstring,
			long_options, 0)) != -1) {
		if (c == '?') {
			printe(EXIT_FAILURE, "Wrong option %s\n", argv[optind - 1]);
		} else if (c == ':') {
			printe(EXIT_FAILURE, "Missing option argument %s\n", argv[optind - 1]);
		} else if (c == 1) {
			printe(EXIT_FAILURE, "Unnecessary argument %s\n", argv[optind - 1]);
		}
		
		if (isoptset[c] == false)
			isoptset[c] = true;
		else
			printe(EXIT_FAILURE, "Option %c set two times\n", c);

		if (c == 'p')
			pesel_arg = optarg;
		else if (c == 'd')
			date_arg = optarg;
		else if (c == 'o')
			ordinals_arg = optarg;
		else if (c == 's')
			gender_arg = optarg;
	}

	printf("pesel arg %s\n", pesel_arg);
	printf("date arg %s\n", date_arg);
	printf("ordinals arg %s\n", ordinals_arg);
	printf("gender arg %s\n", gender_arg);

	for (size_t i = 0; i < 128; i++) {
		if (isoptset[i] == true) 
			printf("%c set\n", i);
	}

	// check option's relations
	if (isoptset['c'] ^ isoptset['g']) {
		if (isoptset['c']) {
			if (!(isoptset['p'] || isoptset['o'] || isoptset['d']))
				printe(EXIT_FAILURE, "One of -[pod] needed with -c\n");

			int opt;
			if (opt = areotheropt("cpod")) {
				printe(EXIT_FAILURE, "Wrong option -%c set with -c\n", opt);
			}
		} else if (isoptset['g']) { // 'if' is unnecessary here
			if (!(isoptset['d'] && isoptset['s']))
				printe(EXIT_FAILURE, "-d and -g needed with -g\n");
			
			if (isoptset['o'] && isoptset['a']) 
				printe(EXIT_FAILURE, "Options -o and -a can't be set together\n");

			int opt;
			if (opt = areotheropt("gdsoa")) {
				printe(EXIT_FAILURE, "Wrong option -%c set with -g\n", opt);
			}
		}
	} else 
		printe(EXIT_FAILURE, "one -c or -g operator is needed\n");

	if (isoptset['p']) {
		if ()
	}

	return EXIT_SUCCESS;
}