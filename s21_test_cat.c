#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void file_gen(int file, char *file_name[]) {
  for (int i = 0; i <= file; i++) {
    int lengh = 300 + rand() % 300;
    FILE *f = fopen(file_name[i], "w");
    for (int ii = 0; ii < lengh; ii++) {
      fputc(1 + rand() % 255, f);
    }
    fclose(f);
  }
}

void str_null(char *str) {
  int len = (int)strlen(str);
  for (int i = 0; i < len; i++) {
    str[i] = '\0';
  }
}

void my_strcat(char *str_old, char *str_new) {
  int j = strlen(str_old) + 1;
  str_old[j - 1] = ' ';
  for (int i = 0; str_new[i] != '\0'; i++, j++) {
    str_old[j] = str_new[i];
  }
  str_old[j] = '\0';
}
int main() {
  int error = 0;
  int koll_test = 200 + rand() % 200;
  char *file_name[] = {"test_1_file.txt", "test_2_file.txt", "test_3_file.txt",
                       "test_4_file.txt"};
  char *flags[] = {"-b", "-e", "-n", "-s", "-t"};
  system("make");
  for (int i = 0; i < koll_test; i++) {
    int file = rand() % 4;
    int koll_flags = rand() % 2;
    file_gen(file, file_name);
    char str_s21[500] = "";
    char str_cat[500] = "";
    char flags_to_str[400] = "";
    str_null(str_s21);
    str_null(str_cat);
    str_null(flags_to_str);
    for (int ii = 0; ii < koll_flags; ii++) {
      my_strcat(flags_to_str, flags[rand() % 5]);
    }
    for (int iii = 0; iii <= file; iii++) {
      my_strcat(flags_to_str, file_name[iii]);
    }
    sprintf(str_s21, "./s21_cat%s > test_s21_cat.txt", flags_to_str);
    sprintf(str_cat, "cat%s > test_cat.txt", flags_to_str);
    printf("\ntest -%s\n", str_s21);
    system(str_s21);
    system(str_cat);
    error = system("cmp test_s21_cat.txt test_cat.txt");
    if (error == 0) {
      printf("\n\ttest # %d - correctly\n", i);
    } else {
      printf("\ttest # %d ERROR !!!\ninput : %s\n", i, flags_to_str);
      i = koll_test;
    }
  }
}