
#ifndef cracker_h
#define cracker_h

#include "unsigned_pair.h"
#include "vigenere.h"

typedef struct {
  char* m_cipher_text;
  wchar_t* m_wcipher_text;
  wchar_t* alphabet;
  UnsignedPair m_klt;
  UnsignedPair m_freq_table;
  int MAX_KEY_LENGTH;
  int MIN_KEY_LENGTH;
} cracker;

cracker vc_init(char* cipher_text, UnsignedPair freq_table, int min_kl, int max_kl, wchar_t* alphabet, wchar_t* wcipher_text);
void vc_crack(cracker* self, size_t key_size, char* key_output);
int vc_get_coincidences(cracker* self, int offset);
void vc_set_klt(cracker* self);
float vc_score_of_guess(cracker* self, UnsignedPair* cct, int key_length, int offset, int shift);
void vc_guess_key(cracker* self, char* output, int key_length);
UnsignedPair vc_get_cct(cracker* self, int key_length, int offset);
char vc_guess_char(cracker* self, int key_length, int offset);

// WIDE

int wvc_get_coincidences(cracker* self, int offset);
void wvc_set_klt(cracker* self);
float wvc_score_of_guess(cracker* self, UnsignedPair* cct, int key_length, int offset, int shift);
void wvc_guess_key(cracker* self, wchar_t* output, int key_length);
UnsignedPair wvc_get_cct(cracker* self, int key_length, int offset);
wchar_t wvc_guess_char(cracker* self, int key_length, int offset);
void wvc_crack(cracker* self, size_t nth, wchar_t* output);

#endif /* cracker_h */
