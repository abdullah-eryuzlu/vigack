
#include "vigenere.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

void vg_ndecrypt(char *text, char *key, char *output, int start, int n) {
	char noutput[n + 1];
	if (start >= strlen(text)) 
		return;
	
	char *start_p = text + start;
	strncpy(noutput, start_p, n);

	noutput[n] = '\0';
	vg_decrypt(noutput, key, output);
	output[n] = '\0';
}

void vg_decrypt_override(char *text, char *key) {
	vg_decrypt(text, key, text);
}

void wvg_ndecrypt(wchar_t *alphabet, wchar_t *text, wchar_t *key, wchar_t *output, int start, int n) {

	wchar_t noutput[n + 1];
	if (start >= wcslen(text)) 
		return;
	wchar_t *start_p = text + start;
	wcsncpy(noutput, start_p, n);

	noutput[n] = '\0';
	wvg_decrypt(alphabet, noutput, key, output);
	output[n] = '\0';
}

void vg_get_eliminated_text(char* text, unsigned is_reversed, char* output) {
  
	for (int i = 0; i < strlen(text); i++) {
		if (text[i] <= 'Z' && text[i] >= 'A')
      output[i] = is_reversed ? (int)'Z' - text[i] + (int)'A' + 1 : text[i];
    else if (text[i] <= 'z' && text[i] >= 'a')
      output[i] = is_reversed ? 'Z' - (text[i] - 'a' + 'A') + 'A' + 1 
				: text[i] - 'a' + 'A';
  }
}

void vg_shift_text(char* text, char* key, unsigned is_encrypt, char* output) {

  char e_key[strlen(key) + 1]; 
	char e_text[strlen(text) + 1];
  e_key[strlen(key)]   = '\0'; 
	e_text[strlen(text)] = '\0';

  vg_get_eliminated_text(key, !is_encrypt, e_key);
  vg_get_eliminated_text(text, 0, e_text);

  for (int i = 0; i < strlen(e_text); i++) {
    char t = ((e_text[i] - 'A') + (e_key[i%strlen(e_key)] - 'A')) 
			% ('Z' - 'A' + 1) +  'A';
    output[i] = t;
  }
}

int wvg_index_of(const wchar_t *text, wchar_t c) {
  for (int i = 0; i < wcslen(text); i++)
    if (text[i] == c) 
			return i;
  
  return -1;
}

void wvg_shift_text(wchar_t* alphabet, wchar_t* text, wchar_t* key, unsigned is_encrypt, wchar_t* output) {
  for (int i = 0; i < wcslen(text); i++) {
    if (is_encrypt)
      output[i] = alphabet[(wvg_index_of(alphabet, text[i]) + 
					wvg_index_of(alphabet, key[i % wcslen(key)])) % wcslen(alphabet)];
    else {
      int index = wvg_index_of(alphabet, text[i]) - 
				wvg_index_of(alphabet, key[i % wcslen(key)]);
      if (index < 0) 
				index = wcslen(alphabet) + index;
      output[i] = alphabet[index];
    }

  }
}

void wvg_encrypt(wchar_t* alphabet, wchar_t* text, wchar_t* key, wchar_t* output) {
  wvg_shift_text(alphabet, text, key, 1, output);
}

void wvg_decrypt(wchar_t* alphabet, wchar_t* text, wchar_t* key, wchar_t* output) {
  wvg_shift_text(alphabet, text, key, 0, output);
}

void vg_encrypt(char* text, char* key, char* output) {
  vg_shift_text(text, key, 1, output);
}

void vg_decrypt(char* text, char* key, char* output) {
  vg_shift_text(text, key, 0, output);
}
