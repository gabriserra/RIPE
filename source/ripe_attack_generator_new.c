
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <stdlib.h>

#include "log.h"
#include "ripe_attack_parameters.h"

logger_t logger;

int main(int argc, char *argv[])
{
    int option_char;

    while ((option_char = getopt(argc, argv, "t:i:c:l:f:d:e:o")) != -1)
    {
        switch (option_char)
        {
        case 't':
            set_technique(optarg);
            break;
        case 'i':
            set_inject_param(optarg);
            break;
        case 'c':
            set_code_ptr(optarg);
            break;
        case 'l':
            set_location(optarg);
            break;
        case 'f':
            set_function(optarg);
            break;
        case 'd':
            DEBUG_INFO_ENABLE(logger, strcmp("t", optarg) == 0);
            break;
        case 'e':
            ERROR_INFO_ENABLE(logger, strcmp("t", optarg) == 0);
            break;
        case 'o':
            // TODO: we really need this?
            break;
        default:
            error_info_fprintf(&logger, "Error: Unknown command option \"%s\"\n", optarg);
            exit(1);
            break;
        }
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
        }
    }

    return enum_pos;
}

void set_enum_param(char *choice, char *params[], int *dest, unsigned int len, char *param_name)
{
    int pos = pstr_to_enum(choice, params, len);

    if (pos < 0)
    {
        if (output_error_msg)
        {
            fprintf(stderr, "Error: Unknown choice of '%s' parameter \"%s\"\n", param_name, choice);
        }
        exit(1);
    }

    *dest = pos;
}

void set_inject_param(char *choice)
{
    set_enum_param(choice, opt_techniques, &attack.technique, nr_of_techniques, "technique");
}

void set_inject_param(char *choice)
{
    set_enum_param(choice, opt_inject_params, &attack.inject_param, nr_of_inject_params, "injection");
}

void set_code_ptr(char *choice)
{
    set_enum_param(choice, opt_code_ptrs, &attack.code_ptr, nr_of_code_ptrs, "code pointer");
}

void set_location(char *choice)
{
    set_enum_param(choice, opt_locations, &attack.location, nr_of_locations, "memory location");
}

void set_function(char *choice)
{
    set_enum_param(choice, opt_funcs, &attack.function, nr_of_funcs, "vulnerable function");
}

// TODO: think a better way to handle this

bool is_attack_possible()
{
    switch (attack.location)
    {
    case STACK:

        break;

    default:
        break;
    }
}