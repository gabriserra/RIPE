#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

bool is_terminating_char(char value);

bool contains_terminating_char(uintptr_t value);

void remove_nulls(char *contents, size_t length);

void locate_terminating_chars(char *contents, size_t length);

int pstr_to_enum(char *pstr, char *strs[], unsigned int len);

int get_file_len(FILE *f);