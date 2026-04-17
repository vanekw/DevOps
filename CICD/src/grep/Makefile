all:s21_grep

s21_grep:
	gcc -Wall -Werror -Wextra -std=c11 s21_grep.c -o s21_grep

test: s21_grep
	@bash s21_grep_test.sh

clean:
	rm s21_grep