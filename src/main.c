
#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>
#include <wctype.h>
#include <locale.h>
#include "unsigned_pair.h"
#include "vigenere.h"
#include "cracker.h"
// #include "tester.c"
#include <time.h>
#include "argparser.c"
#include "globals.h"

char parse_input() {
  printf("Pass: p, Decrypt n characters: d, Decrypt all: a, exit: e Write to file: f: ");

  char c;
	scanf(" %c", &c);

	return c;
}

void run_nonen(param_t *cfg) {
  wvc_set_klt(cfg);

  float pair[2] = { 0 };

  for (int i = 0; i < up_size(&cfg -> klt); i++) {
    param_t *cfg_temp = cfg;
    up_get_pair(&cfg_temp -> klt, i, pair);

	  wchar_t output[(int) pair[0] + 1];
		wvc_crack(cfg_temp, i, output);
    
		printf("****************************\n");
		printf("KEY: %d: ", i+1);
		
		for (int i = 0; i < wcslen(output); i++) 
			printf("%lc", output[i]);
		printf("\n");

		int counter = 0;

		while (1) {
      char op = parse_input();

		  if ('p' == op)
				break;

			else if ('d' == op) {
				wchar_t decrypted[wcslen(output) + 1];
				int output_len    = wcslen(output);
				int decrypted_len = wcslen(decrypted);

				wvg_ndecrypt(cfg_temp -> alphabet, cfg_temp -> wcipher_text, output, 
						decrypted, counter, output_len);
				counter += output_len;

				for (int i = 0; i < decrypted_len; i++)
					printf("%lc", decrypted[i]);
				printf("\n");

				if (decrypted_len < output_len) 
					break;
			}

      else if ('a' == op) {
        wchar_t decrypted[wcslen(cfg -> wcipher_text) + 1];
        decrypted[wcslen(cfg -> wcipher_text)] = '\0';

				wvg_decrypt(cfg_temp -> alphabet, cfg_temp -> wcipher_text, output, decrypted);
				
				for (int i = 0; i < wcslen(decrypted); i++)
					printf("%lc", decrypted[i]);
				printf("\n");
			}

			else if ('f' == op) {
				wchar_t decrypted[wcslen(cfg -> wcipher_text) + 1];
				decrypted[wcslen(cfg -> wcipher_text)] = '\0';
				
				wvg_decrypt(cfg_temp -> alphabet, cfg_temp -> wcipher_text, output, decrypted);
				
				FILE *fp = fopen(cfg -> destination, "w");
				if (NULL == fp) {
					printf("%s\n", cfg -> destination);
				  fprintf(stderr, COLOR_RED "[ - ] Filename does not exists!\n" COLOR_RESET);	
					continue;
				}
				
				fwprintf(fp, L"KEY: %ls\nPLAIN TEXT: %ls", output, decrypted);
				fclose(fp);
			}

			else if ('e' == op) {
				exit(0);
			}
		}
	}
 
}

void run_en(param_t *cfg) {

  vc_set_klt(cfg);

  float pair[2] = { 0 };

  for (int i = 0; i < up_size(&cfg -> klt); i++) {
    param_t *cfg_temp = cfg;
    up_get_pair(&cfg_temp -> klt, i, pair);

		char output[(int) pair[0] + 1];
		vc_crack(cfg_temp, i, output);
    
		printf("****************************\n");
		printf("KEY %d: %s\n", i + 1, output);

		int counter = 0;

		while (1) {
      char op = parse_input();

		  if ('p' == op)
				break;
			
			else if ('d' == op) {
				int output_len = strlen(output);
				char decrypted[output_len + 1];

				vg_ndecrypt(cfg_temp -> cipher_text, output, 
						decrypted, counter, output_len);
				
				counter += output_len;
		    if (strlen(decrypted) < output_len) 
					break;
        
				printf("%s\n", decrypted);
		  }
      
			else if ('a' == op) {
        char decrypted[strlen(cfg -> cipher_text) + 1];
        decrypted[strlen(cfg -> cipher_text)] = '\0';

				vg_decrypt(cfg_temp -> cipher_text, output, decrypted);
        printf("%s\n", decrypted);
			}
			
			else if ('f' == op) {
				char decrypted[strlen(cfg -> cipher_text) + 1];
				decrypted[strlen(cfg -> cipher_text)] = '\0';

				vg_decrypt(cfg -> cipher_text, output, decrypted);

				FILE *fp = fopen(cfg -> destination, "w");
				if (NULL == fp) {
					printf("%s\n", cfg -> destination);
				  fprintf(stderr, COLOR_RED "[ - ] Filename does not exists!\n" COLOR_RESET);	
					continue;
				}

				fprintf(fp, "KEY: %s\nPLAIN TEXT: %s", output, decrypted);
				fclose(fp);
			}
			
			else if ('e' == op) {
				exit(0);
			}
		}
	}
  free(cfg -> cipher_text);
}

int main(int argc, char const *argv[]) {

  srand(time(NULL));

  setlocale(LC_ALL, "en_US.UTF-8");

	param_t cfg = cfg_init();
	cfg_parse_args(&cfg, argv, argc);
  cfg_parse_keywords(&cfg);

  cfg_parse_alphabet(&cfg);
  cfg_parse_freqs(&cfg);
  cfg_parse_cipher(&cfg);

	if (wcscmp(cfg.keyword, EN) == 0) {
		run_en(&cfg);
	} else {
    run_nonen(&cfg);
  }

	exit(0);
}
