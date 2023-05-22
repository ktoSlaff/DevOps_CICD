CC=gcc
CFLAGS=-Wall -Werror -Wextra
CAT=s21_grep.c

all: s21_grep

s21_grep:
	$(CC) $(CFLAGS) $(CAT) -o s21_grep

style_check:
	cp ../../../materials/linters/.clang-format .
	clang-format -Werror -n *.c
	rm -rf .clang-format

style_fix:
	cp ../../../materials/linters/.clang-format .
	clang-format -i *.c
	rm -rf .clang-format

clean:
	rm -rf s21_grep

rebuild: clean all