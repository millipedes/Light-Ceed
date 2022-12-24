#ifndef MDT_H
#define MDT_H

#include <stdlib.h>

typedef enum {
  INIT,
  DEBUG,
  ADD,
  DEEP_COPY,
  FREE,
} method_type;

const char * method_type_to_string(method_type the_mt);

#endif
