#ifndef PESEL_HEADER
#define PESEL_HEADER

#include <stdarg.h>

enum { PESEL_date_length = 6 };
enum { PESEL_ordinals_length = 4};
enum { PESEL_length = PESEL_date_length + PESEL_ordinals_length + 1};

typedef unsigned int uint;
typedef enum Gender { MALE = 'm', FEMALE = 'f' } Gender_t;

// Structure that holds birthday date in normal format.
typedef struct PESEL_bdate_s {
	uint year;
	uint month;
	uint day;
} PESEL_bdate_s;

// Structure that holds data that PESEL contains,
// but in normal format.
typedef struct PESEL_data_s {
	PESEL_bdate_s birth_date;
	Gender_t gender;
} PESEL_data_s;

// Structure that holds raw data from PESEL number.
typedef struct PESEL_s {
	uint year;
	uint month;
	uint day;
	uint ordinals;
	uint control;
} PESEL_s;

// Wages of first ten PESEL's digits
extern const uint const PESEL_wages[PESEL_length - 1];

// Prints error message to stderr with and exits with exit(exit code)
void printe(int exit_code, char* format, ...);

// Checks if a 'year' is leap. Returns 0-false or 1-true
int is_leap_year(uint year);

// Checks if given date is valid: do year fits in PESEL's year interval,
// if month's day suits do the month
int check_date(uint year, uint month, uint day);

// Converts normal month number to PESEL, so the one with
// aditional number indicating century
uint convert_to_pesel_month(uint year, uint month);

// Generates 6-digit PESEL date format from year, month and day.
// Returns pointer to allocated table which musts be freed
const uint* generate_pesel_date(uint year, uint month, uint day);

// Generates a random 4-digit PESEL ordinal numbers.
// Returns pointer to allocated table which musts be freed
const uint* generate_random_pesel_ordinals(Gender_t g);

// Computes PESEL's control number.
const uint pesel_control_nunber(const uint* PESEL_number);

// Generates PESEL number with given details and random ordinal nums.
const uint* generate_pesel(uint year, uint month, uint day, Gender_t gender);

// Converts given year and month from PESEL number to normal date by
// PESEL_data_t struct.
PESEL_bdate_s convert_pesel_month(uint pesel_year, uint pesel_month);

#endif // PESEL_HEADER