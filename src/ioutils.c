/*!
 * \file    ioutils.c
 * \author  Dulik, Jurena
 * \date    16.2.2012
 * \brief   Module for loading the input from user.
 * ******************************************
 * \attention
 * &copy; Copyright (c) 2022 FAI UTB. All rights reserved.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

/*! \defgroup io_utils IO utils
 *
 *  \brief Module for loading data from the user using the standard input.
 *
 *  \{
 */
/* Includes --------------------------------------------------------------------------------------*/
#include "ioutils.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Private types ---------------------------------------------------------------------------------*/
/* Private macros --------------------------------------------------------------------------------*/
/* Private variables -----------------------------------------------------------------------------*/
/* Private function declarations -----------------------------------------------------------------*/
static bool get_string(char *line, int len);

/* Exported functions definitions ----------------------------------------------------------------*/
/*! Read one line from stdin.
 *
 * \param[out]  c       Pointer to array where the result is stored.
 * \param[in]   maxLen  Maximum length of loaded string.
 *
 * \return Return true on success, false otherwise.
 */
bool io_utils_get_string(char *c, int maxLen)
{
  if (c == NULL) {
    return false;
  }

  return get_string(c, maxLen);
}

/*! Loads one character from standard input.
 *
 *  \param[out] c Pointer to the character where it the loaded one will be
 * stored.
 *
 * \return Return true on success, false otherwise.
 */
bool io_utils_get_char(char *c)
{
  return io_utils_get("%c", c);
}

/*! Loads a long type value from standard input.
 *
 * \param[out]  val Pointer to a long type variable where the loaded result is
 * stored.
 *
 * \return Return true on success, false otherwise.
 */
bool io_utils_get_long(long *val)
{
  return io_utils_get("%ld", val);
}

/*! Loads double type variable from standard input. If the read value is not
 * validated as double number the reading is repeated until file ends or the
 * value is loaded.
 *
 * \param[out] val Pointer to a double type variable where the result is
 * stored.
 *
 * \return Return true on success, false otherwise.
 */
bool io_utils_get_double(double *val)
{
  return io_utils_get("%lf", val);
}

/*! Universal call for reading the user input. The function counts the percent
 * signs in \a format argument and compare it to the loaded count. If the values
 * does not match, the new input prompt is requested.
 *
 * \param[in] format  The format expected to be read.
 *
 * \return Return true on success, false otherwise.
 */
bool io_utils_get(const char *format, ...)
{
  int ok;
  char line[BUFSIZ];

  int expectedResult = 0;
  char *percPos = strchr(format, '%');
  while (percPos != NULL) {
    ++expectedResult;
    percPos = strchr(percPos + 1, '%');
  }

  do {
    if (get_string(line, sizeof(line)) == false) {
      return false;
    }

    va_list args;
    va_start(args, format);
    ok = vsscanf(line, format, args);
    va_end(args);

    if (ok != expectedResult) {
      printf("Incorrect format, try again.\n");
    }
  } while (ok != expectedResult);

  return true;
}

/*! Clears one line of input. */
void io_utils_clear_stdin(void)
{
  while (fgetc(stdin) != '\n') {}
}

/* Private function definitions ------------------------------------------------------------------*/

/*! Loads a string from standard input that has its maximum length specified by
 * a parameter \a len or until the end-of-line character (\\n) is encountered.
 *
 * \param[out]  line    Pointer to a text array containing the loaded input.
 * \param[in]   len     Maximum lenght of loaded input.
 *
 * \return Returns true if line is loaded, false otherwise.
 */
static bool get_string(char *line, int len)
{
  if (fgets(line, len, stdin) == NULL) {
    printf("End of file, finishing.\n");
    return false;
  }

#ifdef ECHO
  printf("%s", line);
#endif

  return true;
}

/*! \} */