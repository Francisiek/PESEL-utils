#include "pesel-functions.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stddef.h>
#include <ctype.h>
#include <math.h>

int pesel_error;
void set_pesel_error(int code) {
	pesel_error = code;
}

static int (*random_generator)() = rand;
static int (*random_generator_initializer)() = srand;

enum {MONTHS = 12, MAX_YEAR = 99, DINTERVALS = 5};

const uint const PESEL_wages[PESEL_length - 1] = {1, 3, 7, 9, 1, 3, 7, 9, 1, 3};

const uint PESEL_MONTH[DINTERVALS] = {80, 0, 20, 40, 60};
const uint PESEL_YEAR[DINTERVALS] = {1800, 1900, 2000, 2100, 2200};

static const unsigned char const days_in_month[2][12] = {
	{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
	{31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}
};

PESEL_bdate_s default_PESEL_bdate_s = {};
PESEL_data_s default_PESEL_data_s = {};
PESEL_s default_PESEL_s = {};

void printe(int exit_code, char* format, ...) {
	va_list list;
	va_start(list, format);
	vfprintf(stderr, format, list);
	va_end(list);
	exit(exit_code);
}

static void init_random_generator(void) {
	return srand(time(NULL) * clock());
}

static uint random_uint(void) {
	return (uint)rand();
}

bool is_leap_year(uint year) {
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

int check_pesel_month(uint month) {
	int month_interval_num = -1;
	for (size_t i = 0; i < DINTERVALS; i++) {
		if (month > PESEL_MONTH[i] && 
				month <= PESEL_MONTH[i] + MONTHS) {
			month_interval_num = i;
			break;
		}
	}

	if (month_interval_num == -1) {
		set_pesel_error(EMONTH_RANGE);
		return EMONTH_RANGE;
	}

	return NOERROR;
}

int check_date(uint year, uint month, uint day) {
	if (year < PESEL_YEAR[0] || year > PESEL_YEAR[DINTERVALS - 1] + MAX_YEAR) {
		set_pesel_error(EYEAR_RANGE);
		return EYEAR_RANGE;
	}
	if (month < 1 || month > MONTHS) {
		set_pesel_error(EMONTH_RANGE);
		return EMONTH_RANGE;
	}
	if (day < 1 || day > 31) {
		set_pesel_error(EDAY_RANGE);
		return EDAY_RANGE;
	}

	if (day > days_in_month[is_leap_year(year)][month - 1]) {
		set_pesel_error(EMONTH_DAY);
		return EMONTH_DAY;
	}

	return NOERROR;
}

int check_bdate(PESEL_bdate_s bdate) {
	int year, month, day;
	year = bdate.year;
	month = bdate.month;
	day = bdate.day;

	return check_date(year, month, day);
}

int check_pesel_data(PESEL_data_s pdata) {
	int e;
	if (e = check_bdate(pdata.birth_date)) {
		return e;
	}

	if (e = check_gender(pdata.gender)) {
		return e;
	}

	return NOERROR;
}

int check_pesel_date(PESEL_s pesel_number) {
	if (pesel_number.year > MAX_YEAR) {
		set_pesel_error(EYEAR_RANGE);
		return EYEAR_RANGE;
	}

	int s;
	if (s = check_pesel_month(pesel_number.month)) {
		return s;
	}

	int month_interval_num = month_from_pesel(pesel_number.month);

	// normal-format year and month of birthday
	int nyear, nmonth;
	nyear = PESEL_YEAR[month_interval_num] + pesel_number.year;
	nmonth = pesel_number.month - PESEL_MONTH[month_interval_num];

	if (pesel_number.day > days_in_month[is_leap_year(nyear)][nmonth - 1] || 
			pesel_number.day < 1) {
		set_pesel_error(EMONTH_DAY);
		return EMONTH_DAY;
	}

	return NOERROR;
}

int check_gender(Gender_t gender) {
	if (gender != MALE && gender != FEMALE) {
		set_pesel_error(EGENDER);
		return EGENDER;
	} else
		return NOERROR;
}

int check_ordinals_gender(uint ordinals, Gender_t gender) {
	int e;
	if (e = check_gender(gender))
		return e;

	if ((gender == FEMALE && ordinals % 2 != FEMALE) ||
			(gender == MALE && ordinals % 2 != MALE)) { 
		set_pesel_error(EORDINALS);
		return EORDINALS;
	}

	if (e = check_ordinals(ordinals))
		return e;

	return NOERROR;
}

int check_ordinals(uint ordinals) {
	if (ordinals >= 10000) {
		set_pesel_error(EORDINALS);
		return EORDINALS;
	}

	return NOERROR;
}

int validate_pesel(PESEL_s pesel_number) {
	int e;
	if (e = check_pesel_date(pesel_number))
		return e;
	if (e = check_ordinals(pesel_number.ordinals))
		return e;

	if (pesel_number.control != pesel_control_number(pesel_number)) {
		set_pesel_error(ECONTROL);
		return ECONTROL;
	}

	return NOERROR;
}

uint month_to_pesel(uint year, uint month) {
	if (check_date(year, month, 1))
		return 0;

	for (int i = 0; i < DINTERVALS; i++) {
		if (year >= PESEL_YEAR[i] && year <= PESEL_YEAR[i] + MAX_YEAR) {
			month += PESEL_MONTH[i];
			break;
		}
	}

	return month;
}

int month_from_pesel(uint month) {
	int month_interval_num = -1;
	for (size_t i = 0; i < DINTERVALS; i++) {
		if (month > PESEL_MONTH[i] && 
				month <= PESEL_MONTH[i] + MONTHS) {
			month_interval_num = i;
			break;
		}
	}

	if (month_interval_num == -1) {
		set_pesel_error(EMONTH_RANGE);
		return EMONTH_RANGE;
	}

	return month_interval_num;
}

PESEL_s date_to_pesel(PESEL_bdate_s date) {
	if (check_bdate(date)) {
		return default_PESEL_s;
	}

	PESEL_s pesel_number;
	pesel_number.year = cut_year(date.year);
	pesel_number.month = month_to_pesel(date.year, date.month);
	pesel_number.day = date.day;

	return pesel_number;
}

PESEL_bdate_s date_from_pesel(PESEL_s pesel_number) {
	if (check_pesel_date(pesel_number))
		return default_PESEL_bdate_s;

	int month_interval_num = month_from_pesel(pesel_number.month);

	// normal-format year and month of birthday
	int nyear, nmonth;
	nyear = PESEL_YEAR[month_interval_num] + pesel_number.year;
	nmonth = pesel_number.month - PESEL_MONTH[month_interval_num];

	PESEL_bdate_s pesel_date;
	pesel_date.day = pesel_number.day;
	pesel_date.month = nmonth;
	pesel_date.year = nyear;

	return pesel_date;
}

PESEL_bdate_s make_pesel_bdate(uint year, uint month, uint day) {
	PESEL_bdate_s date;
	date.year = year;
	date.month = month;
	date.day = day;

	return date;
}

PESEL_data_s make_pesel_data(PESEL_bdate_s bdate, Gender_t gender) {
	PESEL_data_s d;
	d.birth_date = bdate;
	d.gender = gender;

	return d;
}

char* pesel_to_string(PESEL_s pesel_number) {
	char* apesel = calloc(sizeof(char), PESEL_length + 1);
	if (apesel == NULL) {
		set_pesel_error(EALLOC);
		return NULL;
	} 

	sprintf(&apesel[0], "%02u", pesel_number.year);
	sprintf(&apesel[2], "%02u", pesel_number.month);
	sprintf(&apesel[4], "%02u", pesel_number.day);
	sprintf(&apesel[6], "%04u", pesel_number.ordinals);
	sprintf(&apesel[10], "%01u", pesel_number.control);
	sprintf(&apesel[11], "\0");

	return apesel;
}

PESEL_s pesel_from_string(char* pesel_string) {
	if (pesel_string == NULL) {
		set_pesel_error(EARGUMENT);
		return default_PESEL_s;
	}

	if (strlen(pesel_string) != PESEL_length) {
		set_pesel_error(EPESEL_LENGTH);
		return default_PESEL_s;
	}

	for (size_t i = 0; i < PESEL_length; i++) {
		if (!isdigit(pesel_string[i])) {
			set_pesel_error(ENOT_DIGIT);
			return default_PESEL_s;
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
		pesel_number.day += to_digit(pesel_string[4 + i]) * power;
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
	if (check_gender(g))
		return 0;

	static const char gender_numbers[2][5] = {
		{0, 2, 4, 6, 8}, {1, 3, 5, 7, 9}
	};
	
	uint ordinals = 0;
	uint power = pow(10, 3);
	init_random_generator();

	for (int i = PESEL_ordinals_length - 1; i > 0; i--) {
		ordinals += (random_uint() % 10) * power;
		power /= 10;
	}

	size_t gindex = (g == FEMALE) ? 0 : 1;
	ordinals += gender_numbers[gindex][random_uint() % 5] * power;

	return ordinals;
}

const uint pesel_control_number(PESEL_s pesel_number) {
	char* pesel_string = pesel_to_string(pesel_number);
	if (pesel_string == NULL) {
		return 0;
	}

	int sum = 0;
	for (size_t i = 0; i < PESEL_length - 1; i++) {
		sum += (to_digit(pesel_string[i]) * PESEL_wages[i]) % 10;
	}

	return (10 - (sum % 10)) % 10;
}


PESEL_s generate_pesel(PESEL_data_s pesel_data) {
	if (check_pesel_data(pesel_data))
		return default_PESEL_s;

	PESEL_s pesel_number;
	pesel_number.year = cut_year(pesel_data.birth_date.year);
	pesel_number.month = month_to_pesel(pesel_data.birth_date.year, 
		pesel_data.birth_date.month);
	pesel_number.day = pesel_data.birth_date.day;
	pesel_number.ordinals = random_pesel_ordinals(pesel_data.gender);
	pesel_number.control = pesel_control_number(pesel_number);

	return pesel_number;
}

PESEL_s generate_pesel_with_ordinals(PESEL_data_s pesel_data, uint ordinals) {
	if (check_ordinals_gender(ordinals, pesel_data.gender))
		return default_PESEL_s;

	PESEL_s pesel_number = generate_pesel(pesel_data);
	if (!memcmp(&pesel_number, &default_PESEL_s, sizeof (PESEL_s))) {
		return default_PESEL_s;
	}

	// only ordinals changes so the control number too
	pesel_number.ordinals = ordinals;
	pesel_number.control = pesel_control_number(pesel_number);

	return pesel_number;
}

static uint ordinals_table[ORDINALS_SIZE];
static size_t ordinals_table_size = 0;

static int push_ordinals(uint number) {
	if (ordinals_table_size < ORDINALS_SIZE) {
		ordinals_table[ordinals_table_size] = number;
		ordinals_table_size++;
		return NOERROR;
	} else {
		set_pesel_error(EOVERFLOW);
		return EOVERFLOW;
	}
}

static void clear_ordinals(void) {
	ordinals_table_size = 0;
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
				rec_ordinals_even(deep + 1);
				rec_ordinal -= i * power;
			}
		} else {
			for (int i = 0; i < 10; i += 2) {
				rec_ordinal += i * power;
				rec_ordinals_even(deep + 1);
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
				rec_ordinals_uneven(deep + 1);
				rec_ordinal -= i * power;
			}
		} else {
			for (int i = 1; i < 10; i += 2) {
				rec_ordinal += i * power;
				rec_ordinals_uneven(deep + 1);
				rec_ordinal -= i * power;
			}
		}
	}
}

PESEL_s* generate_all_pesels(PESEL_data_s pesel_data) {
	if (check_pesel_data(pesel_data))
		return NULL;

	if (pesel_data.gender == FEMALE) {
		rec_ordinals_even(0);
	} else {
		rec_ordinals_uneven(0);
	}

	PESEL_s* pesels_table = calloc(ORDINALS_SIZE, sizeof(PESEL_s));
	if (pesels_table == NULL) {
		set_pesel_error(EALLOC);
		return NULL;
	}

	for (size_t i = 0; i < ORDINALS_SIZE; i++) {
		pesels_table[i] = 
			generate_pesel_with_ordinals(pesel_data, ordinals_table[i]);
	}

	clear_ordinals();
	return pesels_table;
}

int is_pesel_in_base(PESEL_s pesel_number) {

}
