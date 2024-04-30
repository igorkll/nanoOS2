#include "main.h"
#include "xstr.h"

xstr xstr_new() {
    xstr str = {
        .ptr = malloc(1),
        .len = 0
    };
    str.ptr[0] = '\0';
    return str;
}

void xstr_del(xstr* str) {
    free(str->ptr);
}

void xstr_resize(xstr* str, uint32_t len) {
    uint32_t oldlen = xstr_len(str);
    str->ptr = realloc(str->ptr, len + 1);
    str->ptr[len] = '\0';
    if (len > oldlen) {
        for (uint32_t i = oldlen; i < len; i++) {
            str->ptr[i] = ' ';
        }
    }
    str->len = len;
}

void xstr_minsize(xstr* str, uint32_t len) {
    if (str->len < len) xstr_resize(str, len);
}

void xstr_maxsize(xstr* str, uint32_t len) {
    if (str->len > len) xstr_resize(str, len);
}

uint32_t xstr_len(xstr* str) {
    return strlen(str->ptr);
}