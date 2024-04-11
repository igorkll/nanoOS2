#include "main.h"
#include "xstr.h"

struct xstr xstr_new() {
    struct xstr str = {
        .ptr = malloc(1),
        .len = 0
    };
    str.ptr[0] = '\0';
    return str;
}

void xstr_del(struct xstr* str) {
    free(str->ptr);
}

void xstr_resize(struct xstr* str, uint32_t len) {
    str->ptr = realloc(str->ptr, len + 1);
    str->ptr[len] = '\0';
    str->len = len;
}

void xstr_minsize(struct xstr* str, uint32_t len) {
    if (str->len < len) xstr_resize(str, len);
}