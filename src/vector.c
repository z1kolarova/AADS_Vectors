/**
 * @file       functions.c
 * @author     Horak
 * @date       2019.6
 * @brief      Implementation of function.h header file
 * ******************************************
 * @par       COPYRIGHT NOTICE (c) 2019 TBU in Zlin. All rights reserved.
 */

#include "vector.h"
#include "mymalloc.h"

Vector_t *Vector_Create(uint64_t initial_size, uint32_t alloc_step) {
  (void)initial_size;
  (void)alloc_step;
  return NULL;
}

Vector_t *Vector_Copy(const Vector_t *const original) {
  (void)original;
  return NULL;
}

void Vector_Clear(Vector_t *const vector) {
  (void)vector;
}

uint64_t Vector_Length(const Vector_t *const vector) {
  (void)vector;
  return -1;
}

bool Vector_At(const Vector_t *const vector, uint64_t position, uint64_t *const value) {
  (void)vector;
  (void)position;
  (void)value;
  return false;
}

bool Vector_Remove(Vector_t *const vector, uint64_t position) {
  (void)vector;
  (void)position;
  return false;
}

void Vector_Append(Vector_t *const vector, uint64_t value) {
  (void)vector;
  (void)value;
}

bool Vector_Contains(const Vector_t *const vector, uint64_t value) {
  (void)vector;
  (void)value;
  return false;
}

uint64_t Vector_IndexOf(const Vector_t *const vector, uint64_t value, uint64_t from) {
  (void)vector;
  (void)value;
  (void)from;
  return -1;
}

void Vector_Fill(const Vector_t *const vector, uint64_t value, uint64_t start_position, uint64_t end_position) {
  (void)vector;
  (void)value;
  (void)start_position;
  (void)end_position;
}

void Vector_Destroy(Vector_t **const vector) {
  (void)vector;
}
