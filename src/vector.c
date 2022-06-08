/*!
 * \file       vector.c
 * \author     Horak, Jurena
 * \date       2019.6
 * \brief      Implementation of function.h header file
 * ******************************************
 * \attention
 * &copy; Copyright (c) 2022 FAI UTB. All rights reserved.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */
/* Includes --------------------------------------------------------------------------------------*/
#include "vector.h"

/* Private types ---------------------------------------------------------------------------------*/
/* Private macros --------------------------------------------------------------------------------*/
#define UNUSED(x) (void)x

/* Private variables -----------------------------------------------------------------------------*/
/* Private function declarations -----------------------------------------------------------------*/
/* Exported functions definitions ----------------------------------------------------------------*/
Vector_t *Vector_Create(size_t initial_size, size_t alloc_step)
{
  UNUSED(initial_size);
  UNUSED(alloc_step);

  return NULL;
}

Vector_t *Vector_Copy(const Vector_t *const original)
{
  UNUSED(original);
  return NULL;
}

void Vector_Clear(Vector_t *const vector)
{
  UNUSED(vector);
}

size_t Vector_Length(const Vector_t *const vector)
{
  UNUSED(vector);
  return SIZE_MAX;
}

bool Vector_At(const Vector_t *const vector, size_t position, Vector_DataType_t *const value)
{
  UNUSED(vector);
  UNUSED(position);
  UNUSED(value);
  return false;
}

bool Vector_Remove(Vector_t *const vector, size_t position)
{
  UNUSED(vector);
  UNUSED(position);
  return false;
}

size_t Vector_Append(Vector_t *vector, Vector_DataType_t value)
{
  UNUSED(vector);
  UNUSED(value);
  return SIZE_MAX;
}

void Vector_Set(Vector_t *const vector, size_t position, Vector_DataType_t value)
{
  UNUSED(vector);
  UNUSED(position);
  UNUSED(value);
}

bool Vector_Contains(const Vector_t *const vector, Vector_DataType_t value)
{
  UNUSED(vector);
  UNUSED(value);
  return false;
}

size_t Vector_IndexOf(const Vector_t *const vector, Vector_DataType_t value, size_t from)
{
  UNUSED(vector);
  UNUSED(value);
  UNUSED(from);
  return SIZE_MAX;
}

void Vector_Fill(const Vector_t *const vector,
                 Vector_DataType_t value,
                 size_t start_position,
                 size_t end_position)
{
  UNUSED(vector);
  UNUSED(value);
  UNUSED(start_position);
  UNUSED(end_position);
}

void Vector_Destroy(Vector_t **const vector)
{
  UNUSED(vector);
}

/* Private function definitions ------------------------------------------------------------------*/