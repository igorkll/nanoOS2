#include <stdint.h>
#define VEC_MAX 3

struct Vec {
    float data[VEC_MAX];
    uint8_t len;
};

void vec_new(struct Vec*, uint8_t len);
float vec_len(struct Vec*);
void vec_set(struct Vec*, uint8_t i, float v);
float vec_get(struct Vec*, uint8_t i);

bool vec_equals(struct Vec*, struct Vec*);
void vec_add(struct Vec*, struct Vec*);
void vec_sub(struct Vec*, struct Vec*);
void vec_mul(struct Vec*, struct Vec*);
void vec_div(struct Vec*, struct Vec*);

