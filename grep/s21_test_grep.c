#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  char str_grep[500], str_s21[500];
  int koll_test = 1;
  char *file_name[] = {
      "test_1_file.txt",
      "test_2_file.txt",
      "test_3_file.txt",
      "test_4_file.txt",
      "test_2_file.txt test_1_file.txt test_4_file.txt test_3_file.txt",
      "test_3_file.txt test_4_file.txt test_1_file.txt",
      "test_3_file.txt test_4_file.txt test_1_file.txt",
      "test_2_file.txt test_1_file.txt test_4_file.txt",
      "test_3_file.txt test_4_file.txt",
      "test_4_file.txt test_1_file.txt"};
  char *flags[] = {"-e", "-i", "-v", "-c",           "-l",
                   "-n", "-h", "-s", "-f regul.txt", "-o"};
  char *pattern[] = {"Clojure",          "g",  "characters", "C#",   "", "The",
                     "\"woman|people\"", "He", "\"^A\"",     "take", ""};
  system("make");
  for (int i = 0; i < 10; i++) {
    for (int ii = 1; ii < 10; ii++) {
      if ((i == 8) || (ii == 8)) {
        sprintf(str_s21, "./s21_grep %s %s %s > test_s21_grep.txt", flags[i],
                flags[ii], file_name[i]);
        sprintf(str_grep, "grep %s %s %s > test_grep.txt", flags[i], flags[ii],
                file_name[i]);
      } else if (i == 0) {
        sprintf(str_s21, "./s21_grep %s %s %s %s > test_s21_grep.txt", flags[i],
                pattern[i], flags[ii], file_name[i]);
        sprintf(str_grep, "grep %s %s %s %s > test_grep.txt", flags[i],
                pattern[i], flags[ii], file_name[i]);
      } else {
        sprintf(str_s21, "./s21_grep %s %s %s %s > test_s21_grep.txt", flags[i],
                flags[ii], pattern[ii], file_name[ii]);
        sprintf(str_grep, "grep %s %s %s %s > test_grep.txt", flags[i],
                flags[ii], pattern[ii], file_name[ii]);
      }
      system(str_s21);
      system(str_grep);
      printf("\ntest: %s\n", str_s21);
      int error = system("cmp test_s21_grep.txt test_grep.txt");
      if (error == 0) {
        printf("\n\ttest # %d - correctly\n", koll_test);
      } else {
        printf("\ttest # %d ERROR !!!\ninput : %s\n", koll_test, str_s21);
        i = 10;
        ii = i;
      }
      koll_test++;
    }
  }
}
