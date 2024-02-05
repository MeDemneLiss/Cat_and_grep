dCC=gcc
EFLAGS=-Wall -Werror -Wextra

all: 
	$(CC) $(EFLAGS) s21_cat.c -o s21_cat

s21_cat:
	$(CC) $(EFLAGS) s21_cat.c -o s21_cat

test:
	$(CC) $(EFLAGS) s21_test_cat.c -o s21_test_cat
	./s21_test_cat

clean:
	rm -rf *.o *.txt s21_cat s21_test_cat