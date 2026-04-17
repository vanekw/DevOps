#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int e;
  int v;
  int c;
  int l;
  int n;
  int h;
  int f;
  int s;
  int o;
  int regex_flags;
  int error;
  int txt;
  int no_e;
} opt;

opt parser(int argc, char **argv);
void print_no_flags(opt flags);
void print_flags(opt flags);
void print_cl_flag(opt flags, int matched_line_sum, char *line);
void flag_e_space(int *i, int argc, char **argv, opt *flags, FILE *f,
                  FILE *txt);
void flag_f_space(int *i, int argc, char **argv, FILE *f, opt *flags,
                  FILE *txt);
void flag_e(int i, int j, opt *flags, FILE *f, char **argv, FILE *txt);
void flag_f(char **argv, int i, opt *flags, FILE *f, int j, FILE *txt);
void illegal_flag(int i, int j, char **argv, opt *flags, FILE *f, FILE *txt);
void flag_work(opt *flags);
int print_v(int regec_count, opt flags, int *matched_line_sum, int str_count,
            char *line, char *stri);
void print_no_v(int *matched_line_sum, int *pattern_match_in_string, opt flags,
                int str_count, char *line, char *stri, regex_t rx,
                regmatch_t match);
void flag_ino_1(opt flags, char *line, int str_count, char *stri, regex_t rx,
                regmatch_t match);
void flag_ino_2(opt flags, int str_count, char *stri, regex_t rx,
                regmatch_t match);
int error_print(int argc, opt *flags);

int main(int argc, char **argv) {
  opt flags = parser(argc, argv);
  if (error_print(argc, &flags) == 0) return 0;
  if (flags.regex_flags == 0 && flags.v == 0 && flags.c == 0 && flags.l == 0 &&
      flags.n == 0 && flags.h == 0 && flags.o == 0) {
    print_no_flags(flags);
  } else {
    print_flags(flags);
  }
  remove("out.txt");
  remove("txt_file.txt");
  return 0;
}

opt parser(int argc, char **argv) {
  opt flags = {0};
  FILE *f = fopen("out.txt", "w+");
  FILE *txt = fopen("txt_file.txt", "w+");
  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      if (strcmp(argv[i], "-e") == 0) {
        flag_e_space(&i, argc, argv, &flags, f, txt);
        if (flags.error == 1) return flags;
      } else if (strcmp(argv[i], "-f") == 0) {
        flag_f_space(&i, argc, argv, f, &flags, txt);
        if (flags.error == 1) return flags;
      } else {
        for (long unsigned int j = 1; j < strlen(argv[i]); j++) {
          if (argv[i][j] == 'e') {
            flag_e(i, j, &flags, f, argv, txt);
            if (flags.error == 1) return flags;
            break;
          } else if (argv[i][j] == 'i')
            flags.regex_flags |= REG_ICASE;
          else if (argv[i][j] == 'f') {
            flag_f(argv, i, &flags, f, j, txt);
            if (flags.error == 1) return flags;
            break;
          } else if (argv[i][j] == 'v')
            flags.v = 1;
          else if (argv[i][j] == 'h')
            flags.h = 1;
          else if (argv[i][j] == 'c')
            flags.c = 1;
          else if (argv[i][j] == 'l')
            flags.l = 1;
          else if (argv[i][j] == 'o')
            flags.o = 1;
          else if (argv[i][j] == 'n')
            flags.n = 1;
          else if (argv[i][j] == 's')
            flags.s = 1;
          else {
            illegal_flag(i, j, argv, &flags, f, txt);
            if (flags.error == 1) return flags;
          }
        }
      }
    } else {
      fputs(argv[i], txt);
      fputs("\n", txt);
      flags.txt++;
    }
  }
  fclose(f);
  fclose(txt);
  flag_work(&flags);
  return flags;
}

void flag_e_space(int *i, int argc, char **argv, opt *flags, FILE *f,
                  FILE *txt) {
  if (*(i) < argc - 1) {
    if (argv[*(i) + 1][strlen(argv[*(i) + 1]) - 1] == '\\') {
      fprintf(stderr, "grep: trailing backslash (\\)\n");
      flags->error = 1;
      fclose(f);
      fclose(txt);
      remove("txt_file.txt");
      remove("out.txt");
      return;
    }
    fputs(argv[*(i) + 1], f);
    fputs("\n", f);
    flags->e++;
    *(i) = *(i) + 1;
  } else {
    fprintf(stderr,
            "grep: option requires an argument -- e\nsage: grep "
            "[-abcDEFGHhIiJLlmnOoqRSsUVvwxZ] [-A num] [-B num] "
            "[-C[num]]\n[-e pattern] [-f file] [--binary-files=value] "
            "[--color=when]\n[--context[=num]] [--directories=action] "
            "[--label] [--line-buffered]\n[--null] [pattern] [file ...]\n");
    flags->error = 1;
    fclose(f);
    fclose(txt);
    remove("out.txt");
    remove("txt_file.txt");
  }
}

void flag_f_space(int *i, int argc, char **argv, FILE *f, opt *flags,
                  FILE *txt) {
  if (*(i) < argc - 1) {
    FILE *f_file = fopen(argv[*(i) + 1], "r");
    if (!f_file) {
      fprintf(stderr, "grep: %s: No such file or directory", argv[*(i) + 1]);
      fclose(f);
      fclose(txt);
      remove("txt_file.txt");
      remove("out.txt");
      flags->error = 1;
      return;
    } else {
      char pattern[256];
      while (fgets(pattern, 256, f_file) != NULL) {
        if (pattern[strlen(pattern) - 1] == '\n')
          pattern[strlen(pattern) - 1] = '\0';
        if (pattern[strlen(pattern) - 1] == '\\') {
          fprintf(stderr, "grep: trailing backslash (\\)\n");
          flags->error = 1;
          fclose(f);
          fclose(txt);
          fclose(f_file);
          remove("txt_file.txt");
          remove("out.txt");
          return;
        }
        fputs(pattern, f);
        fputs("\n", f);
        flags->e++;
      }
      (*i) = *(i) + 1;
      fclose(f_file);
    }
  } else {
    fprintf(stderr,
            "grep: option requires an argument -- f\nsage: grep "
            "[-abcDEFGHhIiJLlmnOoqRSsUVvwxZ] [-A num] [-B num] "
            "[-C[num]]\n[-e pattern] [-f file] [--binary-files=value] "
            "[--color=when]\n[--context[=num]] [--directories=action] "
            "[--label] [--line-buffered]\n[--null] [pattern] [file ...]\n");
    flags->error = 1;
    fclose(f);
    remove("out.txt");
    fclose(txt);
    remove("txt_file.txt");
    return;
  }
}

void flag_e(int i, int j, opt *flags, FILE *f, char **argv, FILE *txt) {
  if (argv[i][strlen(argv[i]) - 1] == '\\') {
    fprintf(stderr, "grep: trailing backslash (\\)\n");
    flags->error = 1;
    fclose(f);
    fclose(txt);
    remove("txt_file.txt");
    remove("out.txt");
    return;
  }
  for (long unsigned int m = j + 1; m < strlen(argv[i]); m++) {
    fputc(argv[i][m], f);
  }
  fputs("\n", f);
  flags->e++;
}

void flag_f(char **argv, int i, opt *flags, FILE *f, int j, FILE *txt) {
  char name[strlen(argv[i]) - (j + 1)];
  for (long unsigned int m = 0; m < strlen(argv[i]); m++) {
    name[m] = argv[i][m + j + 1];
  }
  FILE *f_file = fopen(name, "r");
  if (!f_file) {
    if (flags->s != 1)
      fprintf(stderr, "grep: %s: No such file or directory\n", name);
    fclose(f);
    fclose(txt);
    remove("out.txt");
    remove("txt_file.txt");
    flags->error = 1;
    return;
  } else {
    char pattern[256];
    while (fgets(pattern, 256, f_file) != NULL) {
      if (pattern[strlen(pattern) - 1] == '\n')
        pattern[strlen(pattern) - 1] = '\0';
      if (pattern[strlen(pattern) - 1] == '\\') {
        fprintf(stderr, "grep: trailing backslash (\\)\n");
        flags->error = 1;
        fclose(f);
        fclose(txt);
        fclose(f_file);
        remove("txt_file.txt");
        remove("out.txt");
        return;
      }
      fputs(pattern, f);
      fputs("\n", f);
      flags->e++;
    }
    fclose(f_file);
  }
}

void illegal_flag(int i, int j, char **argv, opt *flags, FILE *f, FILE *txt) {
  fprintf(stderr,
          "grep: illegal option -- %c\nsage: grep "
          "[-abcDEFGHhIiJLlmnOoqRSsUVvwxZ] [-A num] [-B num] "
          "[-C[num]]\n[-e pattern] [-f file] [--binary-files=value] "
          "[--color=when]\n[--context[=num]] [--directories=action] "
          "[--label] [--line-buffered]\n[--null] [pattern] [file ...]\n",
          argv[i][j]);
  flags->error = 1;
  fclose(f);
  fclose(txt);
  remove("txt_file.txt");
  remove("out.txt");
}

void flag_work(opt *flags) {
  if (flags->v == 1 && flags->o == 1 && (flags->l == 1 || flags->c == 1))
    flags->o = 0;
  else if (flags->v == 1 && flags->o == 1)
    flags->error = 1;
}

void print_no_flags(opt flags) {
  FILE *txt = fopen("txt_file.txt", "r");
  char line[1024];
  while (fgets(line, 1024, txt) != NULL) {
    if (flags.no_e == 1) {
      flags.no_e = 0;
      continue;
    }
    if (line[strlen(line) - 1] == '\n') line[strlen(line) - 1] = '\0';
    FILE *filetowork = fopen(line, "r");
    if (!filetowork) {
      if (flags.s != 1)
        fprintf(stderr, "grep: %s: No such file or directory\n", line);
      continue;
    }
    char stri[1024];
    while (fgets(stri, 1024, filetowork) != NULL) {
      if (stri[strlen(stri) - 1] == '\n') stri[strlen(stri) - 1] = '\0';
      FILE *words = fopen("out.txt", "r");
      char pattern[1024];
      while (fgets(pattern, 1024, words) != NULL) {
        if (pattern[strlen(pattern) - 1] == '\n')
          pattern[strlen(pattern) - 1] = '\0';
        regex_t rx;
        int d1 = regcomp(&rx, pattern, 0);
        d1 = regexec(&rx, stri, 0, NULL, 0);
        if (d1 == 0) {
          if (flags.txt > 1 && flags.h != 1)
            printf("%s:%s\n", line, stri);
          else
            printf("%s\n", stri);
          regfree(&rx);
          break;
        }
        regfree(&rx);
      }
      fclose(words);
    }
    fclose(filetowork);
  }
  fclose(txt);
}

void print_flags(opt flags) {
  FILE *txt = fopen("txt_file.txt", "r");
  char line[1024];
  while (fgets(line, 1024, txt) != NULL) {
    if (flags.no_e == 1) {
      flags.no_e = 0;
      continue;
    }
    if (line[strlen(line) - 1] == '\n') line[strlen(line) - 1] = '\0';
    FILE *filetowork = fopen(line, "r");
    if (!filetowork) {
      if (flags.s != 1)
        fprintf(stderr, "grep: %s: No such file or directory\n", line);
      continue;
    }
    char stri[1024];
    int str_count = 0;
    int matched_line_sum = 0;
    regmatch_t match;
    while (fgets(stri, 1024, filetowork) != NULL) {
      str_count++;
      if (stri[strlen(stri) - 1] == '\n') stri[strlen(stri) - 1] = '\0';
      FILE *words = fopen("out.txt", "r");
      char pattern[1024];
      int regec_count = 0;
      int pattern_match_in_string = 0;
      while (fgets(pattern, 1024, words) != NULL) {
        if (pattern[strlen(pattern) - 1] == '\n')
          pattern[strlen(pattern) - 1] = '\0';
        regex_t rx;
        regcomp(&rx, pattern, flags.regex_flags);
        if (flags.v == 1) {
          if (regexec(&rx, stri, 0, NULL, 0) != 0) regec_count++;
          regfree(&rx);
          if (print_v(regec_count, flags, &matched_line_sum, str_count, line,
                      stri) == 1)
            break;
        } else {
          if (regexec(&rx, stri, 1, &match, 0) == 0)
            print_no_v(&matched_line_sum, &pattern_match_in_string, flags,
                       str_count, line, stri, rx, match);
          regfree(&rx);
        }
      }
      fclose(words);
    }
    fclose(filetowork);
    print_cl_flag(flags, matched_line_sum, line);
  }
  fclose(txt);
}

int print_v(int regec_count, opt flags, int *matched_line_sum, int str_count,
            char *line, char *stri) {
  if (regec_count == flags.e) {
    *(matched_line_sum) = *(matched_line_sum) + 1;
    if (flags.l != 1 && flags.c != 1) {
      if (flags.txt > 1 && flags.h != 1) {
        if (flags.n == 1)
          printf("%s:%d:%s\n", line, str_count, stri);
        else
          printf("%s:%s\n", line, stri);
        return 1;
      } else {
        if (flags.n == 1)
          printf("%d:%s\n", str_count, stri);
        else
          printf("%s\n", stri);
        return 1;
      }
    }
  }
  return 0;
}

void print_no_v(int *matched_line_sum, int *pattern_match_in_string, opt flags,
                int str_count, char *line, char *stri, regex_t rx,
                regmatch_t match) {
  if (*(pattern_match_in_string) == 0 || flags.o == 1) {
    *(matched_line_sum) = *(matched_line_sum) + 1;
    if (flags.l != 1 && flags.c != 1) {
      if (flags.txt > 1 && flags.h != 1) {
        flag_ino_1(flags, line, str_count, stri, rx, match);
      } else {
        flag_ino_2(flags, str_count, stri, rx, match);
      }
    }
  }
  *(pattern_match_in_string) = *(pattern_match_in_string) + 1;
}

void flag_ino_1(opt flags, char *line, int str_count, char *stri, regex_t rx,
                regmatch_t match) {
  if (flags.n == 1) {
    if (flags.o == 1) {
      printf("%s:%d:%.*s\n", line, str_count, (int)(match.rm_eo - match.rm_so),
             stri + match.rm_so);
      char *remaining = stri + match.rm_eo;
      while (regexec(&rx, remaining, 1, &match, 0) == 0) {
        printf("%s:%d:%.*s\n", line, str_count,
               (int)(match.rm_eo - match.rm_so), remaining + match.rm_so);
        remaining = remaining + match.rm_eo;
      }
    } else
      printf("%s:%d:%s\n", line, str_count, stri);
  } else {
    if (flags.o == 1) {
      printf("%s:%.*s\n", line, (int)(match.rm_eo - match.rm_so),
             stri + match.rm_so);
      char *remaining = stri + match.rm_eo;
      while (regexec(&rx, remaining, 1, &match, 0) == 0) {
        printf("%s:%.*s\n", line, (int)(match.rm_eo - match.rm_so),
               remaining + match.rm_so);
        remaining = remaining + match.rm_eo;
      }
    } else
      printf("%s:%s\n", line, stri);
  }
}

void flag_ino_2(opt flags, int str_count, char *stri, regex_t rx,
                regmatch_t match) {
  if (flags.n == 1) {
    if (flags.o == 1) {
      printf("%d:%.*s\n", str_count, (int)(match.rm_eo - match.rm_so),
             stri + match.rm_so);
      char *remaining = stri + match.rm_eo;
      while (regexec(&rx, remaining, 1, &match, 0) == 0) {
        printf("%d:%.*s\n", str_count, (int)(match.rm_eo - match.rm_so),
               remaining + match.rm_so);
        remaining = remaining + match.rm_eo;
      }
    } else
      printf("%d:%s\n", str_count, stri);
  } else {
    if (flags.o == 1) {
      printf("%.*s\n", (int)(match.rm_eo - match.rm_so), stri + match.rm_so);
      char *remaining = stri + match.rm_eo;
      while (regexec(&rx, remaining, 1, &match, 0) == 0) {
        printf("%.*s\n", (int)(match.rm_eo - match.rm_so),
               remaining + match.rm_so);
        remaining = remaining + match.rm_eo;
      }
    } else
      printf("%s\n", stri);
  }
}

void print_cl_flag(opt flags, int matched_line_sum, char *line) {
  if ((flags.c == 1 && flags.l == 0)) {
    if (flags.txt > 1 && flags.h != 1)
      printf("%s:%d\n", line, matched_line_sum);
    else
      printf("%d\n", matched_line_sum);
  } else if ((flags.c == 0 && flags.l == 1)) {
    printf("%s\n", line);
  } else if ((flags.c == 1 && flags.l == 1)) {
    printf("%s\n", line);
  }
}

int error_print(int argc, opt *flags) {
  if (argc == 1) {
    fprintf(stderr,
            "usage: grep [-abcDEFGHhIiJLlmnOoqRSsUVvwxZ] [-A num] [-B num] "
            "[-C[num]]\n[-e pattern] [-f file] [--binary-files=value] "
            "[--color=when]\n[--context[=num]] [--directories=action] "
            "[--label] [--line-buffered]\n[--null] [pattern] [file ...]\n");
    return 0;
  }
  if (flags->error == 1) return 0;
  if (flags->e == 0 && flags->txt > 1) {
    FILE *f = fopen("out.txt", "w+");
    FILE *txt = fopen("txt_file.txt", "r");
    char stri[1024];
    fgets(stri, 1024, txt);
    if (stri[strlen(stri) - 1] == '\n') stri[strlen(stri) - 1] = '\0';
    if (stri[strlen(stri) - 1] == '\\') {
      fprintf(stderr, "grep: trailing backslash (\\)\n");
      flags->error = 1;
      fclose(f);
      fclose(txt);
      remove("txt_file.txt");
      remove("out.txt");
      return 0;
    }
    fputs(stri, f);
    flags->e++;
    flags->txt--;
    flags->no_e = 1;
    fclose(f);
    fclose(txt);
  } else if ((flags->e == 0 && flags->txt == 1) ||
             (flags->e == 1 && flags->txt == 0)) {
    remove("out.txt");
    remove("txt_file.txt");
    return 0;
  } else if (flags->e == 0 && flags->txt == 0) {
    fprintf(stderr,
            "usage: grep [-abcDEFGHhIiJLlmnOoqRSsUVvwxZ] [-A num] [-B num] "
            "[-C[num]]\n[-e pattern] [-f file] [--binary-files=value] "
            "[--color=when]\n[--context[=num]] [--directories=action] "
            "[--label] [--line-buffered]\n[--null] [pattern] [file ...]\n");
    remove("out.txt");
    remove("txt_file.txt");
    return 0;
  }
  return 1;
}