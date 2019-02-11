#ifndef argparser_h
#define argparser_h

#include "globals.h"

typedef struct {
  wchar_t alphabet[ALP_MAX];
  wchar_t keyword[KW_LEN];
  wchar_t alphabet_kw[KW_LEN];
  wchar_t freq_kw[KW_LEN];
  int min_kl;
  int max_kl;
  char destination[FILE_MAX];
  UnsignedPair freq_table;
  UnsignedPair klt;
  char cipher_file[FILE_MAX];
  char *cipher_text;
  wchar_t *wcipher_text;
} param_t;


#endif
