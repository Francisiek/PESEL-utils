#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "pesel-functions.h"

// For now it only generates pesels. Later it will, check them
// and generate new with synchronization
int main(void) {
	uint year, month, day;
	Gender_t gender;

	// generating PESEL
	printf("Give year, month, and day of birth: ");
	scanf("%u %u %u", &year, &month, &day);
	if (check_date(year, month, day))
		printe(EXIT_FAILURE, "Wrong date\n");
	// birth date validated
	
	char tmp_gender_char;
	printf("Give gender M/F: ");
	scanf(" %c", &tmp_gender_char);
	if (toupper(tmp_gender_char) == 'F')
		gender = FEMALE;
	else if (toupper(tmp_gender_char) == 'M')
		gender = MALE;
	else 
		printe(EXIT_FAILURE, "Gender can by only F - female or M - male\n");
	// gender validated

	PESEL_data_s pdata = make_pesel_data(make_pesel_bdate(year, month, day), gender);
	PESEL_s pesel_num = generate_pesel(pdata);
	char* s = pesel_to_string(pesel_num);
	printf("%s\n", s);
	PESEL_s pesel2 = pesel_from_string(s);
	printf("pesel from string %s\n", pesel_to_string(pesel2));
	printf("pesel validate %d\n", validate_pesel(pesel2));
	printf("pesel errno %d\n", pesel_error);
	return EXIT_SUCCESS;
}