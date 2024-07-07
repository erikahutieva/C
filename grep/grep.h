#define _GNU_SOURCE
#ifndef TEST_H
#define TEST_H

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
typedef struct {
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int f;
  int o;
  char main_stroka[4096];
} flag;
int parser(int argc, char **argv, flag *flags);
void use_flags(int argc, char **argv, flag *flags, int simple);
#endif
