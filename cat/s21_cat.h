#ifndef SRC_CAT_S21_CAT_H_
#define SRC_CAT_S21_CAT_H_

#include <stdio.h>
#include <getopt.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

int parse_option(int argc, char **argv);

int parse_file(char ***argv, FILE **f);

int print_out(FILE **f);

#endif  // SRC_CAT_S21_CAT_H_

