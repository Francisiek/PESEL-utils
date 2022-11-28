#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

#include "pesel-functions.h"
#include "pesel-options.h"

//#define DEBUG_MODE 1

int main(int argc, char* argv[]) {
	char* program_name = argv[0];
	int c;
	while ((c = getopt_long(argc, argv, optstring,
			long_options, 0)) != -1) {
		if (c == '?') {
			printex(EXIT_FAILURE, "Wrong option %s\n", argv[optind - 1]);
		} else if (c == ':') {
			printex(EXIT_FAILURE, "Missing option argument %s\n", argv[optind - 1]);
		} else if (c == 1) {
			printex(EXIT_FAILURE, "Unnecessary argument %s\n", argv[optind - 1]);
		}
		
		if (isoptset[c] == false)
			isoptset[c] = true;
		else
			printex(EXIT_FAILURE, "Option -%c set two times\n", c);

		if (c == 'p')
			pesel_arg = optind;
		else if (c == 'd')
			date_arg = optind;
		else if (c == 'o')
			ordinals_arg = optind;
		else if (c == 's')
			gender_arg = optind;
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
				printex(EXIT_FAILURE, "One of -[pods] needed with -c\n");

			int opt;
			if (opt = areotheropt("cpods")) {
				printex(EXIT_FAILURE, "Wrong option -%c set with -c\n", opt);
			}
		} else if (isoptset['g']) { // 'if' is unnecessary here
			if (!(isoptset['d'] && (isoptset['s'] || isoptset['o'])))
				printex(EXIT_FAILURE, "-d and (-s or -o) needed with -g\n");
			
			if (isoptset['o'] && isoptset['a']) 
				printex(EXIT_FAILURE, "Options -o and -a can't be set together\n");

			int opt;
			if (opt = areotheropt("gdsoa")) {
				printex(EXIT_FAILURE, "Wrong option -%c set with -g\n", opt);
			}
		}
	} else 
		printex(EXIT_FAILURE, "one -c or -g operator is needed\n");

	/*
	// check if option format is valid
	PESEL_s pesel_number;
	PESEL_bdate_s pesel_bdate;
	Gender_t gender;
	uint ordinals;
	*/

	if (isoptset['p']) {
		if (check_pesel_arg(pesel_arg))
			printex_pesel_error(EXIT_FAILURE, "Invalid -p argument");
		// pesel_number = pesel_from_string(pesel_arg);	
	}

	if (isoptset['d']) {
		if (check_date_arg(date_arg))
			printex_pesel_error(EXIT_FAILURE, "Invalid -d argument");
		// pesel_bdate = date_from_string(date_arg);
	}

	if (isoptset['o']) {
		if (check_ordinals_arg(ordinals_arg))
			printex_pesel_error(EXIT_FAILURE, "Invalid -o argument");
		// ordinals = atoi(ordinals_arg);
	}

	if (isoptset['s']) {
		if (check_gender_arg(gender_arg))
			printex_pesel_error(EXIT_FAILURE, "Invalid -s argument");
		// gender = gender_from_string(gender_arg);
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
				PESEL_s pesel_number = pesel_from_string(argv[pesel_arg]);

				if (validate_pesel(pesel_number) == NOERROR) {
					char* apesel = pesel_to_string(pesel_number);
					printf("Pesel %s is good\n", apesel);
					free(apesel);

					char* peseldate = date_to_string(date_from_pesel(pesel_number));
					printf("Date: %s\n", peseldate);
					free(peseldate);
					
					Gender_t gtmp = gender_from_pesel(pesel_number);
					printf("Gender: %s\n", (gtmp == FEMALE) ? "female" : "male");
				} else {
					print_pesel_error("Invalid pesel");
				}
				printf("\n");
			}

			if (isoptset['o']) {
				uint ordinals = atoi(argv[ordinals_arg]);

				if (check_ordinals(ordinals) == NOERROR) {
					printf("Ordinals %u are good\n", ordinals);
				} else {
					print_pesel_error("Invalid ordinals");
				}
				printf("\n");
			}

			if (isoptset['d']) {
				PESEL_bdate_s pesel_bdate = date_from_string(argv[date_arg]);

				if (check_bdate(pesel_bdate) == NOERROR) {
					char* adate = date_to_string(pesel_bdate);
					printf("Date %s is good\n", adate);
					free(adate);
				} else {
					print_pesel_error("Invalid pesel date");
				}
				printf("\n");
			}

			if (isoptset['s']) {
				Gender_t gender = gender_from_string(argv[gender_arg]);

				if (check_gender(gender) == NOERROR) {
					printf("Gender %s is good\n", gender_arg);
				} else {
					print_pesel_error("Invalid gender");
				}
				printf("\n");
			}
			
		} else if (isoptset['g']) { // 'if' is unnecessary here
			PESEL_bdate_s pesel_bdate = date_from_string(argv[date_arg]);
			uint ordinals;
			Gender_t gender;

			if (check_bdate(pesel_bdate) != NOERROR) {
				printex_pesel_error(EXIT_FAILURE, "Invalid pesel date");
			}
			
			if (isoptset['s']) {
				gender = gender_from_string(argv[gender_arg]);

				if (check_gender(gender) != NOERROR) {
					printex_pesel_error(EXIT_FAILURE, "Invalid gender");
				}
			}
			
			if (isoptset['o']) {
				ordinals = atoi(argv[ordinals_arg]);

				if (check_ordinals(ordinals) != NOERROR) {
					printex_pesel_error(EXIT_FAILURE, "");
				}

				if (isoptset['s'] && check_ordinals_gender(ordinals, gender) != NOERROR) {
					printex_pesel_error(EXIT_FAILURE, "Invalid ordinals for this gender");
				}
			}
			
			PESEL_data_s pdata;
			pdata.birth_date = pesel_bdate;
			if (isoptset['s'])
				pdata.gender = gender;

			if (isoptset['a']) {
				PESEL_s* all_pesels = generate_all_pesels(pdata);
				if (all_pesels == NULL) {
					printex_pesel_error(EXIT_FAILURE, "Could not generate all pesels");
				}

				for (size_t i = 0; i < ORDINALS_SIZE; i++) {
					char* apesel = pesel_to_string(all_pesels[i]);
					printf("%d: %s\n", i + 1, apesel);
					free(apesel);
				}

				free(all_pesels);
			} else if (isoptset['o']) {
				pdata.gender = gender_from_ordinals(ordinals);

				PESEL_s pn = generate_pesel_with_ordinals(pdata, ordinals);
				if (is_default_pesel_s(pn)) {
					printex_pesel_error(EXIT_FAILURE, "Could not generate pesel");
				}

				char* apesel = pesel_to_string(pn);
				printf("Pesel %s\n", apesel);
				free(apesel);
			} else {
				PESEL_s pn = generate_pesel(pdata);
				if (is_default_pesel_s(pn)) {
					printex_pesel_error(EXIT_FAILURE, "Could not generate pesel");
				}
				
				char* apesel = pesel_to_string(pn);
				printf("Pesel %s\n", apesel);
				free(apesel);
			}
		}
	}

	return EXIT_SUCCESS;
}