#include <stdarg.h>
#include "memoryutils.h"

void print_memory(FILE *stream, char *start, size_t words)
{
    char *pointer;

    for (pointer = start; pointer < (char *)(start + words); pointer += 4)
    {
        fprintf(stream, "%p: 0x%x\n", pointer, *(unsigned int *)pointer);
    }
}

void save_memory(memory_dump_t *dump, char *start, size_t size)
{
    size_t iterator;
    char *pointer = start;

    for (iterator = 0; iterator < size; iterator++)
    {
        snprintf(dump[iterator].address, HEX_STRING_SIZE, "%p", pointer);
        snprintf(dump[iterator].value, HEX_STRING_SIZE, "0x%x", *(unsigned int *)pointer);
        pointer += sizeof(long);
    }
}

void print_n_memory_dumps(FILE *stream, size_t size, unsigned int n, ...)
{
    va_list args;

    for (unsigned int i = 0; i < n; i++)
        fprintf(stream, "%-22s %d", "Memory dump", (i + 1));
    fprintf(stream, "\n");

    for (unsigned int i = 0; i < n; i++)
        fprintf(stream, "%-11s%-11s", "Address", "Value");
    fprintf(stream, "\n");

    va_start(args, n);

    for (size_t i = 0; i < size; i++)
    {
        for (unsigned int i = 0; i < n; i++)
        {
            memory_dump_t *d = va_arg(args, memory_dump_t *);
            fprintf(stream, "%-11s%-11s", d[i].address, d[i].value);
        }
        fprintf(stream, "\n");
    }

    va_end(args);
    fprintf(stream, "\n\n");
}