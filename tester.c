#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>
#include <wctype.h>
#include <locale.h>
#include "unsigned_pair.h"
#include "vigenere.h"
#include "cracker.h"

typedef struct {
  wchar_t* w_string;
  wchar_t* alphabet;
  char* string;
} tester;

tester* t;
tester_init(wchar_t* w_string, wchar_t* alphabet, char* string) {
  t = malloc(sizeof(tester));
  t -> w_string = w_string;
  t -> string = string;
  t -> alphabet = alphabet;
}


tester_run() {
  wchar_t* w_elim_string;
  char* elim_string;
  int key_lengths[] = { 4, 8, 16, 32, 64 };
  int cipher_lengths[] = { 1024, 2048, 4096, 8192, 16384, 32768, 65536 };
  for (int k_i = 0; i < sizeof(key_lengths) / sizeof(int); i++){
    for (int c_i = 0; i < sizeof(cipher_lengths) / sizeof(int); i++) {
      if (tester -> w_string != 0) {
        w_elim_string = malloc(sizeof(wchar_t) * cipher_lengths[c_i]);
        for (int i = 0; i < cipher_lengths[c_i]; i++) {
          w_elim_string[i] = tester -> w_string[i];
        }
        char w_key[key_lengths[k_i] + 1];
        w_key[key_lengths[k_i]] = '\0';
        for (int i = 0; i < key_lengths[k_i]; i++) {
          w_key[i] = t -> alphabet[rand() % wcslen(alphabet)];
        }

      }
    }
  }
}
