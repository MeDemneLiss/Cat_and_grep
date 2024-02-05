#include <regex.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#define FLAG_e 1
#define FLAG_i 1 << 1
#define FLAG_v 1 << 2
#define FLAG_c 1 << 3
#define FLAG_l 1 << 4
#define FLAG_n 1 << 5
#define FLAG_h 1 << 6
#define FLAG_s 1 << 7
#define FLAG_f 1 << 8
#define FLAG_o 1 << 9
#define M_SIZE 5000

int word_processing(FILE* file, unsigned int flag, char* example, int coll_file,
                    char* name_f);
void flag_grep(char* flag, unsigned int* flag_memor, char* example, char* argv,
               int* i);
int coincidence(char* str, regex_t* preg, int flags, int coll_file,
                char* name_f, int num_str);
void no_registr(char* str, int max);
void my_printf(FILE* file, char* str, int koll);
int my_fgets(char* str, int max, FILE* file);
void flag_e_pars(char* example, char* argv, int* i);
int flag_f_pars(char* example, char* argv, int* i);

int main(int argc, char* argv[]) {
  unsigned int flags = 0;
  char example[M_SIZE];
  int coll_file = 0;
  example[0] = '\0';
  for (int i = 1; i < argc; i++) {
    if ((argv[i][0] == '-') && (strlen(argv[i]) > 1)) {  // memorizing flags
      flag_grep(argv[i], &flags, example, argv[i + 1], &i);
      if (flags == 0) {
        i = argc;
      }
    } else if (example[0] == '\0') {
      strcat(example, argv[i]);
    } else {
      coll_file += argc - 1 - i;
      FILE* opening_f = NULL;
      if ((argv[i][0] == '-') &&
          (strlen(argv[i]) == 1)) {  // меняем поток на стандартный(консоль)
        opening_f = stdin;
      } else if ((fopen(argv[i], "r")) !=
                 NULL) {  // здесь обрабатываем файлы считываем и выводим вход
        opening_f = fopen(argv[i], "r");
        int num =
            word_processing(opening_f, flags, example, coll_file, argv[i]);
        if (flags & FLAG_c) {
          if (((!(flags & FLAG_h)) && ((coll_file > 0)))) {
            fprintf(stdout, "%s:", argv[i]);
          }
          if ((flags & FLAG_l) && (num > 1)) num = 1;
          fprintf(stdout, "%d\n", num);
        }
        if ((flags & FLAG_l) && (num > 0)) {
          fprintf(stdout, "%s\n", argv[i]);
        }
      } else if (!(flags & FLAG_s)) {
        fprintf(stdout, "grep: %s: No such file or directory\n", argv[i]);
      }
      fclose(opening_f);
    }
  }
}

void flag_e_pars(char* example, char* argv, int* i) {
  if (example[0] != '\0') {
    strcat(example, "|");
  }
  strcat(example, argv);
  *i += 1;
}

int flag_f_pars(char* example, char* argv, int* i) {
  int rezult = 1;
  FILE* f = fopen(argv, "r");
  if (f != NULL) {
    while (!feof(f)) {
      char str[500];
      fgets(str, 500, f);
      if (str[strlen(str) - 1] == '\n') str[strlen(str) - 1] = '\0';
      if (example[0] != '\0') {
        strcat(example, "|");
      }
      strcat(example, str);
    }
    *i += 1;
  } else
    fclose(f);
  rezult = 0;
  return rezult;
}

void flag_grep(char* flag, unsigned int* flag_memor, char* example, char* argv,
               int* ii) {
  for (int i = 1; i < (int)strlen(flag); i++) {
    switch (flag[i]) {
      case 'e':
        flag_e_pars(example, argv, ii);
        *flag_memor = *flag_memor | FLAG_e;
        break;
      case 'i':
        *flag_memor = *flag_memor | FLAG_i;
        break;
      case 'v':
        *flag_memor = *flag_memor | FLAG_v;
        break;
      case 'c':
        *flag_memor = *flag_memor | FLAG_c;
        break;
      case 'l':
        *flag_memor = *flag_memor | FLAG_l;
        break;
      case 'n':
        *flag_memor = *flag_memor | FLAG_n;
        break;
      case 'h':
        *flag_memor = *flag_memor | FLAG_h;
        break;
      case 's':
        *flag_memor = *flag_memor | FLAG_s;
        break;
      case 'f':
        *flag_memor = *flag_memor | FLAG_f;
        flag_f_pars(example, argv, ii);
        break;
      case 'o':
        *flag_memor = *flag_memor | FLAG_o;
        break;
      default:
        i = strlen(flag);
        fprintf(stdout, "grep: invalid option -- '%c'", flag[i]);
        *flag_memor = 0;
    }
  }
}

int coincidence(char* str, regex_t* preg, int flags, int coll_file,
                char* name_f, int num_str) {
  size_t nmatch = 1;
  regmatch_t pmatch[1];
  int rezult = regexec(preg, str, nmatch, pmatch, 0);
  if ((flags & FLAG_o) && (rezult == 0) && (!(flags & FLAG_v)) &&
      (!(flags & FLAG_c)) && (!(flags & FLAG_l))) {
    int i = 1;
    if ((!(flags & FLAG_h)) && (coll_file > 0)) fprintf(stdout, "%s:", name_f);
    if (flags & FLAG_n) fprintf(stdout, "%d:", num_str);
    while (i) {
      int len = pmatch->rm_eo - pmatch->rm_so;
      if (regexec(preg, str, nmatch, pmatch, 0)) {
        i = 0;
      } else {
        printf("%.*s\n", len, str + pmatch->rm_so);
        str += pmatch->rm_eo;
      }
    }
  }
  return rezult;
}

int my_fgets(char* str, int max, FILE* file) {
  char ch, stop;
  int koll = 0;
  stop = fread(&ch, sizeof(char), 1, file);
  while ((stop != 0) && (ch != '\n') && (koll < max)) {
    str[koll] = ch;
    koll++;
    stop = fread(&ch, sizeof(char), 1, file);
  }
  str[koll] = '\n';
  koll++;
  str[koll] = '\0';
  return koll;
}

void pattern_create(char* exemple, int flags, regex_t* preg) {
  int c_flags = 0;
  if ((flags & FLAG_e) || (flags & FLAG_f)) c_flags = c_flags | REG_EXTENDED;
  if (flags & FLAG_i) c_flags = c_flags | REG_ICASE;
  regcomp(preg, exemple, c_flags);
}

int word_processing(FILE* file, unsigned int flags, char* example,
                    int coll_file, char* name_f) {
  int num_str = 1, matching_str = 0, is_print_str, koll;
  regex_t re;
  pattern_create(example, flags, &re);
  while (!feof(file)) {
    char str[M_SIZE];
    koll = my_fgets(str, M_SIZE - 1, file);
    if (koll != 0) {
      is_print_str = coincidence(str, &re, flags, coll_file, name_f, num_str);
      if (((is_print_str == 0) && (!(flags & FLAG_v))) ||
          ((flags & FLAG_v) && (is_print_str != 0))) {
        matching_str++;
        is_print_str = 0;
      } else {
        is_print_str = -1;
      }
      if (((!(flags & FLAG_o)) || (flags & FLAG_v)) &&
          (!((flags & FLAG_c) || (flags & FLAG_l)) && (is_print_str == 0))) {
        if (((!(flags & FLAG_h)) && (coll_file > 0)))
          fprintf(stdout, "%s:", name_f);
        if (flags & FLAG_n) fprintf(stdout, "%d:", num_str);
        my_printf(stdout, str, koll);
      }
      if ((str[koll - 1] != '\n') && (is_print_str == 0) && (coll_file > 0) &&
          (!(flags & FLAG_l)) && (!(flags & FLAG_c))) {
      }
    }
    num_str++;
  }
  regfree(&re);
  return matching_str;
}

void my_printf(FILE* file, char* str, int koll) {
  for (int i = 0; i < koll; i++) {
    fputc(str[i], file);
  }
}