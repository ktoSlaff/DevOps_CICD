CC=gcc
CFLAGS=-Wall -Werror -Wextra
CAT=s21_cat.c

all: s21_cat

s21_cat:
	$(CC) $(CFLAGS) $(CAT) -o s21_cat

style_check:
	cp ../../../materials/linters/.clang-format .
	clang-format -Werror -n *.c
	rm -rf .clang-format

style_fix:
	cp ../../../materials/linters/.clang-format .
	clang-format -i *.c
	rm -rf .clang-format

clean:
	rm -rf s21_cat

rebuild: clean all