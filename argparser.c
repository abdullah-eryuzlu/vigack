

#include <stdio.h>
#include <stdlib.h>
#include "unsigned_pair.h"
#include <string.h>

#define default_fname "./src/config.cfg"

typedef struct {
  wchar_t* alphabet;
  UnsignedPair freq_table;
  char* filename;
  char* keyword;
} config_t;

config_t cfg_init(char* filename, char* keyword) {
  config_t config;
  config.freq_table = up_init();
  if (filename == "")
    config.filename = default_fname;
  else
    config.filename = filename;
  config.keyword = keyword;

  return config;
}

void cfg_parse(config_t* self) {
  FILE* fp;
  char* line;
  size_t len = 0;

  fp = fopen(self -> filename, "r");

  char kw [20];

  int mode = 0;
  unsigned found_ft = 0;
  char alp[3];
  char ft[3];

  while (getline(&line, &len, fp) != -1) {
    printf("%s", line);
    printf("%d\n", mode);
    if (line == "\n" || line == "#\n") continue;
    if (mode == 0) {
      if (strstr(line, "languages") != NULL)
        mode = 1;
      else if (strstr(line, "alphabets") != NULL)
        mode = 2;
      else if (strstr(line, "freq_tables") != NULL) {
        mode = 3;
      }
      continue;
    }
    else if (mode == 1) {
      char kw[3];

      sscanf(line, "%s %s %s", kw, alp, ft);
      if (strstr(self -> keyword, kw) != NULL) mode = 0;
    }
    else if (mode == 2) {
      char a[3];
      char alph[40];
      if (strstr(alp, "en")) mode = 0;
      else{
        printf("%s\n", kw);
        exit(0);
        if (strstr(alp, kw)) {
          sscanf(line, "%s %s", a, alph);
          printf("%s\n", alph);
          exit(0);
          for (int i = 0; i < strlen(alph); i++) {
            self -> alphabet[i] = (wchar_t) alph[i];
          }
          mode = 0;
        }
      }
    }
    else if (mode == 3) {
      char formatted[5];
      sprintf(formatted, "%s:\n", ft);
      if (strstr(line, formatted)) {
        found_ft = 1;
        continue;
      }
      if (found_ft) {
        if (strstr(line, ";") != NULL) {
          mode = 0;
          continue;
        }
        char key;
        float value;
        sscanf(line, "%c %f", &key, &value);
        up_set_value(&(self -> freq_table), key, value);
      }
    }
  }
  fclose(fp);
}
