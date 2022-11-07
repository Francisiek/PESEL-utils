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
	check_date(year, month, day);
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

	PESEL_data_s pesel_data;
	pesel_data.birth_date.year = year;
	pesel_data.birth_date.month = month;
	pesel_data.birth_date.day = day;
	pesel_data.gender = gender;
	
	const char* PESEL_number = pesel_to_string(generate_pesel(pesel_data));

	printf("%s\n", PESEL_number);

	// checking pesel

	return EXIT_SUCCESS;
}