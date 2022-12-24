#ifndef DTY_H
#define DTY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../main/include/constants_macros.h"

// is x (int|double|char) ? 1 : 0
#define IS_STANDARD_TYPE(x) (!strncmp(x, "int", TYPE_MAX_SIZE) ||        \
                             !strncmp(x, "double", TYPE_MAX_SIZE) ||     \
                             !strncmp(x, "char", TYPE_MAX_SIZE)) ? 1 : 0 \

typedef struct DATA_TYPE_T {
  char * literal;
  int is_standard;
  int dereference_level;
} data_type;

data_type * init_data_type(char * literal, int dereference_level);
void debug_data_type(data_type * the_dt);
int is_standard_type(char * literal);
void free_data_type(data_type * the_dt);

#endif
