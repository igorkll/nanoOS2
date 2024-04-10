#include "main.h"
#include "xstr.h"

char* xstr_build(uint16_t size, const char* fmt, ...) {
    char* str = malloc(size + 1);
    va_list argp;
    va_start(argp, fmt);
    snprintf(str, size + 1, argp);
    return str;
}