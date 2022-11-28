#include <unistd.h>
#include <getopt.h>

extern struct option long_options[];
extern const char* optstring;

#define optset_size 128
extern bool isoptset[optset_size];
extern size_t pesel_arg;
extern size_t date_arg;
extern size_t ordinals_arg;
extern size_t gender_arg;

// find if options other than in optstr are set in 'isoptset'
int areotheropt(const char* optstr);

int check_pesel_arg(const char* peselstr);
int check_date_arg(const char* datestr);
int check_ordinals_arg(const char* ordinalsstr);
int check_gender_arg(const char* gender);
