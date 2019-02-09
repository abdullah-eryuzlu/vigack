#ifndef argparser_h
#define argparser_h

typedef struct {
  wchar_t alphabet[100];
  wchar_t keyword[3];
  wchar_t alphabet_kw[3];
  wchar_t freq_kw[3];
  int min_kl;
  int max_kl;
  char destination[250];
  UnsignedPair freq_table;
  UnsignedPair klt;
  char cipher_file[250];
  char *cipher_text;
  wchar_t *wcipher_text;
} param_t;


#endif
