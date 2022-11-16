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

	PESEL_bdate_s bd = make_pesel_bdate(year, month, day);
	pesel2 = date_to_pesel(bd);
	pesel2.ordinals = random_pesel_ordinals(gender);
	pesel2.control = pesel_control_number(pesel2);
	printf("pesel2 second validation %d\n", validate_pesel(pesel2));
	printf("pesel2 %s\n", pesel_to_string(pesel2));

	PESEL_s* ptable = generate_all_pesels(pdata);
	if (ptable == NULL)
		printe(EXIT_FAILURE, "error generating all pesels\n");

	for (size_t i = 0; i < ORDINALS_SIZE; i++) {
		printf("%s val %d\n", pesel_to_string(ptable[i]), validate_pesel(ptable[i]));
	}
	return EXIT_SUCCESS;
}