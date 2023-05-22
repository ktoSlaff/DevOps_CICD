#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define _GNU_SOURCE

typedef struct options {
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
  int few_files;
  char pattern[1024];
} flags;

void parser(int argc, char *argv[], flags *options);
void reader(char *filename, flags *options);
void flag_f(char *argv, flags *options);
void flag_c_l(flags *options, int match_count, char *filename);

int main(int argc, char *argv[]) {
  if (argc > 2) {
    flags options = {0};
    parser(argc, argv, &options);
  }
  return 0;
}

void parser(int argc, char *argv[], flags *options) {
  int opt = 0, res = 1;
  while ((opt = getopt_long(argc, argv, "e:ivhclsnf:o", 0, NULL)) != -1) {
    switch (opt) {
      case 'e':
        options->e = 1;
        strcat(options->pattern, optarg);
        strcat(options->pattern, "|");
        break;
      case 'i':
        options->i = 1;
        break;
      case 'v':
        options->v = 1;
        break;
      case 'c':
        if (!options->l) options->c = 1;
        break;
      case 'l':
        options->l = 1;
        options->c = 0;
        break;
      case 'n':
        options->n = 1;
        break;
      case 'h':
        options->h = 1;
        break;
      case 's':
        options->s = 1;
        break;
      case 'f':
        options->f = 1;
        flag_f(optarg, options);
        break;
      case 'o':
        options->o = 1;
        break;

      default:
        fprintf(stderr,
                "usage: s21_grep [-e:ivhclsnf:o] [-e pattern] [-f file] "
                "[pattern] [file ...]\n");
        res = 0;
        break;
    }
  }
  if (res) {
    if (!options->e && !options->f) {
      strcpy(options->pattern, argv[optind]);
      optind++;
    } else {
      options->pattern[strlen(options->pattern) - 1] = 0;
    }
    if (argc - optind > 1) options->few_files = 1;
    for (; optind < argc; optind++) reader(argv[optind], options);
  }
}

void reader(char *filename, flags *options) {
  regex_t preg = {0};
  char *line = NULL;
  size_t len = 0;
  regmatch_t pmatch[1] = {0};
  int line_number = 0, match_count = 0;
  int cflags = REG_EXTENDED;
  if (options->i) cflags = REG_EXTENDED | REG_ICASE;
  regfree(&preg);
  int comp_res = regcomp(&preg, options->pattern, cflags);
  FILE *file = fopen(filename, "r");
  if (file && comp_res == 0) {
    while (getline(&line, &len, file) != -1) {
      line_number++;
      int reg_stat = regexec(&preg, line, 1, pmatch, 0);
      if ((reg_stat == 0 && !(options->v)) || (reg_stat != 0 && options->v)) {
        match_count++;
        if (options->l) {
          flag_c_l(options, match_count, filename);
          break;
        }
        if (options->c) continue;
        if (options->o) {
          char *cut_line = line;
          while (!reg_stat) {
            if (!options->h && options->few_files) printf("%s:", filename);
            if (options->n) printf("%d:", line_number);
            for (int i = pmatch->rm_so; i < pmatch->rm_eo; i++)
              printf("%c", cut_line[i]);
            printf("\n");
            cut_line += pmatch->rm_eo;
            reg_stat = regexec(&preg, cut_line, 1, pmatch, 0);
          }
        } else {
          if (!options->h && options->few_files) printf("%s:", filename);
          if (options->n) printf("%d:", line_number);
          printf("%s", line);
          if (line[strlen(line) - 1] != '\n') printf("\n");
        }
      }
    }
    if (options->c) {
      flag_c_l(options, match_count, filename);
    }

  } else if (!options->s && !file) {
    fprintf(stderr, "%s: %s: No such file or directory\n", "s21_grep",
            filename);
  }
  if (file) fclose(file);
  if (line) free(line);
  regfree(&preg);
}

void flag_f(char *argv, flags *options) {
  char *line = NULL;
  size_t len = 0;
  FILE *file = fopen(argv, "r");
  if (file) {
    while (getline(&line, &len, file) != -1) {
      strcat(options->pattern, line);
      if (options->pattern[strlen(options->pattern) - 1] == '\n' &&
          line[0] != '\n')
        options->pattern[strlen(options->pattern) - 1] = 0;
      strcat(options->pattern, "|");
    }
  }
  if (file) fclose(file);
  if (line) free(line);
}

void flag_c_l(flags *options, int match_count, char *filename) {
  if (options->l) {
    printf("%s\n", filename);
  } else {
    if (options->few_files) {
      if (!options->h) printf("%s:", filename);
    }
    printf("%d\n", match_count);
  }
}
