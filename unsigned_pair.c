

#include "unsigned_pair.h"

UnsignedPair up_init() {
  UnsignedPair up;
  up.m_elements = malloc(1);
  up.m_size = 0;
  return up;
}

float* up_get_pointer(UnsignedPair* self, const float key) {
  if (self -> m_size == 0) return 0;
  for (int i = 0; i < self -> m_size; i += 2) {
    if (self -> m_elements[i] == key) return &(self -> m_elements[i+1]);
  }
  return 0;
}
void up_sort(UnsignedPair* self) {
  if (self -> m_size == 0) return;
    int index = 0;
    while (index != up_size(self) - 1) {
      for (int i = index; i < up_size(self); i++) {
        float temp_value[2] = { 0 };
          if (self -> m_elements[2*i+1] > self -> m_elements[2*index+1]) {
            temp_value[0] = self -> m_elements[2*i];
            temp_value[1] = self -> m_elements[2*i+1];
            self -> m_elements[2*i] = self -> m_elements[2*index];
            self -> m_elements[2*i+1] = self -> m_elements[2*index+1];
            self -> m_elements[2*index] = temp_value[0];
            self -> m_elements[2*index+1] = temp_value[1];
          }
        }
      index++;
  }
}
float up_get_value(UnsignedPair* self, const float key) {
  for (int i = 0; i < self -> m_size; i += 2) {
    if (self -> m_elements[i] == key) return self -> m_elements[i+1];
  }
  return -1;
}

void up_log(UnsignedPair* self) {
  for (int i = 0; i < self -> m_size; i += 2) {
    printf("%d %.4f\n", (int) self -> m_elements[i], self -> m_elements[i+1]);
  }
}

void up_set_value(UnsignedPair* self, const float key, const float value) {
  float* current_value = up_get_pointer(self, key);
  if (current_value) *current_value = value;
  else{
    float* new_elements = malloc(sizeof(float) * (self -> m_size + 2));
    for (int i = 0; i < self -> m_size; i++)
      new_elements[i] = self -> m_elements[i];
    new_elements[self -> m_size++] = key;
    new_elements[self -> m_size++] = value;
    free(self -> m_elements);
    self -> m_elements = new_elements;
  };
}
void up_get_pair(UnsignedPair* self, const size_t n, float output[2]) {
  output[0] = self -> m_elements[n * 2];
  output[1] = self -> m_elements[n * 2 + 1];
}
size_t up_size(UnsignedPair* self) {
  return self -> m_size / 2;
};
void up_sum_value(UnsignedPair* self, const float key, const unsigned int amount) {
  float* m_key = up_get_pointer(self, key);
  if (m_key) *m_key += amount;
  else up_set_value(self, key, amount);
}

float up_sum_of_values(UnsignedPair* self) {
  float sum = 0;
  for (int i = 0; i < up_size(self); i++) {
    sum += self -> m_elements[1];
  }
  return sum;
}
