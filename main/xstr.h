#include <stdint.h>

struct xstr {
    char* ptr;
    uint32_t len;
};

struct xstr xstr_new();
void xstr_del(struct xstr* str);
void xstr_resize(struct xstr* str, uint32_t len);
void xstr_minsize(struct xstr* str, uint32_t len);
#define xstr_fill(str, ...) snprintf((str)->ptr, xstr_len(str), __VA_ARGS__);