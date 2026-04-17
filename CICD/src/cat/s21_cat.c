#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int b;
  int e;
  int n;
  int s;
  int t;
  int v;
  int error;
  int count;
} opt;

int print_file(char *name, int argc, char **argv);
opt parser(int argc, char **argv);
void print_flags(opt flags, FILE *f);
void print_no_flags(FILE *f);
opt parser_long(char **argv, int i, opt flags);
opt parser_short(char **argv, int i, opt flags);

int main(int argc, char **argv) {
  opt flags = parser(argc, argv);
  if (flags.error == 1)
    return 0;
  else {
    for (int i = 1 + flags.count; i < argc; i++) {
      int p = print_file(argv[i], argc, argv);
      if (p == 0) continue;
    }
    return 0;
  }
}

int print_file(char *name, int argc, char **argv) {
  opt flags = parser(argc, argv);
  FILE *f = fopen(name, "rt");
  if (f == NULL) {
    fprintf(stderr, "cat: %s: No such file or directory\n", name);
    return 0;
  }
  if (flags.b == 0 && flags.e == 0 && flags.n == 0 && flags.s == 0 &&
      flags.t == 0 && flags.v == 0)
    print_no_flags(f);
  else
    print_flags(flags, f);
  fclose(f);
  return 1;
}

opt parser(int argc, char **argv) {
  opt flags = {0};
  for (int i = 1; i < argc; i++) {
    if (argv[i][0] != '-')
      break;
    else {
      flags.count++;
      if (argv[i][1] == '-') {
        flags = parser_long(argv, i, flags);
        if (flags.error == 1) return flags;
      } else {
        flags = parser_short(argv, i, flags);
        if (flags.error == 1) return flags;
      }
    }
  }
  return flags;
}

void print_flags(opt flags, FILE *f) {
  int c = fgetc(f);
  int prev = '\n';
  int prev_prev;
  int index = 0;
  while (c != EOF) {
    if (flags.s == 1 && prev == '\n' && c == '\n' && prev_prev == '\n') {
      prev_prev = prev;
      prev = c;
      c = fgetc(f);
      continue;
    }
    if ((flags.n == 1 && flags.b == 0 && prev == '\n') ||
        (flags.b == 1 && c != '\n' && prev == '\n')) {
      index += 1;
      printf("%6d\t", index);
    }
    if (flags.e == 1 && c == '\n') {
      printf("$");
    }
    if (flags.t == 1 && c == '\t') {
      printf("^I");
      prev_prev = prev;
      prev = c;
      c = fgetc(f);
      continue;
    }
    if (flags.v == 1) {
      if (c > 127 && c < 160) printf("M-^");
      if ((c < 32 && c != '\n' && c != '\t') || c == 127) printf("^");
      if ((c < 32 || (c > 126 && c < 160)) && c != '\n' && c != '\t')
        c = c > 126 ? c - 128 + 64 : c + 64;
    }
    putc(c, stdout);
    prev_prev = prev;
    prev = c;
    c = fgetc(f);
  }
}

void print_no_flags(FILE *f) {
  int c = fgetc(f);
  while (c != EOF) {
    putc(c, stdout);
    c = fgetc(f);
  }
}

opt parser_long(char **argv, int i, opt flags) {
  if (strcmp(argv[i], "--number-nonblank") == 0)
    flags.b = 1;
  else if (strcmp(argv[i], "--number") == 0)
    flags.n = 1;
  else if (strcmp(argv[i], "--squeeze-blank") == 0)
    flags.s = 1;
  else if (strcmp(argv[i], "--") == 0)
    return flags;
  else {
    fprintf(stderr,
            "cat: illegal option -- %c\nusage: cat [-benstuv] [file ...]\n",
            argv[i][2]);
    flags.error = 1;
  }
  return flags;
}

opt parser_short(char **argv, int i, opt flags) {
  for (long unsigned int j = 1; j < strlen(argv[i]); j++) {
    if (argv[i][j] == 'b')
      flags.b = 1;
    else if (argv[i][j] == 'n')
      flags.n = 1;
    else if (argv[i][j] == 'E')
      flags.e = 1;
    else if (argv[i][j] == 'T')
      flags.t = 1;
    else if (argv[i][j] == 'v')
      flags.v = 1;
    else if (argv[i][j] == 'e') {
      flags.e = 1;
      flags.v = 1;
    } else if (argv[i][j] == 's')
      flags.s = 1;
    else if (argv[i][j] == 't') {
      flags.t = 1;
      flags.v = 1;
    } else {
      fprintf(stderr,
              "cat: illegal option -- %c\nusage: cat [-benstuv] [file ...]\n",
              argv[i][j]);
      flags.error = 1;
    }
  }
  return flags;
}