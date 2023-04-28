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
#include <mymalloc.h>
#include <stdlib.h>

/* Private types ---------------------------------------------------------------------------------*/
/* Private macros --------------------------------------------------------------------------------*/
#define UNUSED(x) (void)x

/* Private variables -----------------------------------------------------------------------------*/
/* Private function declarations -----------------------------------------------------------------*/
/* Exported functions definitions ----------------------------------------------------------------*/
Vector_t *Vector_Create(size_t initial_size, size_t alloc_step)
{
  Vector_t * v = myMalloc(sizeof(Vector_t));
  v->items = myMalloc(initial_size * sizeof(Vector_DataType_t));
  if(v->items == NULL)
  {
      myFree(v);
      return NULL;
  }
  v->size = initial_size;
  v->alloc_step = alloc_step;
  v->next = v->items;
  return v;
}

Vector_t *Vector_Copy(const Vector_t *const original)
{
    if(original)
    {
        Vector_t* v = myMalloc(sizeof(Vector_t));
        if(v == NULL)
        {
            return NULL;
        }
        v->size = original->size;
        v->items = myMalloc(original->size * sizeof(Vector_DataType_t));
        if(v->items == NULL)
        {
            myFree(v);
            return NULL;
        }
        v->alloc_step = original->alloc_step;
        v->next = v->items;

        Vector_DataType_t value;
        size_t itemCount = Vector_Length(original);
        for(size_t i = 0; i < itemCount; i++)
        {
            if(Vector_At(original, i, &value))
            Vector_Append(v, value);
        }

        return v;
    }
    return NULL;
}

void Vector_Clear(Vector_t *const vector)
{
    if(vector)
    {
        myFree(vector->items);
        vector->items = NULL;
        vector->next = NULL;
        vector->size = 0;
    }
}

size_t Vector_Length(const Vector_t *const vector)
{
    if(vector)
    {
        return (vector->next - vector->items);
    }
    return SIZE_MAX;
}

bool Vector_At(const Vector_t *const vector, size_t position, Vector_DataType_t *const value)
{
    if(vector)
    {
        if((vector->items + position) <= vector->next)
        {
            *value = *(vector->items + position);
            return true;
        }
    }
    return false;
}

bool Vector_Remove(Vector_t *const vector, size_t position)
{
    if(vector)
    {
        size_t itemCount = Vector_Length(vector);

        if(position >= itemCount)
        {
            return false;
        }

        for(size_t i = position; i + 1 < itemCount;i++)
        {
            *(vector->items + i) = *(vector->items + i + 1);
        }
        vector->next--;
        return true;
    }
    return false;
}

size_t Vector_Append(Vector_t *vector, Vector_DataType_t value)
{
    if(vector)
    {
        if(vector->next >= vector->items + vector->size)
        {
            size_t itemCount = Vector_Length(vector);
            Vector_DataType_t *temp = realloc(vector->items,
                                              sizeof(Vector_DataType_t)*(vector->size + vector->alloc_step));
            if(temp == NULL)
            {
                return SIZE_MAX;
            }
            vector->items = temp;
            vector->size += vector->alloc_step;
            vector->next = vector->items + itemCount;
        }
        *(vector->next) = value;
        size_t appendedAt = vector->next - vector->items;
        vector->next++;
        return appendedAt;
    }
    return SIZE_MAX;
}

void Vector_Set(Vector_t *const vector, size_t position, Vector_DataType_t value)
{
    if(vector)
    {
        size_t itemCount = Vector_Length(vector);
        if(position >= itemCount)
            return;

        *(vector->items + position) = value;
    }
}

bool Vector_Contains(const Vector_t *const vector, Vector_DataType_t value)
{
    if(vector)
    {
        size_t itemCount = Vector_Length(vector);
        for(size_t i = 0; i < itemCount; i++)
        {
            if(*(vector->items + i) == value)
            {
                return true;
            }
        }
    }
    return false;
}

size_t Vector_IndexOf(const Vector_t *const vector, Vector_DataType_t value, size_t from)
{
    if(vector)
    {
        size_t itemCount = Vector_Length(vector);
        if(from < itemCount)
        {
            for(size_t i = from; i < itemCount; i++)
            {
                if(*(vector->items + i) == value)
                {
                    return i;
                }
            }
        }
    }
    return SIZE_MAX;
}

void Vector_Fill(const Vector_t *const vector,
                 Vector_DataType_t value,
                 size_t start_position,
                 size_t end_position)
{
    if(vector)
    {
        size_t itemCount = Vector_Length(vector);
        if(start_position >= itemCount)
            return;

        for(size_t i = start_position; i <= end_position && i < itemCount; i++)
        {
            *(vector->items + i) = value;
        }
    }
}

void Vector_Destroy(Vector_t **const vector)
{
    if(vector && *vector)
    {
        myFree((*vector)->items);
        (*vector)->items = NULL;
        myFree(*vector);
        *vector = NULL;
    }
    return;
}

/* Private function definitions ------------------------------------------------------------------*/
