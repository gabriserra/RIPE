
#include "log.h"

int debug_info_fprintf(logger_t *logger, const char *format, va_list args)
{
    if (debug_info_is_enabled(logger))
    {
        fprintf(stderr, format, args);
    }
}

int error_info_fprintf(logger_t *logger, const char *format, va_list args)
{
    if (error_info_is_enabled(logger))
    {
        fprintf(stderr, format, args);
    }
}
