all:s21_cat

s21_cat:
	gcc -Wall -Werror -Wextra -std=c11 s21_cat.c -o s21_cat

test: s21_cat
	@bash s21_cat_test.sh

clean:
	rm s21_cat