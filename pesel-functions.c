#include "pesel-functions.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stddef.h>
#include <ctype.h>

const uint const PESEL_wages[PESEL_length - 1] = {1, 3, 7, 9, 1, 3, 7, 9, 1, 3};
static int (*random_generator)() = rand;
static int (*random_generator_initializer)() = srand;

enum {MONTHS = 12, MAX_YEAR = 99};

const uint PESEL_MONTH[] = {80, 0, 20, 40, 60};
const uint PESEL_YEAR[] = {1800, 1900, 2000, 2100, 2200};

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

uint cut_year(uint year) {
	return year % 100;
}

uint to_digit(char c) {
	return c - '0';
}

int check_date(uint year, uint month, uint day) {
	if (year < PESEL_YEAR[0] || year > PESEL_YEAR[4] + MAX_YEAR)
		printe(EXIT_FAILURE, "Year can be only from 1800 to 2299\n");
	if (month < 1 || month > MONTHS)
		printe(EXIT_FAILURE, "Month can be only from 1 to 12\n");
	if (day < 1 || day > 31)
		printe(EXIT_FAILURE, "Day can be only from 1 to 31\n");
	
	if (day > days_in_month[is_leap_year(year)][month - 1])
		printe(EXIT_FAILURE, "Day %u is not right for %u month. It should be max %u day\n",
			day, month, days_in_month[is_leap_year(year)][month - 1]);

	return 0;
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

	if (pesel_number.day > days_in_month[is_leap_year(nyear)][nmonth - 1] || 
			pesel_number.day < 1) {
		printe(EXIT_FAILURE, "Day %u is not right for %u month. It should be max %u day\n",
			pesel_number.day, nmonth, days_in_month[is_leap_year(nyear)][nmonth - 1]);
		return 1;
	}

	return 0;
}

int check_gender(Gender_t gender) {
	if (gender != MALE && gender != FEMALE)
		printe(EXIT_FAILURE, "Sorry... gender can be only male or female\n");
	else
		return 0;
}

uint month_to_pesel(uint year, uint month) {
	for (int i = 0; i < 5; i++) {
		if (year >= PESEL_YEAR[0] && year < PESEL_YEAR[0] + 100) {
			month += PESEL_MONTH[0];
			break;
		}
	}

	return month;
}

size_t month_from_pesel(uint month) {
	size_t month_interval_num = -1;
	for (size_t i = 0; i < 5; i++) {
		if (month > PESEL_MONTH[i] && 
				month <= PESEL_MONTH[i] + MONTHS) {
			month_interval_num = i;
			break;
		}
	}

	return month_interval_num;
}

PESEL_s date_to_pesel(PESEL_bdate_s date) {
	if (check_date(date.year, date.month, date.day)) {
		printe(EXIT_FAILURE, "Invalid date\n");
	}

	PESEL_s pesel_number;
	pesel_number.year = cut_year(date.year);
	pesel_number.month = month_to_pesel(date.year, date.month);
	pesel_number.day = date.day;

	return pesel_number;
}

PESEL_bdate_s date_from_pesel(PESEL_s pesel_number) {
	PESEL_bdate_s pesel_date;
	int nyear, nmonth;
	if (pesel_number.year > MAX_YEAR)
		printe(EXIT_FAILURE, 
			"Invalid PESEL year %u, can be up to 99\n", pesel_number.year);

	int fit_months_counter = 0;
	for (int i = 0; i < 5; i++) {
		if (pesel_number.month > PESEL_MONTH[i] && 
				pesel_number.month <= PESEL_MONTH[i] + MONTHS) {
			fit_months_counter++;
			break;
		}
	}

	if (fit_months_counter == 0) {
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

PESEL_bdate_s make_pesel_bdate(uint year, uint month, uint day) {
	PESEL_bdate_s date;
	date.year = year;
	date.month = month;
	date.day = day;

	return date;
}

char* pesel_to_string(PESEL_s pesel_number) {
	char* apesel = calloc(sizeof(char), PESEL_length + 1);
	sprintf(&apesel[0], "%02u", pesel_number.year);
	sprintf(&apesel[2], "%02u", pesel_number.month);
	sprintf(&apesel[4], "%02u", pesel_number.day);
	sprintf(&apesel[6], "%04u", pesel_number.ordinals);
	sprintf(&apesel[10], "%01u", pesel_number.control);
	sprintf(&apesel[11], "\0");

	return apesel;
}

PESEL_s pesel_from_string(char* pesel_string) {
	if (strlen(pesel_string) != PESEL_length) {
		printe(EXIT_FAILURE, "Bad PESEL number length\n");
	}

	for (size_t i = 0; i < PESEL_length; i++) {
		if (!isdigit(pesel_string[i])) {
			printe(EXIT_FAILURE, "Not a digit in PESEL number\n");
		}
	}

	PESEL_s pesel_number = {};
	int power = 10;
	// year
	for (size_t i = 0; i < 2; i++) {
		pesel_number.year += to_digit(pesel_string[i]) * power;
		power /= 10;
	}
	// month
	power = 10;
	for (size_t i = 0; i < 2; i++) {
		pesel_number.month += to_digit(pesel_string[2 + i]) * power;
		power /= 10;
	}
	// day
	power = 10;
	for (size_t i = 0; i < 2; i++) {
		pesel_number.month += to_digit(pesel_string[4 + i]) * power;
		power /= 10;
	}
	// ordinals
	power = 1000;
	for (size_t i = 0; i < 4; i++) {
		pesel_number.ordinals += to_digit(pesel_string[6 + i]) * power;
		power /= 10;
	}
	// control number
	pesel_number.control = to_digit(pesel_string[PESEL_length - 1]);

	return pesel_number;
}

const uint random_pesel_ordinals(Gender_t g) {
	static const char gender_numbers[2][5] = {
		{1, 3, 5, 7, 9}, {0, 2, 4, 6, 8}
	};
	size_t gindex = (g == MALE) ? 0 : 1;

	uint ordinals = 0;
	uint power = 1;
	init_random_generator();

	for (int i = 0; i < PESEL_ordinals_length - 1; i++) {
		ordinals += (random_uint() % 10) * power;
		power *= 10;
	}
	ordinals += gender_numbers[gindex][random_uint() % 5] * power;

	return ordinals;
}

const uint pesel_control_nunber(PESEL_s pesel_number) {
	char* pesel_string = pesel_to_string(pesel_number);
	int sum = 0;

	for (size_t i = 0; i < PESEL_length - 1; i++) {
		sum += (to_digit(pesel_string[i]) * PESEL_wages[i]) % 10;
	}

	return 10 - (sum % 10);
}

int check_ordinals(uint ordinals, Gender_t gender) {
	check_gender(gender);
	if (gender == FEMALE && ordinals % 2 != 0)
		return 1;
	else if (gender == MALE && ordinals % 2 != 1)
		return 1;
	int digit_counter = 0;
	while (ordinals > 0) {
		digit_counter++;
		ordinals /= 10;
	}

	if (digit_counter != PESEL_ordinals_length) {
		return 1;
	}

	return 0;
}

PESEL_s generate_pesel(PESEL_data_s pesel_data) {
	check_date(pesel_data.birth_date.year, 
		pesel_data.birth_date.month, pesel_data.birth_date.day);
	check_gender(pesel_data.gender);

	PESEL_s pesel_number;
	pesel_number.year = cut_year(pesel_data.birth_date.year);
	pesel_number.month = month_to_pesel(pesel_data.birth_date.year, 
		pesel_data.birth_date.month);
	pesel_number.day = pesel_data.birth_date.day;
	pesel_number.ordinals = random_pesel_ordinals(pesel_data.gender);
	pesel_number.control = pesel_control_nunber(pesel_number);

	return pesel_number;
}

PESEL_s generate_pesel_with_ordinals(PESEL_data_s pesel_data, uint ordinals) {
	check_date(pesel_data.birth_date.year, 
		pesel_data.birth_date.month, pesel_data.birth_date.day);
	check_gender(pesel_data.gender);
	if (check_ordinals(ordinals, pesel_data.gender))
		printe(EXIT_FAILURE, "Bad oridinals\n");

	PESEL_s pesel_number;
	pesel_number.year = cut_year(pesel_data.birth_date.year);
	pesel_number.month = month_to_pesel(pesel_data.birth_date.year, 
		pesel_data.birth_date.month);
	pesel_number.day = pesel_data.birth_date.day;
	pesel_number.ordinals = ordinals;
	pesel_number.control = pesel_control_nunber(pesel_number);

	return pesel_number;
}

int validate_pesel(PESEL_s pesel_number) {
	check_pesel_date(pesel_number);
	if (pesel_number.control != pesel_control_nunber(pesel_number)) {
		printe(EXIT_FAILURE, "Pesel control number isn't good\n");
	}

	return 0;
}

enum {ORDINALS_SIZE = 5000};
static uint ordinals_table[ORDINALS_SIZE];
static size_t ordinals_table_size = 0;

static int push_ordinals(uint number) {
	if (ordinals_table_size < ORDINALS_SIZE) {
		ordinals_table[ordinals_table_size - 1] = number;
		ordinals_table_size++;
		return 0;
	} else
		return 1;
}

static int clear_ordinals(void) {
	ordinals_table_size = 0;
	return 0;
}

static uint rec_ordinal = 0;
static void rec_ordinals_even(uint deep) {
	if (deep > 3) {
		push_ordinals(rec_ordinal);
		return;
	} else {
		int power = pow(10, 3 - deep);
		if (deep < 3) {
			for (int i = 0; i < 10; i++) {
				rec_ordinal += i * power;
				rec_ordinals_gen(deep + 1);
				rec_ordinal -= i * power;
			}
		} else {
			for (int i = 0; i < 10; i += 2) {
				rec_ordinal += i * power;
				rec_ordinals_gen(deep + 1);
				rec_ordinal -= i * power;
			}
		}
	}
}

static void rec_ordinals_uneven(uint deep) {
	if (deep > 3) {
		push_ordinals(rec_ordinal);
		return;
	} else {
		int power = pow(10, 3 - deep);
		if (deep < 3) {
			for (int i = 0; i < 10; i++) {
				rec_ordinal += i * power;
				rec_ordinals_gen(deep + 1);
				rec_ordinal -= i * power;
			}
		} else {
			for (int i = 1; i < 10; i += 2) {
				rec_ordinal += i * power;
				rec_ordinals_gen(deep + 1);
				rec_ordinal -= i * power;
			}
		}
	}
}

PESEL_s* generate_all_pesels(PESEL_data_s pesel_data) {
	check_gender(pesel_data.gender);
	if (pesel_data.gender == FEMALE) {
		rec_ordinals_even(0);
	} else {
		rec_ordinals_uneven(0);
	}

	PESEL_s* pesels_table = calloc(sizeof(PESEL_s), ORDINALS_SIZE);
	for (size_t i = 0; i < ORDINALS_SIZE; i++) {
		pesels_table[i] = 
			generate_pesel_with_ordinals(pesel_data, ordinals_table[i]);
	}
	clear_ordinals();

	return pesels_table;
}

int is_pesel_in_base(PESEL_s pesel_number) {

}
