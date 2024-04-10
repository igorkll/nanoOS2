#include "main.h"
#include "xstr.h"

char* xstr_build(uint16_t size, const char* fmt, ...) {
    char* str = malloc(size + 1);
    snprintf(str, size, fmt, ...);
    return str;
}