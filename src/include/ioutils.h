/*!
 * \file    ioutils.h
 * \author  Dulik, Jurena
 * \date    16.2.2012
 * \brief   Declaration of the module for loading input from user.
 *
 * ******************************************
 * \attention
 * &copy; Copyright (c) 2022 FAI UTB. All rights reserved.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#ifndef IOUTILS_H_
#define IOUTILS_H_

/* Includes --------------------------------------------------------------------------------------*/
#include <stdbool.h>

/* Exported types --------------------------------------------------------------------------------*/
/* Exported macros -------------------------------------------------------------------------------*/
/* Exported variables ----------------------------------------------------------------------------*/
/* Exported functions declarations ---------------------------------------------------------------*/
bool io_utils_get_string(char *c, int maxLen);
bool io_utils_get_char(char *c);
bool io_utils_get_long(long *val);
bool io_utils_get_double(double *val);
void io_utils_clear_stdin(void);
bool io_utils_get(const char *format, ...);

#endif  //_IOUTILS_H_
