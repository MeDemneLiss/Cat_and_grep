CC=gcc
EFLAGS=-Wall -Werror -Wextra

all: 
	$(CC) $(EFLAGS) s21_grep.c -o s21_grep

s21_cat:
	$(CC) $(EFLAGS) s21_grep.c -o s21_grep

test:
	$(CC) $(EFLAGS) s21_test_grep.c -o s21_test_grep
	./s21_test_grep

clean:
	rm -rf *.o s21_grep test_grep.txt test_s21_grep.txt s21_test_grep