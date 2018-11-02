
#include "cracker.h"
#include <string.h>

cracker vc_init(char* cipher_text, UnsignedPair freq_table, int min_kl, int max_kl, wchar_t* alphabet, wchar_t* wcipher_text) {
  cracker c;
  c.m_cipher_text = cipher_text;
  c.m_klt = up_init();
  c.m_freq_table = up_init();
  c.MAX_KEY_LENGTH = max_kl;
  c.MIN_KEY_LENGTH = min_kl;
  c.alphabet = alphabet;
  c.m_wcipher_text = wcipher_text;
  return c;
}

void vc_crack(cracker* self) {
  vc_set_klt(self);
  for (int i = 0; i < self -> MAX_KEY_LENGTH; i++) {
    float pair[2] = { 0 };
    up_get_pair(&(self -> m_klt), i, pair);
    char key[(int)pair[0] + 1];
    vc_guess_key(self, key, (int)pair[0]);
    printf("Key Guess: %s\n", key);
    int n = 0;
    int cipher_size = strlen(self -> m_cipher_text);
    int len = 100 > cipher_size ? cipher_size : 100;
    char* decrypted = vg_decrypt(self -> m_cipher_text, key);
    while (n + len <= cipher_size) {
      for (int i = n; i < len; i++)
        printf("%c", decrypted[i]);
      char c;
      c = getchar();
      if (c == 'p' || c == 'P') break;
      n += len;
      len = n + 100 > cipher_size ? cipher_size - n : 100;
      if (len == 0) break;
    }
    printf("\n");
  }
}
int vc_get_coincidences(cracker* self, int offset) {
  int sum = 0;
  for (int i = 0; i < strlen(self -> m_cipher_text) - offset; i++) {
    if (self -> m_cipher_text[i + offset] == self -> m_cipher_text[i]) sum++;
  }
  return sum;
}
void vc_set_klt(cracker* self) {
  for (int i = 1; i <= self -> MAX_KEY_LENGTH % strlen(self -> m_cipher_text); i++) {
    up_set_value(&(self -> m_klt), i, vc_get_coincidences(self, i));
    up_sort(&(self -> m_klt));
  }
}
float vc_score_of_guess(cracker* self, UnsignedPair* cct, int key_length, int offset, int shift) {
  if (shift < 0) shift += 26;
  float error = 0;
  float count = 0;


  for (int i = 0; i < up_size(cct); i++) {
    float pair[2] = { 0 };
    up_get_pair(cct, i, pair);
    char guess = (int) pair[0] - shift;
    if (guess < 'A') guess = 'Z' + 1 - ('A' - guess);
    else if (guess > 'Z') guess = 'A' + (guess - 'Z');
    float observed = up_get_value(cct, pair[0]);
    float expected = up_get_value(&(self -> m_freq_table), guess);
    float a = observed - expected;
    error += a * a / expected;
    count++;
  }

  return error/count;
}
void vc_guess_key(cracker* self, char* output, int key_length) {
  for (int i = 0; i < key_length; i++)
    output[i] = vc_guess_char(self, key_length, i);
  output[key_length] = '\0';
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
char vc_guess_char(cracker* self, int key_length, int offset) {
  UnsignedPair cct = vc_get_cct(self, key_length, offset);

  up_sort(&cct);

  float chi = 1000;
  char output;
  for (int i = 0; i < 15; i++) {
    float pair[2] = { 0 };
    up_get_pair(&cct, i, pair);
    char temp_output = pair[0];
    float sog = vc_score_of_guess(self, &cct, key_length, offset, temp_output - 'E');
    if (sog < chi) {
      chi = sog;
      output = temp_output - 'E';
      if (output < 0) output += 26;
        output += 'A';
      }
  }
  return output;
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
  for (int i = 1; i <= self -> MAX_KEY_LENGTH % wcslen(self -> m_wcipher_text); i++) {
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
  for (int i = 0; i < 15; i++) {
    float pair[2] = { 0 };
    up_get_pair(&cct, i, pair);
    int temp_output_i = wvg_index_of(self -> alphabet, (wchar_t) pair[0]);
    int most_freq_i = 0;
    float sog = vc_score_of_guess(self, &cct, key_length, offset, temp_output_i - most_freq_i);
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
void wvc_crack(cracker* self) {
  wvc_set_klt(self);
  for (int i = 0; i < self -> MAX_KEY_LENGTH; i++) {
    float pair[2] = { 0 };
    up_get_pair(&(self -> m_klt), i, pair);
    wchar_t key[(int)pair[0] + 1];
    wvc_guess_key(self, key, (int)pair[0]);
    printf("Key Guess: ");
    for (int i = 0; i < wcslen(key); i++)
      printf("%lc", key[i]);
    printf("\n");
    int n = 0;
    int cipher_size = wcslen(self -> m_wcipher_text);
    int len = 100 > cipher_size ? cipher_size : 100;
    wchar_t decrypted[wcslen(self -> m_wcipher_text) + 1];
    wvg_decrypt(self->alphabet, self -> m_wcipher_text, key, decrypted);
    while (n + len <= cipher_size) {
      for (int i = n; i < len; i++)
        printf("%lc", decrypted[i]);
      char c;
      c = getchar();
      if (c == 'p' || c == 'P') break;
      n += len;
      len = n + 100 > cipher_size ? cipher_size - n : 100;
      if (len == 0) break;
    }
    printf("\n");
  }
}
