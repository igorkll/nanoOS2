#include <stdint.h>

typedef struct {
    char* ptr;
    uint32_t len;
} xstr;

xstr xstr_new();
void xstr_del(xstr* str);
void xstr_resize(xstr* str, uint32_t len);
void xstr_minsize(xstr* str, uint32_t len);
void xstr_maxsize(xstr* str, uint32_t len);
uint32_t xstr_len(xstr* str);
#define xstr_fill(str, ...) snprintf((str)->ptr, xstr_len(str), __VA_ARGS__);

void xstr_appendLeft(xstr* str, const char* append);
void xstr_appendRight(xstr* str, const char* append);