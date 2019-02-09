
#ifndef cracker_h
#define cracker_h

#include "unsigned_pair.h"
#include "vigenere.h"
#include "argparser.h"

// Char type used for english alphabet

void vc_crack(const param_t *params, size_t key_size, char* key_output);

int vc_get_coincidences(const char *text, int offset);

void vc_set_klt(param_t *params);

float vc_score_of_guess(const param_t *params, UnsignedPair* cct, 
		int key_length, int offset, int shift);

void vc_guess_key(const param_t *params, char* output, int key_length);

UnsignedPair vc_get_cct(const param_t *params, int key_length, int offset);

char vc_guess_char(const param_t *params, int key_length, int offset);


// Wide char type used for non-english alphabets

void wvc_crack(const param_t *params, size_t nth, wchar_t *output);

int wvc_get_coincidences(const wchar_t *wtext, int offset);

void wvc_set_klt(param_t *params);

float wvc_score_of_guess(const param_t *params, UnsignedPair *cct, 
		int key_length, int offset, int shift);

void wvc_guess_key(const param_t *params, wchar_t* output, int key_length);

UnsignedPair wvc_get_cct(const param_t *params, int key_length, int offset);

wchar_t wvc_guess_char(const param_t *params, int key_length, int offset);


#endif /* cracker_h */
