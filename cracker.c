#include "cracker.h"
#include <string.h>

void vc_crack(const param_t *params, size_t nth, char *key_output) {
    float pair[2] = { 0 };
    up_get_pair(&(params -> klt), nth, pair);
    vc_guess_key(params, key_output, (int)pair[0]);
		key_output[strlen(key_output)] = '\0';
}

void vc_set_klt(param_t *params) {
  int i      = params -> min_kl;
  char *text = params -> cipher_text;

  for (; i <= params -> max_kl % strlen(text); i++)
      up_set_value(&params -> klt, i, vc_get_coincidences(text, i));

  up_sort(&params -> klt);
}

int vc_get_coincidences(const char *text, int offset) {
  int sum = 0;

  for (int i = 0; i < strlen(text) - offset; i++)
    if (text[i + offset] == text[i]) sum++;

  return sum;
}

void vc_guess_key(const param_t *params, char *output, int key_length) {

  for (int i = 0; i < key_length; i++)
    output[i] = vc_guess_char(params, key_length, i);
  output[key_length] = '\0';
}

char vc_guess_char(const param_t *params, int key_length, int offset) {

  UnsignedPair cct = vc_get_cct(params, key_length, offset);
  up_sort(&cct);

	char output;
  float chi     = 1000; 
  float pair[2] = { 0 };

  for (int i = 0; i < 10; i++) {
    up_get_pair(&cct, i, pair);
    char temp_output = pair[0];
    float sog = vc_score_of_guess(params, &cct, key_length, offset, temp_output - 'E');
    if (sog < chi) {
      chi = sog;
      output = temp_output - 'E';
      if (output < 0) 
				output += 26;
      output += 'A';
    }
  }
  return output;
}

float vc_score_of_guess(const param_t *params, UnsignedPair *cct, int key_length,
  int offset, int shift) {

  if (shift < 0) 
		shift += 26;

  float error = 0;
  float count = 0;

  for (int i = 0; i < up_size(cct); i++) {

    float pair[2] = { 0 };
    up_get_pair(cct, i, pair);
    char guess = (int) pair[0] - shift;

    if (guess < 'A') 
			guess = 'Z' + 1 - ('A' - guess);
    else if (guess > 'Z')
			guess = 'A' + (guess - 'Z');

    float observed = up_get_value(cct, pair[0]);
    float expected = up_get_value(&(params -> freq_table), (float) guess);
    float x        = observed - expected;

    error += x * x / expected;
    count++;
  }
  return error/count;
}

UnsignedPair vc_get_cct(const param_t *params, int key_length, int offset) {

  UnsignedPair cct = up_init();

  for (int i = offset; i < strlen(params -> cipher_text); i += key_length)
    up_sum_value(&cct, params -> cipher_text[i], 1);

  float total   = up_sum_of_values(&cct);
  float pair[2] = { 0 };

  for (int i = 0; i < up_size(&cct); i++) {
    up_get_pair(&cct, i, pair);
    up_set_value(&cct, pair[0], up_get_value(&cct, pair[0]) / total);
  }

  return cct;
}


// WIDE

void wvc_crack(const param_t *params, size_t nth, wchar_t* output) {

  float pair[2] = { 0 };

  up_get_pair(&params -> klt, nth, pair);
  wvc_guess_key(params, output, (int)pair[0]);
}

int wvc_get_coincidences(const wchar_t *wtext, int offset) {

  int sum = 0;

  for (int i = 0; i < wcslen(wtext) - offset; i++)
    if (wtext[i + offset] == wtext[i])
      sum++;

  return sum;
}
// No need for wide spec
void wvc_set_klt(param_t* params) {

  int i          = params -> min_kl;
  wchar_t *wtext = params -> wcipher_text;

  for (; i <= params -> max_kl % wcslen(wtext); i++)
    up_set_value(&params -> klt, i, wvc_get_coincidences(wtext, i));

  up_sort(&params -> klt);
}

void wvc_guess_key(const param_t *params, wchar_t *output, int key_length) {

  for (int i = 0; i < key_length; i++)
    output[i] = wvc_guess_char(params, key_length, i);
  output[key_length] = '\0';
}

wchar_t wvc_guess_char(const param_t* params, int key_length, int offset) {

  UnsignedPair cct = wvc_get_cct(params, key_length, offset);
  up_sort(&cct);

  wchar_t output;
  float chi = 1000;
  
  for (int i = 0; i < 10; i++) {
    float pair[2] = { 0 };
    up_get_pair(&cct, i, pair);

    int temp_output_i = wvg_index_of(params -> alphabet, (wchar_t) pair[0]);
    int most_freq_i   = 0;
    float sog         = wvc_score_of_guess(params, &cct, key_length, offset,
				temp_output_i - most_freq_i);

    if (sog < chi) {
      chi = sog;
      output = params -> alphabet[temp_output_i];
    }
  }
  return output;
}
UnsignedPair wvc_get_cct(const param_t *params, int key_length, int offset) {

  UnsignedPair cct = up_init();

  for (int i = offset; i < wcslen(params -> wcipher_text); i += key_length)
    up_sum_value(&cct, params -> wcipher_text[i], 1);

  float count = up_sum_of_values(&cct);

  for (int i = 0; i < up_size(&cct); i++) {
    float pair[2] = { 0 };
    up_get_pair(&cct, i, pair);

    float key = pair[0];
    up_set_value(&cct, key, up_get_value(&cct, key) / count);
  }

  return cct;
}
float wvc_score_of_guess(const param_t *params, UnsignedPair *cct, int key_length,
  int offset, int shift) {

  float error = 0;
  float count = 0;

  for (int i = 0; i < up_size(cct); i++) {
    float pair[2] = { 0 };
    up_get_pair(cct, i, pair);

    int key_i = wvg_index_of(params -> alphabet, (wchar_t) pair[0]);
    int index = key_i - shift;
    if (index < 0) 
			index = wcslen(params->alphabet) + index;

    wchar_t guess = params -> alphabet[index];
    float observed = up_get_value(cct, pair[0]);
    float expected = up_get_value(&params -> freq_table, (float) guess);
    float x = observed - expected;

    error += x * x / expected;
    count++;
  }

  return error/count;
}

