#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>
#include <wctype.h>
#include <locale.h>
#include "unsigned_pair.h"
#include "vigenere.h"
#include "cracker.h"
#include <sys/time.h>
#include <string.h>

#define KEY_SIZE_COUNT 5
#define TEXT_SIZE_COUNT 8
#define MIN_KL 4
#define MAX_KL 64

const int const key_sizes[] = { 4, 8, 16, 32, 64 };
const int const text_sizes[] = { 1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072 };

typedef struct {
  wchar_t* w_string;
  wchar_t* alphabet;
  char* string;
  unsigned t_type;
} tester;

tester tester_init(char* text) {
  tester t;
  t.t_type = 0;
  t.string = text;

  return t;
}

void tester_gen_key(size_t size, char* output) {
  for (int i = 0; i < size; i++) {
    output[i] = (char) (65 + rand() % 26);
  }
  output[size] = '\0';
}

UnsignedPair tester_set_ft() {
  UnsignedPair freq_table = up_init();
  up_set_value(&freq_table, (float) 'A', 0.082);
  up_set_value(&freq_table, (float) 'B', 0.015);
  up_set_value(&freq_table, (float) 'C', 0.028);
  up_set_value(&freq_table, (float) 'D', 0.043);
  up_set_value(&freq_table, (float) 'E', 0.127);
  up_set_value(&freq_table, (float) 'F', 0.022);
  up_set_value(&freq_table, (float) 'G', 0.020);
  up_set_value(&freq_table, (float) 'H', 0.061);
  up_set_value(&freq_table, (float) 'I', 0.070);
  up_set_value(&freq_table, (float) 'J', 0.002);
  up_set_value(&freq_table, (float) 'K', 0.008);
  up_set_value(&freq_table, (float) 'L', 0.040);
  up_set_value(&freq_table, (float) 'M', 0.024);
  up_set_value(&freq_table, (float) 'N', 0.067);
  up_set_value(&freq_table, (float) 'O', 0.075);
  up_set_value(&freq_table, (float) 'P', 0.019);
  up_set_value(&freq_table, (float) 'Q', 0.001);
  up_set_value(&freq_table, (float) 'R', 0.060);
  up_set_value(&freq_table, (float) 'S', 0.063);
  up_set_value(&freq_table, (float) 'T', 0.091);
  up_set_value(&freq_table, (float) 'U', 0.028);
  up_set_value(&freq_table, (float) 'V', 0.010);
  up_set_value(&freq_table, (float) 'W', 0.023);
  up_set_value(&freq_table, (float) 'X', 0.001);
  up_set_value(&freq_table, (float) 'Y', 0.020);
  up_set_value(&freq_table, (float) 'Z', 0.001);

  return freq_table;
}

void tester_run(tester *t) {
  for (int text_i = 0; text_i < TEXT_SIZE_COUNT; text_i++) {
    for (int key_i = 0; key_i < KEY_SIZE_COUNT; key_i++) {
      float succ_counter = 0;
      for (int s_c = 0; s_c < 1; s_c++) {
        struct timeval stop, start;

        //do stuff
        // printf("took %lu\n", stop.tv_usec - start.tv_usec);
        // printf("[ + ] - Cracking KS: %d TS: %d\n", key_sizes[key_i], text_sizes[text_i]);
        // Generate Key
        int key_size = key_sizes[key_i];
        char key[key_size+1];
        tester_gen_key(key_size, key);
        // End Generate Key

        // Generate Frequency Table
        UnsignedPair freq_table = tester_set_ft();
        // up_log(&freq_table);
        // End Generate Freq

        // Generate text
        int text_size = text_sizes[text_i];
        char text[text_size + 1];
        for (int i = 0; i < strlen(t -> string); i++) {
          if (i == text_size) break;
          text[i] = t -> string[i];
        }
        text[text_size] = '\0';
        char encrypted[text_size + 1];
        vg_encrypt(text, key, encrypted);
        // End generate text

        gettimeofday(&start, NULL);
        cracker vc = vc_init(encrypted, freq_table, MIN_KL, MAX_KL, L"", L"");
        for (int i = 0; i < up_size(&vc.m_klt); i++) {
          succ_counter++;
          float pair[2] = { 0 };
          up_get_pair(&vc.m_klt, i, pair);
          char cracked_key[(int) pair[0] + 1];
          vc_crack(&vc, i, cracked_key);
          if (key_size == pair[0]) {
            int counter = 0;
            for (int i = 0; i < strlen(cracked_key); i++) {
              if (cracked_key[i] == key[i]) counter++;
            }
            // gettimeofday(&stop, NULL);
            // succ_counter +=  (stop.tv_sec - start.tv_sec) * 1000;
            // succ_counter += (stop.tv_usec - start.tv_usec) / 1000;
            // printf("%d %d %f\n", succ_counter);
            break;
            // printf("KEY: %s CK: %s\n", key, cracked_key);
          }
        }
      }
      // T1:
      // printf("%d %d %.2f\n", key_sizes[key_i], text_sizes[text_i], succ_counter / 10);
      // T2:
      printf("%d %d %.2f\n", key_sizes[key_i], text_sizes[text_i], succ_counter);
    }
  }
}

tester wtester_init(wchar_t *text, wchar_t *alphabet) {
  tester t;
  t.w_string = text;
  t.alphabet = alphabet;

  return t;
}

void wtester_gen_key(tester *t, size_t size, wchar_t* output) {
  for (int i = 0; i < size; i++) {
    output[i] = t -> alphabet[rand() % 29];
  }
  output[size] = '\0';
}

UnsignedPair wtester_set_ft() {

  UnsignedPair freq_table = up_init();

  up_set_value(&freq_table, (float) L'a', 0.118);
  up_set_value(&freq_table, (float) L'b', 0.03);
  up_set_value(&freq_table, (float) L'c', 0.01);
  up_set_value(&freq_table, (float) L'ç', 0.012);
  up_set_value(&freq_table, (float) L'd', 0.05);
  up_set_value(&freq_table, (float) L'e', 0.091);
  up_set_value(&freq_table, (float) L'f', 0.004);
  up_set_value(&freq_table, (float) L'g', 0.014);
  up_set_value(&freq_table, (float) L'ğ', 0.011);
  up_set_value(&freq_table, (float) L'h', 0.01);
  up_set_value(&freq_table, (float) L'ı', 0.053);
  up_set_value(&freq_table, (float) L'i', 0.083);
  up_set_value(&freq_table, (float) L'j', 0.001);
  up_set_value(&freq_table, (float) L'k', 0.052);
  up_set_value(&freq_table, (float) L'l', 0.006);
  up_set_value(&freq_table, (float) L'm', 0.041);
  up_set_value(&freq_table, (float) L'n', 0.078);
  up_set_value(&freq_table, (float) L'o', 0.03);
  up_set_value(&freq_table, (float) L'ö', 0.01);
  up_set_value(&freq_table, (float) L'p', 0.009);
  up_set_value(&freq_table, (float) L'r', 0.077);
  up_set_value(&freq_table, (float) L's', 0.038);
  up_set_value(&freq_table, (float) L'ş', 0.028);
  up_set_value(&freq_table, (float) L't', 0.039);
  up_set_value(&freq_table, (float) L'u', 0.041);
  up_set_value(&freq_table, (float) L'ü', 0.029);
  up_set_value(&freq_table, (float) L'v', 0.012);
  up_set_value(&freq_table, (float) L'y', 0.041);
  up_set_value(&freq_table, (float) L'z', 0.02);

  return freq_table;
}

void wtester_run(tester *t) {
  for (int text_i = 0; text_i < TEXT_SIZE_COUNT; text_i++) {
    for (int key_i = 0; key_i < KEY_SIZE_COUNT; key_i++) {
      float succ_counter = 0;
      for (int s_c = 0; s_c < 10; s_c++) {
        struct timeval stop, start;


        // Generate Key
        int key_size = key_sizes[key_i];
        wchar_t key[key_size+1];
        wtester_gen_key(t, key_size, key);
        // End Generate Key

        // Generate Frequency Table
        UnsignedPair freq_table = wtester_set_ft();
        // up_log(&freq_table);
        // End Generate Freq

        // Generate text



        int text_size = text_sizes[text_i];
        wchar_t text[text_size + 1];
        for (int i = 0; i < wcslen(t -> w_string); i++) {
          if (i == text_size) break;
          text[i] = t -> w_string[i];
        }
        text[text_size] = '\0';
        wchar_t encrypted[text_size + 1];
        encrypted[text_size] = '\0';
        wvg_encrypt(t -> alphabet, text, key, encrypted);
        // End generate text

        gettimeofday(&start, NULL);
        cracker vc = vc_init("", freq_table, MIN_KL, MAX_KL, t -> alphabet, encrypted);
        // exit(0);
        for (int i = 0; i < up_size(&vc.m_klt); i++) {
          // succ_counter++;
          float pair[2] = { 0 };
          up_get_pair(&vc.m_klt, i, pair);
          wchar_t cracked_key[(int) pair[0] + 1];
          wvc_crack(&vc, i, cracked_key);
          if (key_size == pair[0]) {
            int counter = 0;
            for (int i = 0; i < wcslen(cracked_key); i++) {
              if (cracked_key[i] == key[i]) counter++;
            }
            // gettimeofday(&stop, NULL);
            // succ_counter +=  (stop.tv_sec - start.tv_sec) * 1000;
            // succ_counter += (stop.tv_usec - start.tv_usec) / 1000;
            // printf("%d %d %f\n", succ_counter);
            // printf("%d %d %d - ", key_sizes[key_i], text_sizes[text_i], counter);
            //
            // for (int i = 0; i < wcslen(cracked_key); i++)
            //   printf("%lc", cracked_key[i]);
            // printf(" ");
            // for (int i = 0; i < wcslen(key); i++)
            // printf("%lc", key[i]);
            // printf("\n");
            //
            // printf("KEY: %s CK: %s\n", key, cracked_key);
            succ_counter += (float) counter / wcslen(key);
            // printf("Succ %d\n", succ_counter);
            // exit(0);
            break;
          }
        }
        // exit(0);
      }
      // T1:
      printf("%d %d %.2f\n", key_sizes[key_i], text_sizes[text_i], succ_counter * 10);
      // T2:
    }
  }
}
