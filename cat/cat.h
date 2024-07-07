#ifndef TEST_H
#define TEST_H

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
typedef struct {
  int b;
  int e;
  int n;
  int s;
  int t;
  int v;
} flag;
int parser(int argc, char **argv, flag *flags);
void use_flags(int argc, char **argv, flag *flags, int res);
void flag_b(char *flag, char *c, int *ch, int *i, int *ch_s);
void flag_n(char *flag, int *ch, int *i, int *ch_s);
int flag_s(char *c, int *ch);
void flag_E(char *c, int *ch, int *ch_s);
void flag_v(char *r);
void flag_T(char *c);
#endif
