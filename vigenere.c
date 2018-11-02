
#include "vigenere.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

char* vg_get_eliminated_text(char* text, unsigned is_reversed) {
  int count = 0;
  int index = 0;
  for (int i = 0; i < strlen(text); i++) {
    if ((text[i] <= 'Z' && text[i] >= 'A') || (text[i] <= 'z' && text[i] >= 'a'))
      count++;
  }
  char* output = malloc(sizeof(char) * count + 1);

  for (int i = 0; i < strlen(text); i++) {
    if ((text[i] <= 'Z' && text[i] >= 'A') || (text[i] <= 'z' && text[i] >= 'a')) {
      output[index] = text[i];
      index++;
    }
  }
  output[index] = '\0';

  for (int i = 0; i < count; i++) {
    if (output[i] <= 'Z' && output[i] >= 'A')
      output[i] = is_reversed ? (int)'Z' - output[i] + (int)'A' + 1 : output[i];
    else if (output[i] <= 'z' && output[i] >= 'a')
      output[i] = is_reversed ? (int)'Z'-(output[i] - (int)'a' + (int)'A')+(int)'A' + 1 : output[i] - (int)'a' + (int)'A';
  }

  return output;
}

char* vg_shift_text(char* text, char* key, unsigned is_encrypt) {
  char* e_key = vg_get_eliminated_text(key, !is_encrypt);
  char* e_text = vg_get_eliminated_text(text, 0);

  for (int i = 0; i < strlen(e_text); i++) {
    char t = ((e_text[i] - 'A') + (e_key[i%strlen(e_key)] - 'A')) % ('Z' - 'A' + 1) +  'A';
    e_text[i] = t;
  }

  free(e_key);
  return e_text;
}

int wvg_index_of(wchar_t* text, wchar_t c) {
  for (int i = 0; i < wcslen(text); i++) {
    if (text[i] == c) return i;
  }
  return -1;
}

void wvg_shift_text(wchar_t* alphabet, wchar_t* text, wchar_t* key, unsigned is_encrypt, wchar_t* output) {
  for (int i = 0; i < wcslen(text); i++) {
    if (is_encrypt)
      output[i] = alphabet[(wvg_index_of(alphabet, text[i]) + wvg_index_of(alphabet, key[i % wcslen(key)])) % wcslen(alphabet)];
    else {
      int index = wvg_index_of(alphabet, text[i]) - wvg_index_of(alphabet, key[i % wcslen(key)]);
      if (index < 0) index = wcslen(alphabet) + index;
      output[i] = alphabet[index];
    }

  }
}

void wvg_encrypt(wchar_t* alphabet, wchar_t* text, wchar_t* key, wchar_t* output) {
  return wvg_shift_text(alphabet, text, key, 1, output);
}

void wvg_decrypt(wchar_t* alphabet, wchar_t* text, wchar_t* key, wchar_t* output) {
  return wvg_shift_text(alphabet, text, key, 0, output);
}

char* vg_encrypt(char* text, char* key) {
  return vg_shift_text(text, key, 1);
}

char* vg_decrypt(char* text, char* key) {
  return vg_shift_text(text, key, 0);
}
