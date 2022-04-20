
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

