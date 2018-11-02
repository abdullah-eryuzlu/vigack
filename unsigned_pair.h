#ifndef unsigned_pair_h
#define unsigned_pair_h

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    float *m_elements;
    size_t m_size;
} UnsignedPair;

UnsignedPair up_init();
void up_log(UnsignedPair* self);
float* up_get_pointer(UnsignedPair* self, const float key);
void up_sort(UnsignedPair* self);
float up_get_value(UnsignedPair* self, const float key);
void up_set_value(UnsignedPair* self, const float key, const float value);
void up_get_pair(UnsignedPair* self, const size_t n, float output[2]);
size_t up_size(UnsignedPair* self);
void up_sum_value(UnsignedPair* self, const float key, const unsigned int amount);
float up_sum_of_values(UnsignedPair* self);

#endif
