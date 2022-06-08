/*!
 * \file    mymalloc.h
 * \author  Dulik
 * \brief   MyMalloc module header file (allocation debugging function memory)
 *
 * ******************************************
 * \attention
 * &copy; Copyright (c) 2022 FAI UTB. All rights reserved.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */
#ifndef _MYMALLOC_H_
#define _MYMALLOC_H_

/* Includes --------------------------------------------------------------------------------------*/
#include <stddef.h>

/* Exported types --------------------------------------------------------------------------------*/
/* Exported macros -------------------------------------------------------------------------------*/
/* Exported variables ----------------------------------------------------------------------------*/
/* Exported functions declarations ---------------------------------------------------------------*/
void *myMalloc(size_t size);
void myFree(void *memblock);
void *myRealloc(void *ptr, size_t newSize);

#endif  //_MYMALLOC_H_
