#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>

#include "pesel-options.h"
#include "pesel-functions.h"

bool isoptset[optset_size];
char* pesel_arg;
char* date_arg;
char* ordinals_arg;
char* gender_arg;

struct option long_options[] = {
	{"check", no_argument, NULL, 'c'},
	{"generate", no_argument, NULL, 'g'},
	{"pesel", required_argument, NULL, 'p'},
	{"date", required_argument, NULL, 'd'},
	{"ordinals", required_argument, NULL, 'o'},
	{"gender", required_argument, NULL, 's'},
	{"all", no_argument, NULL, 'a'},
	{NULL, 0, NULL, 0}
};

// date format YYYY,MM,DD

const char* optstring = "-:cgp:d:o:s:a";

// find if options other than in optstr are set in 'isoptset'
int areotheropt(const char* optstr) {
	size_t optstr_len = strlen(optstr);

	for (size_t i = 0; i < optset_size; i++) {
		if (isoptset[i]) {
			bool one_of_opt = false;
			for (size_t j = 0; j < optstr_len; j++) {
				if (i == optstr[j]) {
					one_of_opt = true;
					break;
				}	
			}
			if (one_of_opt == false)
				return i;
		}
	}

	return 0;
}

int check_pesel_arg(const char* peselstr) {
	PESEL_s pesel_number = pesel_from_string(peselstr);
	if (is_default_pesel_s(pesel_number))
		return 1;
}

int check_date_arg(const char* datestr) {
	PESEL_bdate_s date = date_from_string(datestr);
	if (is_default_pesel_bdate_s(date)) {
		return 1;
	}
}

int check_ordinals_arg(const char* ordinalsstr) {
	char* invalid_ptr = NULL;
	uint ordinals = strtol(ordinalsstr, &invalid_ptr, 10);
	if (invalid_ptr != NULL)
		return 1;
}

int check_gender_arg(const char* gender) {
	if (!(tolower(*gender) == 'f' || tolower(*gender) == 'm'))
		return 1;
	else
		return 0;
}
