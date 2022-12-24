/**
 * @file   libs.h
 * @brief  This file contains the function definitions for libs.c
 * @author Matthew C. Lindeman
 * @date   December 24, 2022
 * @bug    None known
 * @todo   None.
 */
#ifndef LIBS_H
#define LIBS_H

#include <stdlib.h>

typedef enum {
  STDIO,
  STDLIB,
  STRING,
} libs;

const char * libs_to_string(libs the_lib);

#endif
