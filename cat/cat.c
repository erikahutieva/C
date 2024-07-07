#include "s21_cat.h"

int main(int argc, char **argv) {
  flag flags = {0};
  int res = parser(argc, argv, &flags);
  use_flags(argc, argv, &flags, res);
  return 0;
}

int parser(int argc, char **argv, flag *flags) {
  int res = -100, simple = 0;
  const struct option long_flags[] = {{"number-nonblank", 0, NULL, 'b'},
                                      {"number", 0, NULL, 'n'},
                                      {"squeeze-blankr", 0, NULL, 's'},
                                      {0, 0, NULL, 0}};
  while ((res = getopt_long(argc, argv, "+benstvTE", long_flags, NULL)) != -1) {
    switch (res) {
      case 'b':
        flags->b = 1;
        simple++;
        break;
      case 'v':
        flags->v = 1;
        simple++;
        break;
      case 'e':
        flags->e = 1;
        flags->v = 1;
        simple++;
        break;
      case 'n':
        flags->n = 1;
        simple++;
        break;
      case 's':
        flags->s = 1;
        simple++;
        break;
      case 't':
        flags->t = 1;
        flags->v = 1;
        simple++;
        break;
      case 'E':
        flags->e = 1;
        simple++;
        break;
      case 'T':
        flags->t = 1;
        simple++;
        break;
      default:
        break;
    }
  }
  return simple;
}

void use_flags(int argc, char **argv, flag *flags, int res) {
  for (int i = res + 1; i < argc; i++) {
    FILE *fp = fopen(argv[i], "r");
    if (fp == NULL) {
      printf("cat: %s: No such file or directory", argv[i]);
      continue;
    }
    char c = fgetc(fp);
    char flag = ' ';
    int i_b = 1, ch_b = 0;
    int i_n = 1, ch_n = 0;
    int ch_s = 0, res_s = 0;
    int ch_E = 0;
    if (c == '\n') ch_s = 1;
    if (fp) {
      while (c != EOF) {
        if (flags->s == 1) {
          res_s = flag_s(&c, &ch_s);
        }
        if (flags->b == 1) {
          if ((flag == '\n' && c != '\n') ||
              (ch_b == 0 && c != '\n' && ch_s <= 2 && ch_s >= 0)) {
            printf("%6d\t", i_b);
            i_b = i_b + 1;
          } /*else if ((flag == '\n' || ch_b == 0) && ch_s <= 2 && ch_s > 0) {
            printf("\t");
          }*/
          else if (flags->e && flag == '\n' && c == '\n' && flags->t) {
            printf("\t");
          }
          ch_b = 1;
        }
        if (flags->n == 1 && flags->b == 0) {
          flag_n(&flag, &ch_n, &i_n, &ch_s);
        }
        if (flags->e == 1 && flags->v == 1) {
          flag_E(&c, &ch_E, &ch_s);
          flag_v(&c);
        }
        if (flags->v == 0 && flags->e == 1) {
          flag_E(&c, &ch_E, &ch_s);
        }
        if (flags->t == 1 && flags->v == 1) {
          if (flags->e == 1) {
            flag_T(&c);
          } else {
            flag_v(&c);
            flag_T(&c);
          }
        }
        if (flags->v == 0 && flags->t == 1) {
          flag_T(&c);
        }
        if (flags->v == 1 && flags->e == 0 && flags->t == 0) {
          flag_v(&c);
        }
        if ((res_s != 0 || flags->s == 0) && !(flags->t == 1 && c == '\t')) {
          printf("%c", c);
        }
        flag = c;
        c = fgetc(fp);
      }
    }
    fclose(fp);
  }
}

void flag_b(char *flag, char *c, int *ch, int *i, int *ch_s) {
  if ((*flag == '\n' && *c != '\n') ||
      (*ch == 0 && *c != '\n' && *ch_s <= 2 && *ch_s > 0)) {
    printf("%6d\t", *i);
    *i = *i + 1;
  } /*else if ((*flag == '\n' || *ch == 0) && *ch_s <= 2 && *ch_s > 0) {
    printf("\t");
  }
  *ch = 1;*/
}

void flag_E(char *c, int *ch, int *ch_s) {
  /*if (*c == '\n' && *ch_s <= 2 && *ch_s >= 0 && )
  else*/
  if (*c == '\n' && *ch_s <= 2 && *ch_s >= 0) {
    printf("$");
  } else {
    *ch = 0;
  }
}
void flag_T(char *c) {
  if (*c == '\t') {
    printf("^I");
  }
}
void flag_n(char *flag, int *ch, int *i, int *ch_s) {
  if (((*flag == '\n') || (*ch == 0)) && *ch_s <= 2 && *ch_s >= 0) {
    printf("%6d\t", *i);
    *i = *i + 1;
  }
  *ch = 1;
}
int flag_s(char *c, int *ch) {
  int res = 1;
  if (*c == '\n') {
    *ch = *ch + 1;
    if (*ch > 2) {
      res = 0;
    }

  } else {
    *ch = 0;
  }
  return res;
}
void flag_v(char *r) {
  int c = *r;
  if (c < 32 && c != 9 && c != 10) {
    printf("^");
    *r = *r + 64;
  } else if (c > 127 && c < 160) {
    printf("M-^");
    *r = *r - 64;
  } else if (c == 127) {
    printf("^?");
    *r = *r - 64;
  }
}

