
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <string.h>
#include "unsigned_pair.h"

typedef struct {
  wchar_t alphabet[100];
  wchar_t keyword[3];
  wchar_t alphabet_kw[3];
  wchar_t freq_kw[3];
  int min_kl;
  int max_kl;
  char filepath[250];
  char destination[250];
  UnsignedPair freq_table;
} config_t;


#define DEFAULT_PATH "./src/config.cfg"
const wchar_t* LANGUAGES_KW = L"languages";
const wchar_t* ALPHABETS_KW = L"alphabets";
const wchar_t* FREQ_KW = L"freq_tables";
const wchar_t* END = L"end";

config_t cfg_init() {
  config_t cfg;
  cfg.freq_table = up_init();
  cfg.min_kl = 4;
  cfg.max_kl = 64;
  cfg.keyword[0] = '\0';

  return cfg;
}

void print_usage(const char* prog_name) {
  printf("Usage: %s [<option> ...]\n", prog_name);
  printf("  -c|--cipher-file\tfile that contains cipher text\n");
  printf("  -d|--destination\tdestination file [stdout (interactive)]\n");
  printf("  -m|--min-kl\t\tminimum key length [4]\n");
  printf("  -M|--max-kl\t\tmaximum key length [64]\n");
  printf("  -k|--keyword\t\tlanguage keyword to use [en]\n");
}

void cfg_parse_args(config_t* cfg, const char* argv[], int argc) {
  for (int i = 0; i < argc; i++) {
    if (!strcmp(argv[i], "-c") || !strcmp(argv[i], "--cipher-file")) {
      if (++i < argc) strncpy(cfg -> filepath, argv[i], strlen(argv[i]) % 250);
    }

    else if (!strcmp(argv[i], "-d") || !strcmp(argv[i], "--destination")) {
      if (++i < argc) {
        strncpy(cfg -> destination, argv[i], strlen(argv[i]) % 250);
        cfg->destination[strlen(argv[i]) % 250] = '\0';
      }

    }

    else if (!strcmp(argv[i], "-m") || !strcmp(argv[i], "--min-kl")) {
      if (++i < argc) cfg -> min_kl = atoi(argv[i]);
    }

    else if (!strcmp(argv[i], "-M") || !strcmp(argv[i], "--max-kl")) {
      if (++i < argc) cfg -> max_kl = atoi(argv[i]);
    }

    else if (!strcmp(argv[i], "-k") || !strcmp(argv[i], "--keyword")) {
      if (++i < argc) {
        cfg -> keyword[0] = (wchar_t) argv[i][0];
        cfg -> keyword[1] = (wchar_t) argv[i][1];
        cfg -> keyword[2] = '\0';
      }
    }
  }

  if (!strlen(cfg->filepath) || !wcslen(cfg->keyword)) {
    print_usage(argv[0]);
    exit(-1);
  }
}

int cfg_parse_keywords(config_t* cfg) {
  FILE * fp;
  wchar_t line[120];

  fp = fopen(DEFAULT_PATH, "r");
  if (fp == NULL) {
    exit(-1);
  }

  int is_found = 0;

  while (fgetws(line, 120, fp) != NULL) {
    // fputws(line, stdout);
    if (line[wcslen(line) - 1] == '\n')
      line[wcslen(line) - 1] = '\0';

    if (is_found == 0) {
      if (wcscmp(line, LANGUAGES_KW) == 0)
        is_found = 1;
    }

    if (wcscmp(line, END) == 0)
      break;

    wchar_t* line_p = line;
    if (is_found) {
      if (line[0] == cfg -> keyword[0] && line[1] == cfg -> keyword[1]) {
        line_p += 3;
        wcsncpy(cfg -> alphabet_kw, line_p, 2);
        line_p += 3;
        wcsncpy(cfg -> freq_kw, line_p, 2);
        cfg->alphabet_kw[2] = '\0';
        cfg->freq_kw[2] = '\0';
        break;
      }
    }
  }

  fclose(fp);
  return is_found;
}

int cfg_parse_alphabet(config_t* cfg) {
  FILE * fp;
  wchar_t line[120];

  fp = fopen(DEFAULT_PATH, "r");
  if (fp == NULL) {
    exit(-1);
  }

  int is_found = 0;

  while (fgetws(line, 120, fp) != NULL) {
    // fputws(line, stdout);
    if (line[wcslen(line) - 1] == '\n')
      line[wcslen(line) - 1] = '\0';

    if (is_found == 0) {
      if (wcscmp(line, ALPHABETS_KW) == 0) {
        is_found = 1;
      }
    }

    if (is_found) {
      if (wcscmp(line, END) == 0)
        break;
    }

    wchar_t* line_p = line;
    if (is_found) {
      if (line[0] == cfg -> alphabet_kw[0] && line[1] == cfg -> alphabet_kw[1]) {
        line_p += 3;
        wcsncpy(cfg -> alphabet, line_p, wcslen(line) - 3);
        cfg->alphabet[wcslen(line) - 3] = '\0';
        break;
      }
    }
  }

  fclose(fp);
  return is_found;
}

int cfg_parse_freqs(config_t* cfg) {
  FILE * fp;
  wchar_t line[120];

  fp = fopen(DEFAULT_PATH, "r");
  if (fp == NULL) {
    exit(-1);
  }

  int is_found = 0;
  int kw_found = 0;

  while (fgetws(line, 120, fp) != NULL) {
    // fputws(line, stdout);
    if (line[wcslen(line) - 1] == '\n')
      line[wcslen(line) - 1] = '\0';

    if (is_found == 0) {
      if (wcscmp(line, FREQ_KW) == 0) {
        is_found = 1;
      }
    }

    if (is_found) {
      if (wcscmp(line, END) == 0)
        break;
    }

    if (is_found && !kw_found) {
      if (line[0] == cfg -> alphabet_kw[0] && line[1] == cfg -> alphabet_kw[1]) {
        kw_found = 1;
        continue;
      }
    }

    if (kw_found) {
      wchar_t* line_p = line;
      line_p += 2;
      up_set_value(&cfg -> freq_table, (float) line[0], wcstof(line_p, NULL));
    }
  }

  fclose(fp);
  return is_found;
}
