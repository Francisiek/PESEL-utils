#include <unistd.h>
#include <getopt.h>

extern struct option long_options[];
extern const char* optstring;

#define optset_size 128
extern bool isoptset[optset_size];
extern char* pesel_arg;
extern char* date_arg;
extern char* ordinals_arg;
extern char* gender_arg;

// find if options other than in optstr are set in 'isoptset'
int areotheropt(const char* optstr);