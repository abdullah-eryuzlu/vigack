#ifndef vigenere_h
#define vigenere_h

#include <stdio.h>
#include <wchar.h>
#include <wctype.h>

void vg_get_eliminated_text(char* text, unsigned is_reversed, char* output);

void vg_shift_text(char* text, char* key, unsigned is_encrypt, char* output);

int wvg_index_of(const wchar_t* text, wchar_t c);

void wvg_shift_text(wchar_t* alphabet, wchar_t* text, wchar_t* key, unsigned is_encrypt, wchar_t* output);

void wvg_encrypt(wchar_t* alphabet, wchar_t* text, wchar_t* key, wchar_t* output);

void wvg_decrypt(wchar_t* alphabet, wchar_t* text, wchar_t* key, wchar_t* output);

void vg_encrypt(char* text, char* key, char* output);

void vg_decrypt(char* text, char* key, char* output);

void vg_ndecrypt(char *text, char *key, char *output, int start, int n);

void wvg_ndecrypt(wchar_t *alphabet, wchar_t *text, wchar_t *key, wchar_t *output, int start, int n);

#endif /* vigenere_h */
