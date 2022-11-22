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

	PESEL_s pesel_number;
	PESEL_bdate_s pesel_bdate;
	Gender_t gender;
	uint ordinals;

	if (isoptset['p']) {
		if (check_pesel_arg(pesel_arg))
			printe(EXIT_FAILURE, "Invalid -p argument\n");
		pesel_number = pesel_from_string(pesel_arg);	
	}

	if (isoptset['d']) {
		if (check_date_arg(date_arg))
			printe(EXIT_FAILURE, "Invalid -d argument\n");
		pesel_bdate = date_from_string(date_arg);
	}

	if (isoptset['o']) {
		if (check_ordinals_arg(ordinals_arg))
			printe(EXIT_FAILURE, "Invalid -o argument\n");
		ordinals = atoi(ordinals_arg);
	}

	if (isoptset['s']) {
		if (check_gender_arg(gender_arg))
			printe(EXIT_FAILURE, "Invalid -s argument\n");
		gender = gender_from_string(gender_arg);
	}

	printf("pesel %s\n", pesel_to_string(pesel_number));
	printf("date %s\n", date_to_string(pesel_bdate));

	return EXIT_SUCCESS;
}