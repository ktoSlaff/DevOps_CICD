#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct short_options {
  int b;
  int e;
  int n;
  int s;
  int t;
  int v;
} sh_opt;

int parser(int argc, char *argv[], sh_opt *short_options);
void reader(int argc, char *argv[], sh_opt *short_options);

int main(int argc, char *argv[]) {
  sh_opt short_options = {0};
  if (parser(argc, argv, &short_options)) reader(argc, argv, &short_options);
  return 0;
}

int parser(int argc, char *argv[], sh_opt *short_options) {
  int index = 0, long_index = 0, res = 1;
  static struct option long_opt[] = {{"number-nonblank", 0, NULL, 'b'},
                                     {"number", 0, NULL, 'n'},
                                     {"squeeze-blank", 0, NULL, 's'},
                                     {NULL, 0, NULL, 0}};
  while ((index = getopt_long(argc, argv, "+benstvTE", long_opt,
                              &long_index)) != -1) {
    switch (index) {
      case 'b':
        short_options->b = 1;
        break;
      case 'e':
        short_options->e = 1;
        short_options->v = 1;
        break;
      case 'n':
        short_options->n = 1;
        break;
      case 's':
        short_options->s = 1;
        break;
      case 't':
        short_options->t = 1;
        short_options->v = 1;
        break;
      case 'v':
        short_options->v = 1;
        break;
      case 'E':
        short_options->e = 1;
        break;
      case 'T':
        short_options->t = 1;
        break;
      default:
        fprintf(stderr, "usage: s21_cat [-benstvTE] [file ...]");
        res = 0;
    }
  }
  return res;
}

void reader(int argc, char *argv[], sh_opt *short_options) {
  const int buffer_size = 4096;
  char buffer[buffer_size];
  if (argc == 1) {
    while (fgets(buffer, buffer_size, stdin)) {
      int length = strlen(buffer);
      buffer[length - 1] = '\0';
      fprintf(stdout, "%s\n", buffer);
    }
  } else {
    while (optind < argc) {
      FILE *file = fopen(argv[optind], "r+");
      if (file) {
        int line_number = 1;
        int blank_lines = 0;
        int last_ch = '\n';
        int ch;
        while ((ch = fgetc(file)) != EOF) {
          if (short_options->s && ch == '\n' && last_ch == '\n') {
            blank_lines++;
            if (blank_lines > 1) {
              continue;
            }
          } else {
            blank_lines = 0;
          }
          if (last_ch == '\n' &&
              ((short_options->b && ch != '\n') ||
               (short_options->n && short_options->b && ch != '\n'))) {
            printf("%6d\t", line_number++);
          }

          if (last_ch == '\n' && short_options->n && !(short_options->b))
            printf("%6d\t", line_number++);
          if (short_options->e && ch == '\n') {
            printf("$");
          }
          if (short_options->t && ch == '\t') {
            printf("^");
            ch = 'I';
          }
          if (short_options->v) {
            if ((ch >= 0 && ch < 9) || (ch > 10 && ch < 32)) {
              printf("^");
              ch += 64;
            }
            if (ch == 127) {
              printf("^");
              ch = '?';
            }
            if (ch > 127 && ch < 160) {
              printf("M-^");
              ch -= 64;
            }
          }
          printf("%c", ch);
          last_ch = ch;
        }
      } else {
        fprintf(stderr, "%s: %s: No such file or directory", argv[0],
                argv[optind]);
      }
      optind++;
      fclose(file);
    }
  }
}
