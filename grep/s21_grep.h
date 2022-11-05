#ifndef SRC_GREP_S21_GREP_H_
#define SRC_GREP_S21_GREP_H_

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <pcre.h>
#include <sys/stat.h>
#include <string.h>
#include <getopt.h>

//  Флаги
bool e = false;
bool i = false;
bool v = false;
bool c = false;
bool l = false;
bool n = false;
bool h = false;
bool s = false;
bool f = false;
bool o = false;

// Массив паттернов
static char regex_val[1024];

static struct stat sb;
static int file_counter = 0;
static char *this_file_name = NULL;

int parse_option(int argc, char **argv);
pcre *parse_pattern(char **argv);
int get_files_amount(char **argv);
int parse_file(char **argv, FILE **fp);
int output(pcre *re, FILE **fp);

#endif  //  SRC_GREP_S21_GREP_H_
