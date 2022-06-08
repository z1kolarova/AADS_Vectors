/*!
 * \file    vector.h
 * \author  Roman Dosek, Tomas Jurena
 * \date    2/2015
 * \brief   Headers of Vector data structure
 *
 * ******************************************
 * \attention
 * &copy; Copyright (c) 2022 FAI UTB. All rights reserved.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#ifndef __VECTOR_H
#define __VECTOR_H

/*! \defgroup vector Vector
 *  \brief This module defines the API interface to the Vector data structure. The data structure is
 * designed to contain numerical data.
 *  \{
 */
/* Includes --------------------------------------------------------------------------------------*/
#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Exported types --------------------------------------------------------------------------------*/
/*! Data type that is stored in the vector. */
typedef uint64_t Vector_DataType_t;

/*! Structure that describes the vector. */
typedef struct {
  /*! Internal pointer to allocated memory. */
  Vector_DataType_t *items;

  /*! Number of currently allocated cells. */
  size_t size;

  /*! Pointer to the next empty cell. */
  Vector_DataType_t *next;

  /*! Number of cells allocated during expanding. */
  size_t alloc_step;
} Vector_t;

/* Exported macros -------------------------------------------------------------------------------*/
/*! Symbol that can be used when reading data from user that can be stored in vector.
 *  \note It must be updated when \ref Vector_DataType_t is changed!
 */
#define VECTOR_DATATYPE_READ SCNu64

/*! Symbol that can be used for printing of values stored in the vector.
 *  \note It must be updated when \ref Vector_DataType_t is changed!
 */
#define VECTOR_DATATYPE_PRINT PRIu64

/* Exported variables ----------------------------------------------------------------------------*/
/* Exported functions declarations ---------------------------------------------------------------*/
/*! Creates a vector with \a initial_size and a \a alloc_step. The returned pointer points to the
 * heap where is allocated new instance of the \ref Vector_t structure. After return from the
 * function the owner of a pointer to a vector structure is the user instead of the library and user
 * is responsible for freeing the memory.
 *
 * \param[in]   initial_size    Initial items count of the vector.
 * \param[in]   alloc_step      Number of items that are allocated to the vector when run out of
 * memory.
 *
 * \return  Pointer to the allocated vector structure or NULL in case of failure.
 *
 * \sa Vector_Destroy
 */
Vector_t *Vector_Create(size_t initial_size, size_t alloc_step);

/*! Creates a separate (independent) copy of a vector that contains the same data. The returned
 * instance contains only the inserted items to the original vector.
 *
 * \param[in]   original    Pointer to the vector to be copied.
 *
 * \return  Returns pointer to the allocated memory of copied vector, NULL is returned in case of
 * failure.
 */
Vector_t *Vector_Copy(const Vector_t *const original);

/*! Erases the content of a vector, allocated memory for \ref Vector_t.items is freed, the \ref
 * Vector_t.size of a vector is set to 0, \ref Vector_t.items and \ref Vector_t.next pointers are
 * set to NULL. The \ref Vector_t.alloc_step remains unchanged.
 *
 * \param[in]   vector  Pointer to a vector to clear.
 */
void Vector_Clear(Vector_t *const vector);

/*! Returns the current length of a vector. Current length means number of items inserted into the
 * vector (not the amount of allocated items).
 *
 * \param[in]   vector  Pointer to a vector.
 *
 * \return  Current length of a vector or SIZE_MAX if vector is NULL.
 */
size_t Vector_Length(const Vector_t *const vector);

/*! Returns the value of stored item at selected \a position in the \a vector.
 *
 * \param[in]   vector      Pointer to a vector.
 * \param[in]   position    Position within the vector.
 * \param[out]  value       Pointer to a value to be returned in case it is successfully found.
 *
 * \return Returns true when valid \a vector, \a position, and \a value is passed, otherwise it
 * returns false.
 */
bool Vector_At(const Vector_t *const vector, size_t position, Vector_DataType_t *const value);

/*! Removes an item at desired \a position and shifts data by one item to the left from that
 * position.
 *
 * \internal The \ref Vector_t.next is decremented after removing.
 *
 * \param[in]   vector      Pointer to a vector.
 * \param[in]   position    Position within the vector to be removed.
 *
 * \return Returns true when valid \a vector and valid \a position is passed,
 * otherwise returns false.
 */
bool Vector_Remove(Vector_t *const vector, size_t position);

/*! Appends a new item to the end of a vector. If \ref Vector_t.items is full, the memory is
 * reallocated to new size that is computed from current size and \ref Vector_t.alloc_step.
 *
 * \param[in]   vector  Pointer to a vector.
 * \param[in]   value   Value to be inserted.
 *
 * \return Returns index of the inserted item. If append fails it returns SIZE_MAX.
 */
size_t Vector_Append(Vector_t *const vector, Vector_DataType_t value);

/*! Sets \a value in the \a vector at specified \a position. The \a value is applied only if \a
 * position is smaller then current length of the \a vector, otherwise nothing is done.
 *
 * \param[in] vector    Pointer to a vector.
 * \param[in] position  Position within the vector to be overriden.
 * \param[in] value     Value to set.
 */
void Vector_Set(Vector_t *const vector, size_t position, Vector_DataType_t value);

/*! Looks for the \a value in the \a vector.
 *
 * \param[in]   vector  Pointer to a vector.
 * \param[in]   value   Value to be found.
 *
 * \return Returns true if valid \a vector is passed and the \a value is
 * contained by it, false otherwise.
 */
bool Vector_Contains(const Vector_t *const vector, Vector_DataType_t value);

/*! Finds the position of a \a value in the \a vector. An argument \a from specifies the search
 * offset from where is the value searched.
 *
 * \param[in]   vector  Pointer to a vector.
 * \param[in]   value   Value to be found.
 * \param[in]   from    Starting position for searching.
 *
 * \return  Returns index of position when \a value is found in the \a vector data, otherwise it
 * returns SIZE_MAX.
 */
size_t Vector_IndexOf(const Vector_t *const vector, Vector_DataType_t value, size_t from);

/*! Fills a portion of \a vector specified by a range with a desired value. Vector is overwritten
 * from the \a start_position to the \a end_position (including it). If the \a end_position is
 * located out of \a vector boundaries, \a vector is filled from the \a start_position to the last
 * item of the \a vector. If the \a start_position is located behind the last item of a vector, no
 * changes are made.
 *
 * \param[in]   vector          Pointer to a vector.
 * \param[in]   value           Value to be set.
 * \param[in]   start_position  Starting position.
 * \param[in]   end_position    End position.
 */
void Vector_Fill(const Vector_t *const vector,
                 Vector_DataType_t value,
                 size_t start_position,
                 size_t end_position);

/*! Erases all items of a \a vector and releases the allocated memory for structure. Pointer to a
 * \a vector is then set to NULL.
 *
 * \param[in,out] vector  Pointer to an adress of a structure that holds the vector.
 */
void Vector_Destroy(Vector_t **const vector);

/*! \} */
#endif  //__VECTOR_H
