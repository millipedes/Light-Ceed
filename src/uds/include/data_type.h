/**
 * @file   data_type.h
 * @brief  This function contains the function definitions for data_type.c
 * @author Matthew C. Lindeman
 * @date   December 24, 2022
 * @bug    None known
 * @todo   Nothing
 */
#ifndef DTY_H
#define DTY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../main/include/constants_macros.h"

// is x (int|double|char) ? 1 : 0
#define IS_STANDARD_TYPE(x) (!strncmp(x, "int", TYPE_MAX_SIZE) ||        \
                             !strncmp(x, "double", TYPE_MAX_SIZE) ||     \
                             !strncmp(x, "char", TYPE_MAX_SIZE)) ? 1 : 0
#define IS_INT(x, l)      (!strncmp(x, "int",    l)) ? 1 : 0
#define IS_DOUBLE(x, l)   (!strncmp(x, "double", l)) ? 1 : 0
#define IS_CHAR(x, l)     (!strncmp(x, "char",   l)) ? 1 : 0
#define IS_UDS(x, u, l)   (!strncmp(x, u,        l)) ? 1 : 0

#define INT_SIZE 3
#define DOUBLE_SIZE 6
#define CHAR_SIZE 4

typedef struct DATA_TYPE_T {
  char * literal;
  int is_standard;
  int dereference_level;
} data_type;

data_type * init_data_type(char * literal, int dereference_level);
void debug_data_type(data_type * the_dt);
void free_data_type(data_type * the_dt);

#endif
