#include "pesel-functions.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>


const uint const PESEL_wages[PESEL_length - 1] = {1, 3, 7, 9, 1, 3, 7, 9, 1, 3};
static int (*random_generator)() = rand;
static int (*random_generator_initializer)() = srand;

enum {MONTHS = 12, MAX_YEAR = 99};

static const uint PESEL_MONTH[] = {80, 0, 20, 40, 60};
static const uint PESEL_YEAR[] = {1800, 1900, 2000, 2100, 2200};

static const unsigned char const days_in_month[2][12] = {
		{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
		{31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
	};

void printe(int exit_code, char* format, ...) {
	va_list list;
	va_start(list, format);
	vfprintf(stderr, format, list);
	va_end(list);
	exit(exit_code);
}

void init_random_generator(void) {
	return srand(time(NULL) * clock());
}

uint random_uint(void) {
	return (uint)rand();
}

int is_leap_year(uint year) {
	if (year % 4 != 0)
		return false;
	else if (year % 100 != 0)
		return true;
	else if (year % 400 != 0)
		return false;
	else
		return true;
}

int check_date(uint year, uint month, uint day) {
	if (year < 1800 || year > 2299)
		printe(EXIT_FAILURE, "Year can be only from 1800 to 2299\n");
	if (month < 1 || month > 12)
		printe(EXIT_FAILURE, "Month can be only from 1 to 12\n");
	if (day < 1 || day > 31)
		printe(EXIT_FAILURE, "Day can be only from 1 to 31\n");
	
	if (day > days_in_month[is_leap_year(year)][month - 1])
		printe(EXIT_FAILURE, "Day %u is not right for %u month. It should be max %u day\n",
			day, month, days_in_month[is_leap_year(year)][month - 1]);

	return 0;
}

int check_gender(Gender_t gender) {
	if (gender != MALE && gender != FEMALE)
		printe(EXIT_FAILURE, "Sorry... gender can be only male or female\n");
	else
		return 0;
}

uint convert_to_pesel_month(uint year, uint month) {
	for (int i = 0; i < 5; i++) {
		if (year >= PESEL_YEAR[0] && year < PESEL_YEAR[0] + 100) {
			month += PESEL_MONTH[0];
			break;
		}
	}

	return month;
}

PESEL_bdate_s make_pesel_bdate(uint year, uint month, uint day) {
	PESEL_bdate_s date;
	date.year = year;
	date.month = month;
	date.day = day;

	return date;
}

char* pesel_to_char(PESEL_s pesel_number) {
	char* adate = calloc(sizeof(char), PESEL_date_length + 1);
	sprintf(&adate[0], "%u", birhtday.year);
}

const uint* generate_pesel_date(uint year, uint month, uint day) {
	uint* PESEL_date = calloc(sizeof(uint), PESEL_date_length);
	month = convert_to_pesel_month(year, month);
	PESEL_date[0] = (year / 10) % 10;
	PESEL_date[1] = year % 10;
	PESEL_date[2] = (month / 10) % 10;
	PESEL_date[3] = month % 10;
	PESEL_date[4] = (day / 10) % 10;
	PESEL_date[5] = day % 10;

	return PESEL_date; 
}


const uint* generate_random_pesel_ordinals(Gender_t g) {
	static const char gender_numbers[2][5] = {
		{1, 3, 5, 7, 9}, {0, 2, 4, 6, 8}
	};
	size_t gindex = (g == MALE) ? 0 : 1;

	uint* const ordinals = calloc(sizeof(uint), PESEL_ordinals_length);
	init_random_generator();
	for (int i = 0; i < PESEL_ordinals_length - 1; i++)
		ordinals[i] = random_uint() % 10;
	ordinals[PESEL_ordinals_length - 1] = gender_numbers[gindex][random_uint() % 5];

	return ordinals;
}

const uint pesel_control_nunber(const uint* PESEL_number) {
	int sum = 0;
	for (int i = 0; i < 10; i++) {
		sum += (PESEL_number[i] * PESEL_wages[i]) % 10;
	}
	return 10 - (sum % 10);
}

const uint* generate_pesel(uint year, uint month, uint day, Gender_t gender) {
	check_date(year, month, day);
	check_gender(gender);

	uint* const PESEL_number = calloc(sizeof(uint), PESEL_length);
	const uint* PESEL_date;
	const uint* PESEL_ordinals;

	// Generates full PESEL format date from normal one
	PESEL_date = generate_pesel_date(year, month, day);
	// copy date into pesel number
	memcpy(PESEL_number, PESEL_date, sizeof(uint) * PESEL_date_length);
	
	PESEL_ordinals = generate_random_pesel_ordinals(gender);
	memcpy(&PESEL_number[PESEL_date_length], 
		PESEL_ordinals, sizeof(uint) * PESEL_ordinals_length);

	PESEL_number[PESEL_length - 1] = pesel_control_nunber(PESEL_number);

	return PESEL_number;
}

int check_pesel_date(PESEL_s pesel_number) {
	if (pesel_number.year > MAX_YEAR) {
		printe(EXIT_FAILURE, 
			"Invalid PESEL year %u, can be up to 99\n", pesel_number.year);
		return 1;
	}

	int month_interval_num = -1;
	for (int i = 0; i < 5; i++) {
		if (pesel_number.month > PESEL_MONTH[i] && 
				pesel_number.month <= PESEL_MONTH[i] + MONTHS) {
			month_interval_num = i;
			break;
		}
	}

	if (month_interval_num == -1) {
		printe(EXIT_FAILURE, "Invalid PESEL month - %u\n", pesel_number.month);
		return 1;
	}

	// normal-format year and month of birthday
	int nyear, nmonth;
	nyear = PESEL_YEAR[month_interval_num] + pesel_number.year;
	nmonth = pesel_number.month - PESEL_MONTH[month_interval_num];

	if (pesel_number.day > days_in_month[is_leap_year(nyear)][nmonth - 1] || pesel_number.day < 1) {
		printe(EXIT_FAILURE, "Day %u is not right for %u month. It should be max %u day\n",
			pesel_number.day, nmonth, days_in_month[is_leap_year(nyear)][nmonth - 1]);
		return 1;
	}

	return 0;
}

PESEL_bdate_s date_from_pesel(PESEL_s pesel_number) {
	PESEL_bdate_s pesel_date;
	int nyear, nmonth;
	if (pesel_number.year > 99)
		printe(EXIT_FAILURE, 
			"Invalid PESEL year %u, can be up to 99\n", pesel_number.year);

	int fit_months_counter = 0;
	for (int i = 0; i < 5; i++) {
		if (pesel_number.month > PESEL_MONTH[i] && 
			pesel_number.month <= PESEL_MONTH[i] + MONTHS)
		fit_months_counter++;
	}

	if (fit_months_counter == -1) {
		printe(EXIT_FAILURE, "Invalid PESEL month - %u\n", pesel_number.month);
	}
	
	pesel_date.year = nyear;
	pesel_date.month = nmonth;

	if (days_in_month[is_leap_year(nyear)][nmonth] < pesel_number.day || 
		pesel_number.day < 1) {
		printe(EXIT_FAILURE, "Invalid PESEL day - %u it can be up to%u\n", 
			pesel_number.day, days_in_month[is_leap_year(nyear)][nmonth]);
	} else
		pesel_date.day = pesel_number.day; 

	return pesel_date;
}

PESEL_bdate_s convert_pesel_month(uint pesel_year, uint pesel_month) {
	if (pesel_month > 92)
		printe(EXIT_FAILURE, 
			"Invalid PESEL month - %u, can be up to 92\n", pesel_month);
	if (pesel_year > 99)
		printe(EXIT_FAILURE, 
			"Invalid PESEL year %u, can be up to 99\n", pesel_year);
	
	PESEL_bdate_s pesel_month_date;

	if (pesel_month < 13) {
		pesel_month_date.year = 1900 + pesel_year;
		pesel_month_date.month = pesel_month - 0;
	} else if (pesel_month > 20 && pesel_month < 33) {
		pesel_month_date.year = 2000 + pesel_year;
		pesel_month_date.month = pesel_month - 20;
	} else if (pesel_month > 40 && pesel_month < 53) {
		pesel_month_date.year = 2100 + pesel_year;
		pesel_month_date.month = pesel_month - 40;
	} else if (pesel_month > 60 && pesel_month < 73) {
		pesel_month_date.year = 2200 + pesel_year;
		pesel_month_date.month = pesel_month - 60;
	} else if (pesel_month > 80 && pesel_month < 93) {
		pesel_month_date.year = 1800 + pesel_year;
		pesel_month_date.month = pesel_month - 80;
	} else {
		printe(EXIT_FAILURE, "Invalid PESEL month - %u\n", pesel_month);
	}

	return pesel_month_date;
}

// scalić te dwie funkcje wyżej
