#ifndef PESEL_HEADER
#define PESEL_HEADER

#include <stdarg.h>
#include <stddef.h>
#include <stdbool.h>

enum PESEL_ERRORS {
	// NOERROR should always be set to zero
	NOERROR = 0,
	EALLOC = 5,
	EYEAR_RANGE,
	EMONTH_RANGE,
	EDAY_RANGE,
	EMONTH_DAY,
	EGENDER,
	EORDINALS,
	ECONTROL,
	EPESEL_LENGTH,
	EVAL_PESEL,
	ENOT_DIGIT,
	EARGUMENT,
};

int _pesel_error_value(void);
#define pesel_error _pesel_error_value()
extern char* pesel_error_msg(int code);

enum { PESEL_date_length = 6 };
enum { PESEL_ordinals_length = 4};
enum { PESEL_length = PESEL_date_length + PESEL_ordinals_length + 1};
enum { ORDINALS_SIZE = 5000};

typedef unsigned int uint;
// DO NOT CHANGE. Those values indicates
// result of operation (gender_digit(last ordinals digit) mod 2)
typedef enum Gender_t { FEMALE = 0, MALE = 1 } Gender_t;

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
void printex(int exit_code, char* format, ...);

int printe(char* format, ...);

char* pesel_error_msg(int code);

int print_pesel_error(char* head);

void printex_pesel_error(int exit_code, char* head);

bool is_default_pesel_s(PESEL_s pesel_number);

int is_default_pesel_data_s(PESEL_data_s pesel_data);

int is_default_pesel_bdate_s(PESEL_bdate_s pesel_date);

// Checks if a 'year' is leap. Returns 0-false or 1-true
bool is_leap_year(uint year);

uint cut_year(uint year);

int to_digit(char c);

char* itos(int n);

size_t digits_equity(int number);

int check_pesel_month(uint month);

// Checks if given date is valid: do year fits in PESEL's year interval,
// if month's day suits do the month
int check_date(uint year, uint month, uint day);

int check_bdate(PESEL_bdate_s bdate);

// Checks date in PESEL number.
int check_pesel_date(PESEL_s pesel_number);

// Checks if 'gender' has valid value.
int check_gender(Gender_t gender);

// Checks if ordinals are 4 digit number and if they match the gender.
int check_ordinals_gender(uint ordinals, Gender_t gender);

// Checks if ordinals are 4 digit number.
int check_ordinals(uint ordinals);

int check_pesel_data(PESEL_data_s pesel_data);

int gender_from_string(const char* genderstr);

Gender_t gender_from_pesel(PESEL_s pesel_number);

Gender_t gender_from_ordinals(uint ordinals);

int validate_pesel(PESEL_s pesel_number);

// Converts normal month number to PESEL format, so the one with
// aditional number indicating century
uint month_to_pesel(uint year, uint month);

// Returns number of years interval to which 'month' belongs or -1 on failure.
int month_from_pesel(uint month);

// Fills PESEL_s type with date in PESEL format.
PESEL_s date_to_pesel(PESEL_bdate_s date);

// Gets normal format date from 'pesel_number' and
// returns it with PESEL_bdate_s type.
PESEL_bdate_s date_from_pesel(PESEL_s pesel_number);

// Fills PESEL_bdate_s structure with given date.
PESEL_bdate_s make_pesel_bdate(uint year, uint month, uint day);

// Returns 'pesel_number' as null-terminated string.
char* pesel_to_string(PESEL_s pesel_number);

// Converts 'pesel_string' to PESEL_s structure.
PESEL_s pesel_from_string(char* pesel_string);

PESEL_bdate_s date_from_string(const char* datestr);

char* date_to_string(PESEL_bdate_s date);

// Generates a random 4-digit PESEL ordinal numbers.
const uint random_pesel_ordinals(Gender_t g);

int ordinals_from_string(const char* ordinalsstr);

char* ordinals_to_string(uint ordinals);

// Computes PESEL's control number from PESEL_s structure.
const uint pesel_control_number(PESEL_s pesel_number);

// Generates PESEL number with given details and random ordinal nums.
PESEL_s generate_pesel(PESEL_data_s pesel_data);

// Generates PESEL number with given details and given ordinal numbers.
PESEL_s generate_pesel_with_ordinals(PESEL_data_s pesel_data, uint ordinals);

// Generates all possible PESELs for given data and returns
// them as table, which must be later unallocated.
PESEL_s* generate_all_pesels(PESEL_data_s pesel_data);

#endif // PESEL_HEADER