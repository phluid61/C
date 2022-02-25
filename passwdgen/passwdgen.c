/*
 * passwdgen.c
 *
 * Generates a pseudo-my_rand password.
 *
 * Author:  Matthew Kerwin <matthew@kerwin.net.au>
 *
 * Copyright (C) 2009-2016 Matthew Kerwin. All Rights Reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#ifdef USE_RAND
#  if 0
#    define my_srand  srand
#    define my_rand_  rand
#    define my_RMAX   RAND_MAX
#  else
#    define my_srand  srandom
#    define my_rand_  random
#    define my_RMAX   2147483647
#  endif
#else
	/*  Written in 2019 by David Blackman and Sebastiano Vigna (vigna@acm.org)
	 *
	 * To the extent possible under law, the author has dedicated all copyright
	 * and related and neighboring rights to this software to the public domain
	 * worldwide. This software is distributed without any warranty.
	 *
	 * See <http://creativecommons.org/publicdomain/zero/1.0/>. */

	#include <stdint.h>

	/* This is xoshiro256++ 1.0, one of our all-purpose, rock-solid generators.
	 * It has excellent (sub-ns) speed, a state (256 bits) that is large
	 * enough for any parallel application, and it passes all tests we are
	 * aware of.
	 *
	 * For generating just floating-point numbers, xoshiro256+ is even faster.
	 *
	 * The state must be seeded so that it is not everywhere zero. If you have
	 * a 64-bit seed, we suggest to seed a splitmix64 generator and use its
	 * output to fill s. */

	uint64_t __rotl(const uint64_t x, int k) {
		return (x << k) | (x >> (64 - k));
	}

	uint64_t __s[4];

	uint64_t __next(void) {
		const uint64_t result = __rotl(__s[0] + __s[3], 23) + __s[0];

		const uint64_t t = __s[1] << 17;

		__s[2] ^= __s[0];
		__s[3] ^= __s[1];
		__s[1] ^= __s[2];
		__s[0] ^= __s[3];

		__s[2] ^= t;

		__s[3] = __rotl(__s[3], 45);

		return result;
	}
	/* END */

	#define my_RMAX 0x7fffffff

	long my_rand_() {
		uint64_t n = __next();
		return (long)(n & my_RMAX);
	}

	int my_srand(unsigned int seed) {
		uint64_t s = (uint64_t)seed;
		__s[0] = (s & 0xf0f0f0f0) | ((s & 0x0f0f0f0f) << 32);
		__s[2] = (s & 0x0f0f0f0f) | ((s & 0xf0f0f0f0) << 32);
		__s[1] = __rotl(__s[0], 17);
		__s[3] = __rotl(__s[2], 45);
		return (int)my_rand_();
	}

#endif

int my_rand(int n) {
	int limit;
	int r;

	limit = my_RMAX - (my_RMAX % n);

	/* if the chosen number isn't in the range that divides 'n' evenly, discard it */
	while ((r = my_rand_()) >= limit) ;

	return r % n;
}


#define BOLD   "\x1B[1m"
#define NORMAL "\x1B[0m"

const char* app_name = "passwdgen";
const char* version  = "1.1";

#define NO    0
#define YES   1
#define FORCE 2

const char UPPER[27] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const char LOWER[27] = "abcdefghijklmnopqrstuvwxyz";
const char NUMER[11] = "0123456789";
const char UPP_P[26] = "ABCDEFGHIJKLMNPQRSTUVWXYZ";
const char LOW_P[26] = "abcdefghijkmnopqrstuvwxyz";
const char NUM_P[9]  = "23456789";
const char ASCII[33] = "`-=~!@#$%^&*()_+[]\\{}|;\':\",./<>?";

/* OPTIONS */
int repetitions = 5;
int min_length = 16;
int max_length = 32;
int upper = FORCE;
int lower = FORCE;
int numer = FORCE;
int ascii = NO;
int printable = YES;

char *custom_chars   = 0;
int   custom_chars_n = 0;

void parse_command_line(int argc, char *argv[]);
void permute(char* string, int length);

int main(int argc, char *argv[]) {

	char *passwd = 0;
	int   length = 0;

	int has_upper = 0;
	int has_lower = 0;
	int has_numer = 0;
	int has_ascii = 0;

	int try_upper = 0;
	int try_lower = 0;
	int try_numer = 0;
	int try_ascii = 0;

	int i, j;

	parse_command_line(argc, argv);

	if ((upper | lower | numer | ascii | custom_chars_n) == 0) {
		fprintf(stderr, "WARNING:  all characters disallowed.  Using default sets.\n");
		upper = FORCE;
		lower = FORCE;
		numer = FORCE;
		ascii = NO;
	}

	my_srand(time(0));

	for (j = 0; j < repetitions; j++) {
		if (min_length == max_length) { length = max_length; }
		else { length = my_rand(max_length - min_length) + min_length; }
		passwd = (char*) malloc(length + 1);
		has_upper = 0;
		has_lower = 0;
		has_numer = 0;
		has_ascii = 0;
		for (i = 0; i < length; i++) {
			if (custom_chars_n > 0) {
				passwd[i] = custom_chars[my_rand(custom_chars_n)];
			} else {
				if (upper == FORCE && !has_upper) { try_upper = 1; }
				if (lower == FORCE && !has_lower) { try_lower = 2; }
				if (numer == FORCE && !has_numer) { try_numer = 4; }
				if (ascii == FORCE && !has_ascii) { try_ascii = 8; }
				if ((try_upper | try_lower | try_numer | try_ascii) == 0) {
					try_upper = (upper != NO) ? 1 : 0;
					try_lower = (lower != NO) ? 2 : 0;
					try_numer = (numer != NO) ? 4 : 0;
					try_ascii = (ascii != NO) ? 8 : 0;
				}
				do {
					switch (my_rand(4)) {
						case 0:
							if (try_upper) {
								if (printable != NO) { passwd[i] = UPP_P[my_rand(25)]; }
								else                 { passwd[i] = UPPER[my_rand(26)]; }
								has_upper = 1;
								goto ENDWHILE;
							}
							break;
						case 1:
							if (try_lower) {
								if (printable != NO) { passwd[i] = LOW_P[my_rand(25)]; }
								else                 { passwd[i] = LOWER[my_rand(26)]; }
								has_lower = 1;
								goto ENDWHILE;
							}
							break;
						case 2:
							if (try_numer) {
								if (printable != NO) { passwd[i] = NUM_P[my_rand(8)]; }
								else                 { passwd[i] = NUMER[my_rand(10)]; }
								has_numer = 1;
								goto ENDWHILE;
							}
							break;
						case 3:
							if (try_ascii) {
								passwd[i] = ASCII[my_rand(32)];
								has_ascii = 1;
								goto ENDWHILE;
							}
							break;
						default:
							break;
					}
				} while (1);
ENDWHILE:
				;
			}
		}
		passwd[length] = 0;
		permute(passwd, length);
		printf("%s\n", passwd);
		free(passwd);
	}

	return 0;
}

void show_version(void) {
	printf("%s (v%s)\n", app_name, version);
}

void show_help(void) {
	printf("%sVERSION:%s\n", BOLD, NORMAL);
	show_version();
	printf("\n");
	printf("%sOPTIONS:%s\n", BOLD, NORMAL);
	printf("\n");
	printf(" %s-C%s #  %s-c%s #  %s--count%s=#\n   generate a fixed number of passwords.  Default is 5.\n", BOLD, NORMAL, BOLD, NORMAL, BOLD, NORMAL);
	printf("\n");
	printf(" %s-N%s #  %s-n%s #  %s--min%s=#\n %s-X%s #  %s-x%s #  %s--max%s=#\n", BOLD, NORMAL, BOLD, NORMAL, BOLD, NORMAL, BOLD, NORMAL, BOLD, NORMAL, BOLD, NORMAL);
	printf("   passwords have the given minimum/maximum length.  Defaults are 16, 32.\n");
	printf("\n");
	printf(" %s--%s [CHARACTERS]\n   passwords are built using precisely the given set of characters.\n", BOLD, NORMAL);
	printf("\n");
	printf(" %s[+-=]U  [+-=]u  --upper%s=[YES|NO|FORCE]\n", BOLD, NORMAL);
	printf(" %s[+-=]L  [+-=]l  --lower%s=[YES|NO|FORCE]\n", BOLD, NORMAL);
	printf(" %s[+-=]D  [+-=]d  --digits%s=[YES|NO|FORCE]\n", BOLD, NORMAL);
	printf(" %s[+-=]A  [+-=]a  --ascii%s=[YES|NO|FORCE]\n", BOLD, NORMAL);
	printf("   accept/deny/force upper case characters, lower case characters, decimal digits,\n");
	printf("   and non-alphanumeric ASCII characters (respectively) in passwords.\n");
	printf("   Defaults are: %s=ULD-A%s\n", BOLD, NORMAL);
	printf("   Ignored if %s-- characters%s is specified.\n", BOLD, NORMAL);
	printf("\n");
	printf(" %s[+-]P  [+-]p  --printable%s=[YES|NO]\n", BOLD, NORMAL);
	printf("   only generate printable passwords (no 1,l,O,0 characters).  Default = %sYES%s\n", BOLD, NORMAL);
	printf("   Ignored if %s-- characters%s is specified.\n", BOLD, NORMAL);
	printf("\n");
	printf(" %s-V%s  %s-v%s  %s--version%s\n   show version information, and quit.\n", BOLD, NORMAL, BOLD, NORMAL, BOLD, NORMAL);
	printf("\n");
	printf(" %s-?%s  %s--help%s\n   show this help information, and quit.\n", BOLD, NORMAL, BOLD, NORMAL);
}

void bad_parameter(const char* param) {
	fprintf(stderr, "Unrecognised parameter: %s\n", param);
	exit(1);
}

void bad_parameter_token(const char* param, char token) {
	fprintf(stderr, "Unrecognised token %c in parameter: %s\n", token, param);
	exit(2);
}

void parameter_missing_argument(const char* param) {
	fprintf(stderr, "Parameter missing argument: %s\n", param);
	exit(3);
}

int starts_with(const char* long_str, const char* short_str) {
	char *sp = (char*)short_str;
	char *lp = (char*)long_str;
	do {
		if (*sp == *lp) {
			++ sp;
			++ lp;
		} else {
			return 0;
		}
	} while (*sp != 0);
	return 1;
}

void get_number(int* var, const char* arg, const char* message) {
	int n = atoi(arg);
	if (n < 1) {
		fprintf(stderr, "Invalid %s: %s\n", message, arg);
		exit(4);
	}
	*var = n;
}

void get_accept(int* var, const char* arg, const char* message) {
	if (strcmp(arg, "NO" )   == 0) {*var = NO;    return;}
	if (strcmp(arg, "YES")   == 0) {*var = YES;   return;}
	if (strcmp(arg, "FORCE") == 0) {*var = FORCE; return;}
	if (strcmp(arg, "no" )   == 0) {*var = NO;    return;}
	if (strcmp(arg, "yes")   == 0) {*var = YES;   return;}
	if (strcmp(arg, "force") == 0) {*var = FORCE; return;}
	fprintf(stderr, "Invalid %s: %s\nExpected YES, NO, or FORCE\n", message, arg);
	exit(5);
}

void get_boolean(int* var, const char* arg, const char* message) {
	if (strcmp(arg, "NO" )   == 0) {*var = 0; return;}
	if (strcmp(arg, "YES")   == 0) {*var = 1; return;}
	if (strcmp(arg, "no" )   == 0) {*var = 0; return;}
	if (strcmp(arg, "yes")   == 0) {*var = 1; return;}
	fprintf(stderr, "Invalid %s: %s\nExpected YES, NO\n", message, arg);
	exit(6);
}

void get_custom_chars(const char* arg, const char* message) {
	int n;
	if (custom_chars != 0) {
		free(custom_chars);
	}
	n = strlen(arg);
	if (n != 0) {
		custom_chars_n = n;
		custom_chars   = (char*) malloc(custom_chars_n);
		strcpy(custom_chars, arg);
	} else {
		fprintf(stderr, "WARNING: blank %s specified.  Using default.\n", message);
	}
}

static void consume_chained_params(const char *arg, const char *original_arg, int accept) {
	char *ptr = (char*) arg;
	int got_chars = 0;
	for (; *ptr != 0; ptr ++) {
		switch (*ptr) {
			case 'U':
			case 'u':
				upper = accept;
				break;
			case 'L':
			case 'l':
				lower = accept;
				break;
			case 'D':
			case 'd':
				numer = accept;
				break;
			case 'A':
			case 'a':
				ascii = accept;
				break;
			case 'P':
			case 'p':
				if (accept == FORCE) {
					bad_parameter_token(original_arg, *ptr);
				} else {
					printable = accept;
				}
				break;
			case '+':
				if (got_chars == 0) {
					bad_parameter(original_arg);
				} else {
					consume_chained_params(ptr + 1, original_arg, YES);
				}
				return;
			case '=':
				if (got_chars == 0) {
					bad_parameter(original_arg);
				} else {
					consume_chained_params(ptr + 1, original_arg, FORCE);
				}
				return;
			case '-':
				if (got_chars == 0) {
					bad_parameter(original_arg);
				} else {
					consume_chained_params(ptr + 1, original_arg, NO);
				}
				return;
			default:
				bad_parameter_token(original_arg, *ptr);
		}
		got_chars = 1;
	}
}

void parse_command_line(int argc, char *argv[]) {
	int debug = 0;
	int i,j;

	i = 1;
	while (i < argc) {
		switch (argv[i][0]) {
			case '+':
				consume_chained_params(argv[i] + 1, argv[i], YES);
				goto NEXT_ARG;
			case '=':
				consume_chained_params(argv[i] + 1, argv[i], FORCE);
				goto NEXT_ARG;
			case '-':
				switch (argv[i][1]) {
					case 'U':
					case 'u':
					case 'L':
					case 'l':
					case 'D':
					case 'd':
					case 'A':
					case 'a':
					case 'P':
					case 'p':
						consume_chained_params(argv[i] + 1, argv[i], NO);
						goto NEXT_ARG;
					case 'C':
					case 'c':
						if (argv[i][2] != 0) {
							bad_parameter(argv[i]);
						}
						if (argc <= i + 1) {
							parameter_missing_argument(argv[i]);
						}
						++ i;
						get_number(&repetitions, argv[i], "count");
						goto NEXT_ARG;
					case 'X':
					case 'x':
						if (argv[i][2] != 0) {
							bad_parameter(argv[i]);
						}
						if (argc <= i + 1) {
							parameter_missing_argument(argv[i]);
						}
						++ i;
						get_number(&max_length, argv[i], "maximum length");
						goto NEXT_ARG;
					case 'N':
					case 'n':
						if (argv[i][2] != 0) {
							bad_parameter(argv[i]);
						}
						if (argc <= i + 1) {
							parameter_missing_argument(argv[i]);
						}
						++ i;
						get_number(&min_length, argv[i], "minimum length");
						goto NEXT_ARG;
					case '?':
						if (argv[i][2] != 0) {
							bad_parameter(argv[i]);
						}
						show_help();
						exit(0);
					case 'V':
					case 'v':
						if (argv[i][2] != 0) {
							bad_parameter(argv[i]);
						}
						show_version();
						exit(0);
					case '-':
						if (argv[i][2] == 0) {
							if (argc <= i + 1) {
								parameter_missing_argument(argv[i]);
							}
							++ i;
							get_custom_chars(argv[i], "custom characters");
							goto NEXT_ARG;
						} else if (strcmp(argv[i] + 2, "help") == 0) {
							show_help();
							exit(0);
						} else if (strcmp(argv[i] + 2, "version") == 0) {
							show_version();
							exit(0);
						} else if (starts_with(argv[i] + 2, "count=")) {
							get_number(&repetitions, argv[i] + 8, "count");
							goto NEXT_ARG;
						} else if (starts_with(argv[i] + 2, "max=")) {
							get_number(&max_length, argv[i] + 6, "maximum length");
							goto NEXT_ARG;
						} else if (starts_with(argv[i] + 2, "maximum=")) {
							get_number(&max_length, argv[i] + 10, "maximum length");
							goto NEXT_ARG;
						} else if (starts_with(argv[i] + 2, "min=")) {
							get_number(&min_length, argv[i] + 6, "minimum length");
							goto NEXT_ARG;
						} else if (starts_with(argv[i] + 1, "minimum=")) {
							get_number(&min_length, argv[i] + 10, "minimum length");
							goto NEXT_ARG;
						} else if (starts_with(argv[i] + 2, "upper=")) {
							get_accept(&upper, argv[i] + 8, "upper");
							goto NEXT_ARG;
						} else if (starts_with(argv[i] + 2, "upper-case=")) {
							get_accept(&upper, argv[i] + 13, "upper-case");
							goto NEXT_ARG;
						} else if (starts_with(argv[i] + 2, "lower=")) {
							get_accept(&lower, argv[i] + 8, "lower");
							goto NEXT_ARG;
						} else if (starts_with(argv[i] + 2, "lower-case=")) {
							get_accept(&lower, argv[i] + 13, "lower-case");
							goto NEXT_ARG;
						} else if (starts_with(argv[i] + 2, "digits=")) {
							get_accept(&numer, argv[i] + 9, "digits");
							goto NEXT_ARG;
						} else if (starts_with(argv[i] + 2, "decimal=")) {
							get_accept(&numer, argv[i] + 10, "decimal");
							goto NEXT_ARG;
						} else if (starts_with(argv[i] + 2, "ascii=")) {
							get_accept(&ascii, argv[i] + 8, "ascii");
							goto NEXT_ARG;
						} else if (starts_with(argv[i] + 2, "print=")) {
							get_boolean(&printable, argv[i] + 8, "printable");
							goto NEXT_ARG;
						} else if (starts_with(argv[i] + 2, "printable=")) {
							get_boolean(&printable, argv[i] + 12, "printable");
							goto NEXT_ARG;
						} else if (strcmp(argv[i] + 2, "debug") == 0) {
							debug = 1;
							goto NEXT_ARG;
						}
						/*fall through*/
					default:
						;
						/*fall through*/
				}
			default:
				bad_parameter(argv[i]);
				/*fall through*/
		}
NEXT_ARG:
		++ i;
	}

	if (debug) {
		show_help();
		printf("%sOPTIONS:%s\n", BOLD, NORMAL);
		printf("  -C %d\n", repetitions);
		printf("  -X %d\n", max_length);
		printf("  -N %d\n", min_length);
		if (custom_chars_n > 0) {
			printf("  -- \"%s\"\n", custom_chars);
		} else {
			printf("  -- \"\"\n");
		}
		printf("  %cU\n", upper == NO ? '-' : (upper == FORCE ? '=' : '+'));
		printf("  %cL\n", lower == NO ? '-' : (lower == FORCE ? '=' : '+'));
		printf("  %cD\n", numer == NO ? '-' : (numer == FORCE ? '=' : '+'));
		printf("  %cA\n", ascii == NO ? '-' : (ascii == FORCE ? '=' : '+'));
		printf("  %cP\n", printable == NO ? '-' : '+');
		exit(0);
	}
}

void permute(char* string, int length) {
	int i, j;
	char c;
	if (length < 2) return;
	for (i = 0; i < length; i++) {
		j = my_rand(length);
		if (j == i) {
			j = (j + 1) % length;
		}
		c = string[j];
		string[j] = string[i];
		string[i] = c;
	}
}
