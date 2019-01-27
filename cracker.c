
#include "cracker.h"
#include <string.h>

cracker vc_init(char* cipher_text, UnsignedPair freq_table, int min_kl, int max_kl, wchar_t* alphabet, wchar_t* wcipher_text) {
  cracker c;
  c.m_cipher_text = cipher_text;
  c.m_klt = up_init();
  c.m_freq_table = freq_table;
  c.MAX_KEY_LENGTH = max_kl;
  c.MIN_KEY_LENGTH = min_kl;
  c.alphabet = alphabet;
  c.m_wcipher_text = wcipher_text;
  // wvc_set_klt(&c);

  wvc_set_klt(&c);
  // printf("[ + ] Initialization done!\n");
  return c;
}

void vc_crack(cracker* self, size_t nth, char* key_output) {
    float pair[2] = { 0 };
    up_get_pair(&(self -> m_klt), nth, pair);
    vc_guess_key(self, key_output, (int)pair[0]);
}

void vc_set_klt(cracker* self) {
  for (int i = self -> MIN_KEY_LENGTH; i <= self -> MAX_KEY_LENGTH % strlen(self -> m_cipher_text); i++) {
    up_set_value(&(self -> m_klt), i, vc_get_coincidences(self, i));
  }
  up_sort(&(self -> m_klt));
}

int vc_get_coincidences(cracker* self, int offset) {
  int sum = 0;
  for (int i = 0; i < strlen(self -> m_cipher_text) - offset; i++) {
    if (self -> m_cipher_text[i + offset] == self -> m_cipher_text[i]) sum++;
  }
  return sum;
}

void vc_guess_key(cracker* self, char* output, int key_length) {
  for (int i = 0; i < key_length; i++)
    output[i] = vc_guess_char(self, key_length, i);
  output[key_length] = '\0';
}

char vc_guess_char(cracker* self, int key_length, int offset) {
  UnsignedPair cct = vc_get_cct(self, key_length, offset);

  up_sort(&cct);

  float chi = 1000;
  char output;
  for (int i = 0; i < 10; i++) {
    float pair[2] = { 0 };
    up_get_pair(&cct, i, pair);
    char temp_output = pair[0];
    float sog = vc_score_of_guess(self, &cct, key_length, offset, temp_output - 'E');
    // printf("SOG: %f CUR CHI: %f\n", sog, chi);
    if (sog < chi) {
      chi = sog;
      output = temp_output - 'E';
      if (output < 0) output += 26;
        output += 'A';
      }
  }
  return output;
}

float vc_score_of_guess(cracker* self, UnsignedPair* cct, int key_length, int offset, int shift) {
  if (shift < 0) shift += 26;
  float error = 0;
  float count = 0;

  for (int i = 0; i < up_size(cct); i++) {
    float pair[2] = { 0 };
    up_get_pair(cct, i, pair);
    char guess = (int) pair[0] - shift;
    // printf("GUESS: %f", (float) guess);
    if (guess < 'A') guess = 'Z' + 1 - ('A' - guess);
    else if (guess > 'Z') guess = 'A' + (guess - 'Z');
    float observed = up_get_value(cct, pair[0]);
    float expected = up_get_value(&(self -> m_freq_table), (float) guess);
    float a = observed - expected;
    error += a * a / expected;
    count++;
  }
  return error/count;
}

UnsignedPair vc_get_cct(cracker* self, int key_length, int offset) {
  UnsignedPair cct = up_init();

  for (int i = offset; i < strlen(self -> m_cipher_text); i = i + key_length) {
    up_sum_value(&cct, self -> m_cipher_text[i], 1);
  }


  float count = up_sum_of_values(&cct);

  for (int i = 0; i < up_size(&cct); i++) {
    float pair[2] = { 0 };
    up_get_pair(&cct, i, pair);
    float key = pair[0];
    up_set_value(&cct, key, up_get_value(&cct, key) / count);
  }

  return cct;
}


// WIDE
int wvc_get_coincidences(cracker* self, int offset) {
  int sum = 0;
  for (int i = 0; i < wcslen(self -> m_wcipher_text) - offset; i++) {
    if (self -> m_wcipher_text[i + offset] == self -> m_wcipher_text[i]) sum++;
  }
  return sum;
}
void wvc_set_klt(cracker* self) {
  for (int i = self -> MIN_KEY_LENGTH; i <= self -> MAX_KEY_LENGTH % wcslen(self -> m_wcipher_text); i++) {
    up_set_value(&(self -> m_klt), i, wvc_get_coincidences(self, i));
    up_sort(&(self -> m_klt));
  }
}
void wvc_guess_key(cracker* self, wchar_t* output, int key_length) {
  for (int i = 0; i < key_length; i++)
    output[i] = wvc_guess_char(self, key_length, i);
  output[key_length] = '\0';
}
wchar_t wvc_guess_char(cracker* self, int key_length, int offset) {
  UnsignedPair cct = wvc_get_cct(self, key_length, offset);

  up_sort(&cct);

  float chi = 1000;
  wchar_t output;
  for (int i = 0; i < 10; i++) {
    float pair[2] = { 0 };
    up_get_pair(&cct, i, pair);
    int temp_output_i = wvg_index_of(self -> alphabet, (wchar_t) pair[0]);
    int most_freq_i = 0;
    float sog = wvc_score_of_guess(self, &cct, key_length, offset, temp_output_i - most_freq_i);
    if (sog < chi) {
      chi = sog;
      output = self -> alphabet[temp_output_i];
    }
  }
  return output;
}
UnsignedPair wvc_get_cct(cracker* self, int key_length, int offset) {
  UnsignedPair cct = up_init();

  for (int i = offset; i < wcslen(self -> m_wcipher_text); i = i + key_length) {
    up_sum_value(&cct, self -> m_wcipher_text[i], 1);
  }

  float count = up_sum_of_values(&cct);

  for (int i = 0; i < up_size(&cct); i++) {
    float pair[2] = { 0 };
    up_get_pair(&cct, i, pair);
    float key = pair[0];
    up_set_value(&cct, key, up_get_value(&cct, key) / count);
  }

  return cct;
}
float wvc_score_of_guess(cracker* self, UnsignedPair* cct, int key_length, int offset, int shift) {
  float error = 0;
  float count = 0;


  for (int i = 0; i < up_size(cct); i++) {
    float pair[2] = { 0 };
    up_get_pair(cct, i, pair);
    int key_i = wvg_index_of(self -> alphabet, (wchar_t) pair[0]);
    int index = key_i - shift;
    if (index < 0) index = wcslen(self->alphabet) + index;
    wchar_t guess = self -> alphabet[index];
    float observed = up_get_value(cct, pair[0]);
    float expected = up_get_value(&(self -> m_freq_table), (float) guess);
    float a = observed - expected;
    error += a * a / expected;
    count++;
  }

  return error/count;
}
void wvc_crack(cracker* self, size_t nth, wchar_t* output) {
  float pair[2] = { 0 };
  up_get_pair(&(self -> m_klt), nth, pair);
  wvc_guess_key(self, output, (int)pair[0]);
}
