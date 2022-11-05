CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11 -pedantic -D_GNU_SOURCE
GREP_PATH = s21_grep.c
INSTALL_PCRE = sudo apt-get install libpcre3 libpcre3-dev

s21_grep: #install_pcre
	$(CC) $(CFLAGS) ./s21_grep.c -o ./s21_grep $(shell pcre-config --libs-posix)
	chmod +x ./s21_grep

install_pcre:
	$(INSTALL_PCRE)

all: s21_grep

clean:
	rm -rf s21_grep

