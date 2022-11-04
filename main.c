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

	const uint* PESEL_number = generate_pesel(year, month, day, gender);

	for (int i = 0; i < PESEL_length; i++)
		printf("%d", PESEL_number[i]);
	printf("\n");

	// checking pesel

	return EXIT_SUCCESS;
}