#include "s21_cat.h"

//  Флаги
bool number = false;
bool number_nonblank = false;
bool squeeze_blank = false;
bool show_ends = false;
bool show_nonprinting = false;
bool show_tabs = false;

//  Массив структур опций для opt_long
struct option const long_options[] = {
        {"number-nonblank",  no_argument, NULL, 'b'},
        {"number",           no_argument, NULL, 'n'},
        {"squeeze-blank",    no_argument, NULL, 's'},
        {"show-nonprinting", no_argument, NULL, 'v'},
        {"show-ends",        no_argument, NULL, 'E'},
        {"show-tabs",        no_argument, NULL, 'T'},
        {NULL, 0,                         NULL, 0},
};

static struct stat sb;

static int file_counter = 0;

static int line_counter = 1;


int parse_option(const int argc, char **argv) {
    char *available_options = "benstuvET";
    int opt_val;

    while ((opt_val = getopt_long(argc, argv, available_options, long_options, NULL)) != EOF) {
        switch (opt_val) {
            case 'b':
                number = true;
                number_nonblank = true;
                break;
            case 'e':
                show_ends = true;
                show_nonprinting = true;
                break;
            case 'n':
                number = true;
                break;
            case 's':
                squeeze_blank = true;
                break;
            case 't':
                show_tabs = true;
                show_nonprinting = true;
                break;
            case 'E':
                show_ends = true;
                break;
            case 'T':
                show_tabs = true;
                break;
            default:
                perror("Usage: -benstv");
                exit(EXIT_FAILURE);
        }
    }
    return EXIT_SUCCESS;
}


int parse_file(char ***argv, FILE **f) {
    (*argv)++;
    while (**argv) {
        if (***argv != '-') {
            if (stat(**argv, &sb)) {
                perror("No such file or directory");
                return (EXIT_FAILURE);
            } else {
                *f = fopen(**argv, "r");
                file_counter++;
                break;
            }
        }
        (*argv)++;
    }
    return EXIT_SUCCESS;
}

int print_out(FILE **f) {
    int ch_print;
    int ch_prev = 777;
    int ch_buff;

    if (*f) {
        if (file_counter > 1) printf("\n");
        if (number && !number_nonblank) printf("%6d\t", line_counter++);

        while ((ch_print = getc(*f)) != EOF) {
            if (squeeze_blank && ch_print == '\n' && (ch_prev == '\n' || ch_prev == 777)) {
                while ((ch_print = getc(*f)) == '\n') {
//                    ch_prev = ch_print;
                }
//                if (number_nonblank && (ch_prev == '\n') &&
//                    ch_print != EOF) {
//                    printf("%6s\t", "");
//                } if use MAC
                if (show_ends) printf("$");
                if (ch_print != EOF) printf("\n");
                ch_prev = '\n';
                if (number && !number_nonblank && ch_print != EOF) printf("%6d\t", line_counter++);
            }

            if (number_nonblank && (ch_prev == '\n' || ch_prev == 777) &&
                ch_print != '\n') {
                ch_buff = getc(*f);
                if (ch_buff != EOF) printf("%6d\t", line_counter++);
                ungetc(ch_buff, *f);
            }
//            } else if (number_nonblank && (ch_prev == '\n' || !ch_prev) &&
//                       ch_print == '\n') {
//                printf("%6s\t", "");
//            } if use mac

            if (show_tabs && ch_print == '\t') {
                do {
                    printf("^I");
                } while ((ch_print = getc(*f)) == '\t');
            }

            if (show_ends && ch_print == '\n') printf("$");

            if (ch_print != EOF) {
                if (((ch_print >= 0 && ch_print < 32) || ch_print == 127) && show_nonprinting &&
                    ch_print != '\n' && ch_print != '\t') {
                    if (ch_print == 127)
                        printf("^?");
                    else
                        printf("^%c", (ch_print + 64));
                } else {
                    printf("%c", ch_print);
                }
            }

            if (number && !number_nonblank && ch_print == '\n') {
                ch_buff = getc(*f);
                if (ch_buff != EOF) printf("%6d\t", line_counter++);
                ungetc(ch_buff, *f);
            }

            ch_prev = ch_print;
        }

        if (*f) {
            fclose(*f);
            *f = NULL;
        }
    }
    return EXIT_SUCCESS;
}

int main(int argc, char **argv) {
    FILE *f = NULL;
    if (argc > 1) {
        parse_option(argc, argv);
        while (*argv) {
            parse_file(&argv, &f);
            print_out(&f);
        }
        if (!file_counter) {
            perror("Set file path");
            exit(EXIT_FAILURE);
        }
    } else {
        perror("EMPTY ARGUMENTS");
        exit(EXIT_FAILURE);
    }
    if (f) fclose(f);
    return EXIT_SUCCESS;
}

