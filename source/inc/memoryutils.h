
#include <stdio.h>

#define HEX_STRING_SIZE 11

struct memory_dump
{
    char address[HEX_STRING_SIZE];
    char value[HEX_STRING_SIZE];
};

typedef struct memory_dump memory_dump_t;

void print_memory(FILE *stream, char *start, size_t words);

void save_memory(memory_dump_t *dump, char *start, size_t size);

void print_n_memory_dumps(FILE *stream, size_t size, unsigned int n, ...);