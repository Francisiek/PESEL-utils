#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

#include "pesel-functions.h"
#include "pesel-options.h"

//#define DEBUG_MODE 1

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
			printe(EXIT_FAILURE, "Option -%c set two times\n", c);

		if (c == 'p')
			pesel_arg = optarg;
		else if (c == 'd')
			date_arg = optarg;
		else if (c == 'o')
			ordinals_arg = optarg;
		else if (c == 's')
			gender_arg = optarg;
	}

	#ifdef DEBUG_MODE
	printf("pesel arg %s\n", pesel_arg);
	printf("date arg %s\n", date_arg);
	printf("ordinals arg %s\n", ordinals_arg);
	printf("gender arg %s\n", gender_arg);
	
	for (size_t i = 0; i < 128; i++) {
		if (isoptset[i] == true) 
			printf("%c set\n", i);
	}
	#endif

	// check option's relations
	if (isoptset['c'] ^ isoptset['g']) {
		if (isoptset['c']) {
			if (!(isoptset['p'] || isoptset['o'] || isoptset['d'] || isoptset['s']))
				printe(EXIT_FAILURE, "One of -[pods] needed with -c\n");

			int opt;
			if (opt = areotheropt("cpods")) {
				printe(EXIT_FAILURE, "Wrong option -%c set with -c\n", opt);
			}
		} else if (isoptset['g']) { // 'if' is unnecessary here
			if (!(isoptset['d'] && (isoptset['s'] || isoptset['o'])))
				printe(EXIT_FAILURE, "-d and (-s or -o) needed with -g\n");
			
			if (isoptset['o'] && isoptset['a']) 
				printe(EXIT_FAILURE, "Options -o and -a can't be set together\n");

			int opt;
			if (opt = areotheropt("gdsoa")) {
				printe(EXIT_FAILURE, "Wrong option -%c set with -g\n", opt);
			}
		}
	} else 
		printe(EXIT_FAILURE, "one -c or -g operator is needed\n");

	// check if option format is valid
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

	#ifdef DEBUG_MODE
	printf("pesel %s\n", pesel_to_string(pesel_number));
	printf("date %s\n", date_to_string(pesel_bdate));
	printf("gender %d\n", gender);
	printf("ordinals %u\n", ordinals);
	#endif

	if (isoptset['c'] ^ isoptset['g']) {
		if (isoptset['c']) {
			if (isoptset['p']) {
				if (validate_pesel(pesel_number) == NOERROR) {
					printf("Pesel %s is good\n", pesel_to_string(pesel_number));
					char* peseldate = date_to_string(date_from_pesel(pesel_number));
					printf("Date: %s\n", peseldate);
					free(peseldate);
					Gender_t gtmp = gender_from_pesel(pesel_number);
					printf("Gender: %s\n", (gtmp == FEMALE) ? "female" : "male");
				} else {
					printf("Invalid pesel %s\n", pesel_to_string(pesel_number));
					// error message
				}
				printf("\n");
			}

			if (isoptset['o']) {
				if (check_ordinals(ordinals) == NOERROR) {
					printf("Ordinals %u are good\n", ordinals);
				} else {
					printf("Invalid ordinals %u\n", ordinals);
					// error message
				}
				printf("\n");
			}

			if (isoptset['d']) {
				if (check_bdate(pesel_bdate) == NOERROR) {
					printf("Date %s is good\n", date_to_string(pesel_bdate));
				} else {
					printf("Invalid pesel date %s\n", date_to_string(pesel_bdate));
					// error message
				}
				printf("\n");
			}

			if (isoptset['s']) {
				if (check_gender(gender) == NOERROR) {
					printf("Gender %s is good\n", gender_arg);
				} else {
					printf("Invalid %s gender\n", gender_arg);
				}
				printf("\n");
			}
			
		} else if (isoptset['g']) { // 'if' is unnecessary here
			if (check_bdate(pesel_bdate) != NOERROR) {
				printf("Invalid pesel date %s\n", date_to_string(pesel_bdate));
				return EXIT_FAILURE;
				// error message
			}
			
			if (isoptset['s'] && check_gender(gender) != NOERROR) {
				printf("Invalid %s gender\n", gender_arg);
				return EXIT_FAILURE;
			}
			
			if (isoptset['o']) {
				if (check_ordinals(ordinals) != NOERROR) {
					printf("Invalid ordinals %u\n", ordinals);
					return EXIT_FAILURE;
				}

				if (isoptset['s'] && check_ordinals_gender(ordinals, gender) != NOERROR) {
					printf("Invalid ordinals %u for %s gender \n", ordinals, gender_arg);
					return EXIT_FAILURE;
				}
			}
			
			PESEL_data_s pdata;
			pdata.birth_date = pesel_bdate;
			if (isoptset['s'])
				pdata.gender = gender;

			if (isoptset['a']) {
				PESEL_s* all_pesels = generate_all_pesels(pdata);
				if (all_pesels == NULL) {
					printf("Could not generate all pesels\n");
					return EXIT_FAILURE;
					// error messages
				}

				for (size_t i = 0; i < ORDINALS_SIZE; i++) {
					printf("%d: %s\n", i + 1, pesel_to_string(all_pesels[i]));
				}
			} else if (isoptset['o']) {
				pdata.gender = gender_from_ordinals(ordinals);

				PESEL_s pn = generate_pesel_with_ordinals(pdata, ordinals);
				if (is_default_pesel_s(pn)) {
					printf("Could not generate pesel\n");
					return EXIT_FAILURE;
				}

				printf("Pesel %s\n", pesel_to_string(pn));
			} else {
				PESEL_s pn = generate_pesel(pdata);
				if (is_default_pesel_s(pn)) {
					printf("Could not generate pesel\n");
					return EXIT_FAILURE;
				}

				printf("Pesel %s\n", pesel_to_string(pn));
			}
		}
	}

	return EXIT_SUCCESS;
}