#include <string.h>
#include <stdint.h>
#include <limits.h>
#include <stdlib.h>

#include "stringutils.h"

bool is_terminating_char(char value)
{
    if (value == '\0')
        return true;

    return false;
}

bool contains_terminating_char(uintptr_t value)
{
    size_t i = 0;
    char current_char, previous_char;

    // it checks if there is a zero char in the address
    // the zeros at the beginning are ignored
    while (i < sizeof(uintptr_t))
    {
        current_char = (char)(value & (unsigned char)-1);
        if (i != 0 && current_char != 0 && is_terminating_char(previous_char))
        {
            return true;
        }

        previous_char = current_char;
        value >>= CHAR_BIT;
        i++;
    }

    return false;
}

void remove_nulls(char *contents, size_t length)
{
    for (size_t i = 0; i < length; i++)
        if (contents[i] == '\0')
            contents[i]++;
}

void locate_terminating_chars(char *contents, size_t length)
{
    char found_one = 0;
    char print_in_the_middle = 1;

    for (size_t i = 0; i < length; i++)
    {
        if (is_terminating_char(contents[i]))
        {
            if (found_one)
            {
                fprintf(stderr, ", %x at %zu", contents[i] & 0xff, i);
                print_in_the_middle = 1;
            }
            else
            {
                found_one = 1;
                fprintf(stderr, "The payload has a terminating char (%x) at pos %zu", contents[i] & 0xff, i);
            }
        }
        else
        {
            if (found_one && print_in_the_middle)
            {
                print_in_the_middle = 0;
                fprintf(stderr, " (in the middle)");
            }
        }
    }
    if (found_one)
    {
        fprintf(stderr, "\n");
    }
}

int pstr_to_enum(char *pstr, char *strs[], unsigned int len)
{
    int enum_pos = -1;

    for (int i = 0; i < len; i++)
    {
        if (strcmp(pstr, strs[i]) == 0)
        {
            enum_pos = i;
            break;
        }
    }

    return enum_pos;
}

long get_file_len(FILE *f)
{
    long file_len;

    // seek to the end of file
    fseek(f, 0, SEEK_END);

    // get the current byte offset in the file
    file_len = ftell(f);

    // seek back to the beginning of the file
    rewind(f);

    return file_len;
}

/**
 * @brief Copies the content of a binary file into a buffer.
 *
 * Copies the content of a binary file into a buffer. Returns the number
 * of byte copied (namely the file length) or -1 in case of errors.
 *
 * @param buffer
 * @return int
 */
long copy_binary_content(const char *filename, char *buffer[])
{
    long file_len;
    size_t bytes_read;
    FILE *f;

    f = fopen(filename, "rb");

    if (f == NULL)
    {
        fprintf(stderr, "Error: file error in find_gadget");
        exit(1);
    }

    file_len = get_file_len(f);
    *buffer = (char *)malloc((file_len + 1) * sizeof(char));

    if (*buffer == NULL)
    {
        fprintf(stderr, "Error: Cannot allocate memory in find_gadget");
        exit(1);
    }

    bytes_read = fread(*buffer, file_len, 1, f);

    if (bytes_read == 0 && ferror(f))
    {
        fprintf(stderr, "Error: Cannot read file in find_gadget");
        exit(1);
    }

    fclose(f);

    return file_len;
}

int find_gadget_offset(const char *filename, char *search_chars)
{
    long file_len;
    char *buffer, function_signature[] = {0x55, 0x48, 0x89, 0xE5};
    size_t search_chars_count = strlen(search_chars);
    size_t function_signature_count = sizeof(function_signature) / sizeof(function_signature[0]);
    int i = 0, found = 0, current_found_i = 0, offset = 0;

    copy_binary_content(filename, &buffer);

    // Look backward for the bytes pattern search_chars and then the beginning of the function 0x554889e5
    while ((i < file_len) && (found == 0))
    {
        if (buffer[file_len - i] == search_chars[search_chars_count - 1 - current_found_i])
        {
            current_found_i++;
            if (current_found_i > search_chars_count - 1)
            { // pattern found, look for start of local function
                current_found_i = 0;
                // the gadgets are at an offset from 50 to 150 from the function beginning
                offset = 50;
                // if (output_debug_info)
                //   fprintf(stderr,"Pattern found at byte %ld\n",file_len-i);
                while ((offset < 150) && (found == 0))
                {
                    if (buffer[file_len - i - offset] == function_signature[function_signature_count - 1 - current_found_i])
                    {
                        current_found_i++;
                        if (current_found_i > function_signature_count - 1)
                        {
                            if (output_debug_info)
                                fprintf(stderr, "Begin of function byte at %ld, offset:%d\n", file_len - i - offset, offset);
                            found = 1;
                            break;
                        }
                    }
                    offset++;
                }
            }
        }
        else
        {
            current_found_i = 0;
        }
        i++;
    }

    if (found == 0)
    {
        fprintf(stderr, "pattern not found\n");
        return 0;
    }
    else
    {
        if (output_debug_info)
            fprintf(stderr, "Found at gadgetX+%d\n", offset);
        return offset;
    }
}
