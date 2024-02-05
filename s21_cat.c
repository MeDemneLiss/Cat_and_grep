#include <stdio.h>
#include <string.h>
#define FLAG_b 1
#define FLAG_e 1 << 1
#define FLAG_n 1 << 2
#define FLAG_s 1 << 3
#define FLAG_t 1 << 4
#define FLAG_v 1 << 5
#define M_SIZE 5000

void add_char_to_string(FILE* file, unsigned int flag, int* numstr);
void numbering(int* num);
void endstr(unsigned char* str, int* koll);
void visibility_tab(unsigned char* str, int* max);
void m_notation(unsigned char* str, int* max);
void flag_long_cat(char* flag, unsigned int* flag_memor);
void flag_short_cat(char* flag, unsigned int* flag_memor);
void move_str_right(unsigned char* str, int end, int num, int max);
int my_fgets(unsigned char* str, int max, FILE* file);
void my_printf(FILE* file, unsigned char* str, int koll);

int main(int argc, char* argv[]) {
  unsigned int flags = 0;
  int numstr = 1;
  for (int i = 1; i < argc; i++) {
    if ((argv[i][0] == '-') && (strlen(argv[i]) > 1)) {  // memorizing flags
      if (argv[i][1] == '-') {
        flag_long_cat(argv[i], &flags);
      } else {
        flag_short_cat(argv[i], &flags);
      }
      if (flags == 0) {
        i = argc;
      }
    } else {
      FILE* opening_f = NULL;
      if ((argv[i][0] == '-') &&
          (strlen(argv[i]) == 1)) {  // меняем поток на стандартный(консоль)
        opening_f = stdin;
      } else if ((fopen(argv[i], "r")) !=
                 NULL) {  // здесь обрабатываем файлы считываем и выводим вход
        opening_f = fopen(argv[i], "r");
      } else {
        fprintf(stdout, "fcat: %s: No such file or directory", argv[i]);
      }
      add_char_to_string(opening_f, flags, &numstr);
      fclose(opening_f);
    }
  }
}

void flag_long_cat(char* flag, unsigned int* flag_memor) {
  if (!strcmp(flag, "--number-nonblank")) {
    *flag_memor = *flag_memor | FLAG_b;
  } else if (!strcmp(flag, "--number")) {
    *flag_memor = *flag_memor | FLAG_n;
  } else if (!strcmp(flag, "--squeeze-blank")) {
    *flag_memor = *flag_memor | FLAG_s;
  } else {
    fprintf(stdout, "cat: unrecognized option '%s\n'", flag);
    *flag_memor = 0;
  }
}

void flag_short_cat(char* flag, unsigned int* flag_memor) {
  for (int i = 1; i < (int)strlen(flag); i++) {
    switch (flag[i]) {
      case 'b':
        *flag_memor = *flag_memor | FLAG_b;
        break;
      case 'e':
        *flag_memor = *flag_memor | FLAG_e;
        *flag_memor = *flag_memor | FLAG_v;
        break;
      case 'n':
        *flag_memor = *flag_memor | FLAG_n;
        break;
      case 's':
        *flag_memor = *flag_memor | FLAG_s;
        break;
      case 't':
        *flag_memor = *flag_memor | FLAG_t;
        *flag_memor = *flag_memor | FLAG_v;
        break;
      case 'E':
        *flag_memor = *flag_memor | FLAG_e;
        break;
      case 'T':
        *flag_memor = *flag_memor | FLAG_t;
        break;
      case 'v':
        *flag_memor = *flag_memor | FLAG_v;
        break;
      default:
        i = strlen(flag);
        fprintf(stdout,
                "cat: illegal option -- %c\nusage: cat [-benstuv] [file...]",
                flag[i]);
        *flag_memor = 0;
    }
  }
}

void numbering(int* num) {
  fprintf(stdout, "%6d\t", *num);
  *num = *num + 1;
}

void endstr(unsigned char* str, int* koll) {
  if (str[*koll - 1] == '\n') {
    str[*koll - 1] = '$';
    str[*koll] = '\n';
    str[*koll + 1] = '\0';
    *koll += 1;
  }
}

void visibility_tab(unsigned char* str, int* max) {
  for (int i = 0; i < *max; i++) {
    if (str[i] == 9) {
      move_str_right(str, i, 1, *max);
      str[i] = '^';
      str[i + 1] = 'I';
      *max += 1;
    }
  }
}

void m_notation(unsigned char* str, int* max) {
  for (int i = 0; i < *max; i++) {
    if ((str[i] > 127) && (str[i] < 160)) {
      move_str_right(str, i, 2, *max);
      str[i] = 'M';
      str[i + 1] = '-';
      str[i + 2] = str[i + 2] - 128;
      *max += 2;
      if ((str[i + 2] == 9) || (str[i + 2] == 10)) {
        move_str_right(str, i + 2, 1, *max);
        str[i + 2] = '^';
        str[i + 3] = str[i + 3] + 64;
        *max += 1;
      }
    } else if ((str[i] < 9) || ((str[i] < 32) && (str[i] > 10))) {
      move_str_right(str, i, 1, *max);
      str[i] = '^';
      str[i + 1] = str[i + 1] + 64;
      *max += 1;
    } else if (str[i] == 127) {
      move_str_right(str, i, 1, *max);
      str[i] = '^';
      str[i + 1] = '?';
      *max += 1;
    }
  }
}
void move_str_right(unsigned char* str, int end, int num, int max) {
  for (int ii = max; ii >= end; ii--) {
    str[ii + num] = str[ii];
  }
}

int my_fgets(unsigned char* str, int max, FILE* file) {
  char ch, stop;
  int koll = 0;
  stop = fread(&ch, sizeof(char), 1, file);
  while ((stop != 0) && (ch != '\n') && (koll < max)) {
    str[koll] = ch;
    koll++;
    stop = fread(&ch, sizeof(char), 1, file);
  }
  if (ch == '\n') {
    str[koll] = ch;
    koll++;
  }
  str[koll] = '\0';
  return koll;
}

void add_char_to_string(FILE* file, unsigned int flag, int* numstr) {
  int last_str = 1;
  *numstr = 1;
  while (!feof(file)) {
    unsigned char now_str[M_SIZE] = "";
    int koll = my_fgets(now_str, M_SIZE - 1, file);
    if (koll != 0) {
      if ((flag & FLAG_s) && (now_str[0] == '\n') && (last_str == 0)) {
        koll = -1;
      }
      if ((flag & FLAG_b) && ((now_str[0] != '\n'))) {
        numbering(numstr);
      } else if ((flag & FLAG_n) && (!(flag & FLAG_b))) {
        numbering(numstr);
      }
      if (flag & FLAG_e) {
        endstr(now_str, &koll);
      }
      if (flag & FLAG_t) {
        visibility_tab(now_str, &koll);
      }
      if (flag & FLAG_v) {
        m_notation(now_str, &koll);
      }
      my_printf(stdout, now_str, koll);
      if ((now_str[0] == '\n') || (now_str[0] == '\0')) {
        last_str = 0;
      } else {
        last_str = 1;
      }
    }
  }
}

void my_printf(FILE* file, unsigned char* str, int koll) {
  for (int i = 0; i < koll; i++) {
    fputc(str[i], file);
  }
}