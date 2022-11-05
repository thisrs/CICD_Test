#include "s21_grep.h"

int main(int argc, char **argv) {
    if (argc < 3) {
        perror("few arguments");
        exit(EXIT_FAILURE);
    }
    FILE *fp = NULL;
    pcre *re = NULL;
    parse_option(argc, argv);
    get_files_amount(argv);
    re = parse_pattern(argv);
    while (argv[optind]) {
        parse_file(argv, &fp);
        if (fp) output(re, &fp);
        optind++;
    }
    pcre_free(re);
    return EXIT_SUCCESS;
}

int parse_option(int argc, char **argv) {
    int opt_val;
    FILE *f_file = NULL;
    char* f_file_buf = NULL;
    size_t f_file_size = 0;
    char *opt_available = "hsoicvlne:f:";
    while ((opt_val = getopt_long(argc, argv, opt_available, NULL, NULL)) != EOF) {
        switch (opt_val) {
            case 'h':
                h = true;
                break;
            case 's':
                s = true;
                break;
            case 'f':
                f = true;
                if (optarg) {
                    if (stat(optarg, &sb)) {
                        if (!s) perror("No such file or directory");
                        exit(EXIT_FAILURE);
                    } else {
                        f_file = fopen(optarg, "r");
                        getline(&f_file_buf, &f_file_size, f_file);
                        if (strchr(f_file_buf, '\n')) *(strchr(f_file_buf, '\n')) = '\0';
                        char* reg_val_ptr = regex_val;
                        sprintf(reg_val_ptr, "%s%s%s", regex_val, !reg_val_ptr[0] ? "" : "|", f_file_buf);
                        fclose(f_file);
                        f_file = NULL;
                        free(f_file_buf);
                        f_file_buf = NULL;
                    }
                } else {
                    perror("use pattern");
                    exit(EXIT_FAILURE);
                }
                break;
            case 'o':
                o = true;
                break;
            case 'n':
                n = true;
                break;
            case 'l':
                l = true;
                break;
            case 'c':
                c = true;
                break;
            case 'v':
                v = true;
                break;
            case 'e':
                e = true;
                if (optarg) {
                    char* reg_val_ptr = regex_val;
                    sprintf(reg_val_ptr, "%s%s%s", regex_val, !reg_val_ptr[0] ? "" : "|", optarg);
                } else {
                    perror("use pattern");
                    exit(EXIT_FAILURE);
                }
                break;
            case 'i':
                i = true;
                break;
            default:
                perror("ERROR");
                exit(EXIT_FAILURE);
        }
    }
    return EXIT_SUCCESS;
}

pcre *parse_pattern(char **argv) {
    if (!e && !f) {
        argv++;
        while (**argv) {
            if (**argv != '-') {
                sprintf(regex_val, "%s", *argv);
                break;
            }
            argv++;
        }
        optind++;
    }
    const char *error;
    int erroffset;
    pcre *re;
    re = pcre_compile((char *) regex_val, i ? PCRE_CASELESS : 0, &error, &erroffset, NULL);
    if (!re) {
        perror("Failed at offset ");
        exit(EXIT_FAILURE);
    }

    return re;
}

int get_files_amount(char **argv) {
    for (int x = optind; argv[x]; x++) {
        if (stat(argv[x], &sb)) continue;
        file_counter++;
    }
    if (!file_counter) {
        perror("files doesnt exists");
        exit(EXIT_FAILURE);
    }
    return EXIT_SUCCESS;
}

int parse_file(char **argv, FILE **fp) {
    while (optind) {
        if (stat(argv[optind], &sb)) {
            if (!s) perror("No such file or directory");
            return (EXIT_FAILURE);
        } else {
            *fp = fopen(argv[optind], "r");
            this_file_name = argv[optind];
            break;
        }
    }
    return EXIT_SUCCESS;
}

int output(pcre *re, FILE **fp) {
    char *new_line = NULL;
    int count;
    int c_counter = 0;
    bool l_counter = false;
    size_t len = 0;
    const char *buff_pcre;
    int vector[(0 + 1) * 3];
    int line_counter = 0;

    while ((getline(&new_line, &len, *fp)) != EOF) {
        count = pcre_exec(re, NULL, new_line, (int) strlen(new_line), 0, 0, vector, (0 + 1) * 3);
        line_counter++;
        if (count >= 0 && l && !v) l_counter = true;
        if (count < 0 && l && v) l_counter = true;
        if (count >= 0 && c && !v) {
            c_counter++;
        }
        if (count < 0 && c && v) {
            c_counter++;
        }
        if (count >= 0 && !v && !c && !l) {
            if (!o) {
                if (n && file_counter == 1) printf("%d:%s", line_counter, new_line);
                else if (file_counter > 1 && !n && !h) printf("%s:%s", this_file_name, new_line);
                else if (file_counter > 1 && !n && h) printf("%s", new_line);
                else if (file_counter > 1 && n && h) printf("%d:%s", line_counter, new_line);
                else if (file_counter > 1 && n && !h)
                    printf("%s:%d:%s", this_file_name, line_counter, new_line);
                else
                    printf("%s", new_line);
            } else {
                while (count > 0) {
                    if (pcre_get_substring(new_line, (int *) &vector, count, 0, &buff_pcre) >= 0) {
                        if (n && file_counter == 1) printf("%d:%s\n", line_counter, buff_pcre);
                        else if (file_counter > 1 && !n && !h) printf("%s:%s\n", this_file_name, buff_pcre);
                        else if (file_counter > 1 && !n && h) printf("%s\n", buff_pcre);
                        else if (file_counter > 1 && n && h) printf("%d:%s\n", line_counter, buff_pcre);
                        else if (file_counter > 1 && n && !h)
                            printf("%s:%d:%s\n", this_file_name, line_counter, buff_pcre);
                        else
                            printf("%s\n", new_line);
                    }
                    count = pcre_exec
                            (re, NULL, new_line, (int) strlen(new_line), vector[1], 0, vector, (0 + 1) * 3);
                    pcre_free_substring(buff_pcre);
                    buff_pcre = NULL;
                }
                continue;
            }
        } else if (count < 0 && v && !c && !l) {
            if (n && file_counter == 1) printf("%d:%s", line_counter, new_line);
            else if (file_counter > 1 && !n && !h) printf("%s:%s", this_file_name, new_line);
            else if (file_counter > 1 && n && h) printf("%d:%s", line_counter, new_line);
            else if (file_counter > 1 && n && !h)
                printf("%s:%d:%s", this_file_name, line_counter, new_line);
            else
                printf("%s", new_line);
        }
    }
    if (c && !l) {
        if (file_counter > 1 && !h)
            printf("%s:%d\n", this_file_name, c_counter);
        else
            printf("%d\n", c_counter);
    }
    if (c && l) {
        if (file_counter > 1 && !h)
            printf("%s:%d\n", this_file_name, c_counter > 0 ? 1 : 0);
        else
            printf("%d\n", c_counter > 0 ? 1 : 0);
    }
    if (l_counter) printf("%s\n", this_file_name);
    free(new_line);
    new_line = NULL;
    fclose(*fp);
    *fp = NULL;

    return (EXIT_SUCCESS);
}
