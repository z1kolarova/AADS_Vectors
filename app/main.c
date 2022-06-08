/*!
 * \file       main.c
 * \author     FAI
 * \date       9. 2. 2022
 * \brief      Unit tests for vector module.
 * ******************************************
 * \attention
 * &copy; Copyright (c) 2022 FAI UTB. All rights reserved.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */
#include "ioutils.h"
#include "vector.h"

/* Includes --------------------------------------------------------------------------------------*/
#include <ctype.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

/* Private types ---------------------------------------------------------------------------------*/
/* Private macros --------------------------------------------------------------------------------*/

#define GET_VALUE_AND_TEST(FORMAT, VAR)                                                            \
  do {                                                                                             \
    run = io_utils_get(FORMAT, &VAR);                                                              \
    if (!run) {                                                                                    \
      break;                                                                                       \
    }                                                                                              \
  } while (0)

#define GET_VECTOR_VALUE_AND_TEST(VAR) GET_VALUE_AND_TEST("%" VECTOR_DATATYPE_READ, VAR)

/* Private variables -----------------------------------------------------------------------------*/
/* Private function declarations -----------------------------------------------------------------*/
/* Exported functions definitions ----------------------------------------------------------------*/
int main(void)
{
  bool run = true;
  Vector_DataType_t n;
  printf("Vector test program\n"
         "Type the default size of array and the size of incrementation.\n"
         "Size:\n");
  Vector_t *vector;
  {
    size_t size;

    if (io_utils_get("%zu", &size) == false) {
      return 0;
    }

    printf("Incrementation:\n");

    size_t step;
    if (io_utils_get("%zu", &step) == false) {
      return 0;
    }

    vector = Vector_Create(size, step);
  }

  if (vector == NULL) {
    printf("Memory for vector was not allocated successfully.\n");
    return 0;
  }

  for (Vector_DataType_t val = 0; val < 100; val += 5) {
    Vector_Append(vector, val);
  }

  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);

  while (run) {
    printf("Press:\n"
           "1 to show the length of vector\n"
           "2 to add a new item\n"
           "3 to remove an item\n"
           "4 to print all items\n"
           "5 to find out whether an item is present\n"
           "6 to find an item's position\n"
           "7 to clear the vector\n"
           "8 to fill a portion of the vector with desired value\n"
           "9 to create a copy of a vector\n"
           "0 to print one item\n"
           "S to set value of item in vector\n"
           "anything else to finish.\n");
    char c;

    if (io_utils_get_char(&c) == false) {
      break;
    }

    c = toupper(c);

    switch (c) {
      case '1':
        printf("Vector length: %zu\n", Vector_Length(vector));
        break;

      case '2':
        printf("Choose the value of an item:\n");
        GET_VECTOR_VALUE_AND_TEST(n);
        Vector_Append(vector, n);
        break;

      case '3':
        printf("Choose the position of an item to be removed:\n");
        GET_VECTOR_VALUE_AND_TEST(n);
        Vector_Remove(vector, n);
        break;

      case '4':
        if (Vector_Length(vector) != SIZE_MAX) {
          for (size_t i = 0; i < Vector_Length(vector); i++) {
            Vector_DataType_t elm;

            if (Vector_At(vector, i, &elm)) {
              printf("vector[%zu]: %" VECTOR_DATATYPE_PRINT "\n", i, elm);
            } else {
              printf("Item at position %zu was not found within the vector.\n", i);
            }
          }
        }

        break;

      case '5':
        printf("Choose the value of an item to be found:\n");
        GET_VECTOR_VALUE_AND_TEST(n);

        if (Vector_Contains(vector, n)) {
          printf("Vector contains this item.\n");
        } else {
          printf("Vector does not contain this item.\n");
        }

        break;

      case '6':
        printf("Choose the value of an item to be found:\n");
        GET_VECTOR_VALUE_AND_TEST(n);
        size_t index;
        printf("Choose the initial index for searching:\n");
        GET_VALUE_AND_TEST("%zu", index);
        size_t position = Vector_IndexOf(vector, n, index);
        if (position != SIZE_MAX) {
          printf("Position of an item: %zu\n", position);
        } else {
          printf("Position of an item: not found\n");
        }
        break;

      case '7':
        Vector_Clear(vector);
        break;

      case '8':
        printf("Choose the value to be inserted:\n");
        Vector_DataType_t val = 0;
        GET_VECTOR_VALUE_AND_TEST(val);
        printf("Choose the starting position:\n");
        size_t begin = 0;
        GET_VALUE_AND_TEST("%zu", begin);
        printf("Choose the end position:\n");
        size_t end = 0;
        GET_VALUE_AND_TEST("%zu", end);
        Vector_Fill(vector, val, begin, end);
        break;

      case '9': {
        Vector_t *copy = Vector_Copy(vector);

        if (copy != NULL) {
          if (Vector_Length(copy) != SIZE_MAX) {
            for (size_t i = 0; i < Vector_Length(copy); i++) {
              Vector_DataType_t elm;

              if (Vector_At(copy, i, &elm)) {
                printf("copy[%zu]: %" VECTOR_DATATYPE_PRINT "\n", i, elm);
              } else {
                printf("Item at position %zu was not found within the vector.\n", i);
              }
            }
          }

          printf("Removing the vector \"copy\"\n");
          Vector_Destroy(&copy);
        }
      } break;

      case '0': {
        printf("Choose the value of index to be found: \n");
        size_t i = 0;
        GET_VALUE_AND_TEST("%zu", i);
        Vector_DataType_t elm;

        if (Vector_At(vector, i, &elm)) {
          printf("vector[%zu]: %" VECTOR_DATATYPE_PRINT "\n", i, elm);
        } else {
          printf("Item at position %zu was not found within the vector.\n", i);
        }
      } break;

      case 'S': {
        puts("Enter index to set new data:");
        size_t index;
        GET_VALUE_AND_TEST("%zu", index);

        puts("Enter value to set:");
        Vector_DataType_t data;
        GET_VECTOR_VALUE_AND_TEST(data);

        Vector_Set(vector, index, data);
      } break;

      default:
        run = false;
        break;
    }
  }

  printf("Freeing all allocated memory.\n");

  Vector_Destroy(&vector);

  printf("All allocated memory was freed.\n");

  return 0;
}

/* Private function definitions ------------------------------------------------------------------*/