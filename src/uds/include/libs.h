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
