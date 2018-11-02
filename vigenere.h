#ifndef vigenere_h
#define vigenere_h

#include <stdio.h>
#include <wchar.h>
#include <wctype.h>

char* vg_get_eliminated_text(char* text, unsigned is_reversed);
char* vg_shift_text(char* text, char* key, unsigned is_encrypt);
char* vg_encrypt(char* text, char* key);
char* vg_decrypt(char* text, char* key);
int wvg_index_of(wchar_t* text, wchar_t c);
void wvg_shift_text(wchar_t* alphabet, wchar_t* text, wchar_t* key, unsigned is_encrypt, wchar_t* output);
void wvg_encrypt(wchar_t* alphabet, wchar_t* text, wchar_t* key, wchar_t* output);
void wvg_decrypt(wchar_t* alphabet, wchar_t* text, wchar_t* key, wchar_t* output);

#endif /* vigenere_hpp */
