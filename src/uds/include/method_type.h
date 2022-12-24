#ifndef MDT_H
#define MDT_H

#include <stdlib.h>

#define IS_INIT(x, l) !strncmp(x, method_type_to_string(INIT), l) ? 1 : 0
#define IS_DEBUG(x, l) !strncmp(x, method_type_to_string(DEBUG), l) ? 1 : 0
#define IS_ADD(x, l) !strncmp(x, method_type_to_string(ADD), l) ? 1 : 0
#define IS_DEEP_COPY(x, l) !strncmp(x, method_type_to_string(DEEP_COPY), l) \
  ? 1 : 0
#define IS_FREE(x, l) !strncmp(x, method_type_to_string(FREE), l) ? 1 : 0

#define ADD_SIZE 3
#define INIT_SIZE 4
#define DEBUG_SIZE 5
#define DEEP_COPY_SIZE 9
#define FREE_SIZE 4

typedef enum {
  INIT,
  DEBUG,
  DEEP_COPY,
  FREE,
  // We need this for lex to tell the EOL:
  MT_NULL,
} method_type;

const char * method_type_to_string(method_type the_mt);

#endif
