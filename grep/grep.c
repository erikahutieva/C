#include "s21_grep.h"

int main(int argc, char **argv) {
  flag flags = {0};

  int simple = parser(argc, argv, &flags);
  use_flags(argc, argv, &flags, simple);
  return 0;
}

int parser(int argc, char **argv, flag *flags) {
  int res = 0, simple = 0;
  while ((res = getopt(argc, argv, "e:ivclnhsf:o")) != -1) {
    switch (res) {
      case 'e':
        flags->e = 1;
        strcat(flags->main_stroka, optarg);
        strcat(flags->main_stroka, "|");
        simple += 2;
        break;
      case 'i':
        flags->i = 1;
        simple++;
        break;
      case 'v':
        flags->v = 1;
        simple++;
        break;
      case 'c':
        flags->c = 1;
        simple++;
        break;
      case 'n':
        flags->n = 1;
        simple++;
        break;
      case 'h':
        flags->h = 1;
        simple++;
        break;
      case 's':
        flags->s = 1;
        simple++;
        break;
      case 'f':
        simple += 2;
        flags->f = 1;
        char stroka[256] = {'\0'};
        FILE *fr = fopen(optarg, "r");
        if (fr == NULL && flags->s == 0) {
          printf("grep: %s: No such file or directory\n", optarg);
        } else if (fr) {
          while (fgets(stroka, 256, fr) != NULL) {
            if (stroka[0] != '\n' && stroka[strlen(stroka) - 1] == '\n') {
              stroka[strlen(stroka) - 1] = '\0';
            } else if (stroka[0] == '\n') {
            }
            strcat(flags->main_stroka, stroka);
            strcat(flags->main_stroka, "|");
          }

          fclose(fr);
        }
        break;
      case 'o':
        simple++;
        flags->o = 1;
        break;
      case 'l':
        simple++;
        flags->l = 1;
        break;
      default:
        exit(1);
        break;
    }
  }
  if (flags->e || flags->f) {
    flags->main_stroka[strlen(flags->main_stroka) - 1] = '\0';
  }
  return simple;
}

void use_flags(int argc, char **argv, flag *flags, int simple) {
  int plus = 1;
  if ((flags->f == 1 || flags->e == 1)) {
    plus = 0;
  }
  if (flags->n && simple == 1) {
    plus = 2;
  }
  for (int i = optind + plus; i < argc; i++) {
    int result = -1;
    FILE *fp = fopen(argv[i], "r");
    char buffer[256] = {'\0'};
    if (fp == NULL && flags->s == 0) {
      printf("grep: %s: No such file or directory\n", argv[i]);
    } else if (fp) {
      if (optind + plus == argc - 1) {
        flags->h = 1;
      }
      int count_c = 0;
      int count_n = 0;
      while (fgets(buffer, 256, fp) != NULL) {
        regex_t ourstr;
        regmatch_t pmatch[2];
        count_n++;
        if ((flags->f == 1 || flags->e == 1) && flags->i == 1) {
          regcomp(&ourstr, flags->main_stroka, REG_ICASE);
        } else if ((flags->f == 1 || flags->e == 1) && flags->i == 0) {
          regcomp(&ourstr, flags->main_stroka, REG_EXTENDED);
        } else if (flags->i == 1) {
          regcomp(&ourstr, argv[i - plus], REG_ICASE);
        } else {
          regcomp(&ourstr, argv[i - plus], REG_EXTENDED);
        }
        result = regexec(&ourstr, buffer, 0, NULL, 0);
        if ((result != 0 && flags->v) || (result == 0 && flags->v == 0)) {
          if (flags->h == 0 && flags->l == 0 && flags->c == 0) {
            printf("%s:", argv[i]);
          }
          if (flags->n && simple == 1) {
            printf("%s:", argv[i]);
          }
          if (flags->c || flags->l) {
            if (flags->c) {
              count_c++;
            }
            if (flags->l == 1) {
              flags->l = 2;
              break;
            }
          } else if ((flags->n || flags->o)) {
            if (flags->n && flags->o == 0) {
              printf("%d:%s", count_n, buffer);
            } else if (flags->n && flags->o) {
              printf("%d:", count_n);
            }
            if (flags->o) {
              while (regexec(&ourstr, buffer, 2, pmatch, 0) == 0) {
                long long int start = pmatch->rm_so, end = pmatch->rm_eo;
                for (int k = start; k < end; k++) {
                  printf("%c", buffer[k]);
                }
                printf("\n");
                for (int j = end; j < 256; j++) {
                  buffer[j - end] = buffer[j];
                  if (j >= (256 - (256 - end))) {
                    buffer[j] = '\0';
                  }
                }
              }
            }

          } else {
            printf("%s", buffer);
          }
        }
        regfree(&ourstr);
      }
      if (flags->c && (flags->e || flags->l) && optind + plus != argc - 1) {
        printf("%s:", argv[i]);
      }
      if (flags->c == 1) {
        printf("%d\n", count_c);
      }
      if (flags->l == 2) {
        printf("%s\n", argv[i]);
      }
      if (flags->v && flags->l == 0 && flags->c == 0 &&
          buffer[strlen(buffer) - 1] != '\n') {
        printf("\n");
      }
      fclose(fp);
    }
  }
}

