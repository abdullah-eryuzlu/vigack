#ifndef unsigned_pair_h
#define unsigned_pair_h

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    float *m_elements;
    size_t m_size;
} UnsignedPair;

UnsignedPair up_init();

void up_log(const UnsignedPair *self);

float* up_get_pointer(const UnsignedPair *self, const float key);

void up_sort(const UnsignedPair *self);

float up_get_value(const UnsignedPair *self, const float key);

void up_set_value(UnsignedPair *self, const float key, const float value);

void up_get_pair(const UnsignedPair *self, const size_t n, float output[2]);

size_t up_size(const UnsignedPair *self);

void up_sum_value(UnsignedPair *self, const float key, const unsigned int amount);

float up_sum_of_values(const UnsignedPair *self);

#endif
