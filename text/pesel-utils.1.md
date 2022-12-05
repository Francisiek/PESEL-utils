% pesel-utils(1) 1.0
% Francisiek (https://github.com/Francisiek) frx30342@protonmail.com
% December 2022

# NAME
pesel-utils - a program for checking and generating PESEL numbers.

# SYNOPSIS
**pesel-utils** [*-c* or *-g*][*options*]

# OPTIONS
**-c**, **--check** [-[*pods*]]
: checks given arguments.

**-g**, **--generate** [-[*dso*][-*a*]]
: generates PESEL number based on given values.

**NOTE** -c and -g can't be used together. 

**-d**, **--date** [*YYYY,M[M],D[D]*]
: loads date into program.

**-s**, **--gender** [[*f/m/female/male*]]
: loads gender into program.

**-o**, **--ordinals** [*NNNN*]
: loads 4-digit number called 'ordinals' into program.

**-p**, **--pesel** [*NNNNNNNNNNN*]
: loads 11-digit PESEL number into program.

**-a**, **--all**
: used with option -g tells it to generate all possible PESEL numbers.

# DESCRIPTION
PESEL is a Polish citizien's ID number regulated by law. It is widely used in Poland.

**pesel-utils** program has two modes: checking and generating.

It can check if a date (*-d*) or ordinals number (*-o*) fits pesel standard,
if a gender (*-s*) is one of *"f"*, *"female"*, *"m"*, *"male"* 
(letter case doesn't matter) and if PESEL number (*-p*) is valid. 
The program can check every argument type at once, but every type must 
appear only once. So it can't check two dates at one invocation, 
for example. When checking PESEL program prints out it's details:
born date and gender.

The generating mode takes at least two arguments needed to make a 
PESEL number, so obligatory date (*-d*) and at least one of gender (*-s*)
or ordinals (*-s*). If the program is given gender and ordinals they
must suit to each other. It can olso generate all possible PESELs for
given data with all (*-a*) option which can't be set when ordinals (*-o*)
are given.

# EXAMPLES
**pesel-utils -g -d 1997,4,22 -s f**
: Generates PESEL for female born on 22 April 1997.

**pesel-utils -g -s MALE -d 2000,02,01**
: Generates PESEL for male born on 1st February 2000.

**pesel-utils --generate --gender MALE --date 2000,02,01**
: The same as previous.

**pesel-utils -c -p 97042226664**
: Checks given PESEL.

**pesel-utils --check -d 1888,09,12 -p 00220193470**
: Checks firstly date and then PESEL.

**pesel-utils -c -o 9999**
: Checks ordinals number.

# EXIT VALUES
**0**
: successfull termination

# REPORTING BUGS
I kindly request to report any bugs to me via frx30342@protonmail.com.

# KNOW BUGS
None :) !

# VERSION
1.0

# COPYRIGHT
The MIT License \
Copyright (c) 2022 Francisiek
