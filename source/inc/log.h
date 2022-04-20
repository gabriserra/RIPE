
#include <stdbool.h>
#include <stdio.h>

typedef struct logger_t
{
    bool output_debug_info;
    bool output_error_info;
} logger_t;

#define DEBUG_INFO_ENABLE(logger, value) \
    logger.output_debug_info = (value);

#define DEBUG_INFO_IS_ENABLED(logger) \
    (logger.output_debug_info == true);

#define ERROR_INFO_ENABLE(logger, value) \
    logger.output_error_info = (value);

#define ERROR_INFO_IS_ENABLED(logger) \
    (logger.output_error_info == true);

int debug_info_fprintf(logger_t *logger, const char *format, va_list args);

int error_info_fprintf(logger_t *logger, const char *format, va_list args);
