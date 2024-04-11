#include "main.h"
#include "vec.h"

void vec_new(struct Vec* vec, uint8_t len) {
    if (len > VEC_MAX) {
        printf("max vec len %i\n", VEC_MAX);
        abort();
        return;
    }

    vec->len = len;
    for (uint8_t i = 0; i < len; i++) {
        vec->data[i] = 0;
    }
}

float vec_len(struct Vec* vec) {
    float sum = 0;
    for (uint8_t i = 0; i < vec->len; i++) {
        sum += pow(vec->data[i], 2);
    }
    return sqrt(sum);
}

void vec_set(struct Vec* vec, uint8_t i, float v) {
    vec->data[i] = v;
}

float vec_get(struct Vec* vec, uint8_t i) {
    return vec->data[i];
}

bool vec_equals(struct Vec* vec1, struct Vec* vec2) {
    for (uint8_t i = 0; i < vec1->len; i++) {
        if (vec1->data[i] != vec2->data[i]) return false;
    }
    return true;
}

void vec_add(struct Vec* vec1, struct Vec* vec2) {
    for (uint8_t i = 0; i < vec1->len; i++) {
        vec1->data[i] += vec2->data[i];
    }
}

void vec_sub(struct Vec* vec1, struct Vec* vec2) {
    for (uint8_t i = 0; i < vec1->len; i++) {
        vec1->data[i] -= vec2->data[i];
    }
}

void vec_mul(struct Vec* vec1, struct Vec* vec2) {
    for (uint8_t i = 0; i < vec1->len; i++) {
        vec1->data[i] *= vec2->data[i];
    }
}

void vec_div(struct Vec* vec1, struct Vec* vec2) {
    for (uint8_t i = 0; i < vec1->len; i++) {
        vec1->data[i] /= vec2->data[i];
    }
}