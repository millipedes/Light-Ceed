#define _POSIX_C_SOURCE 200809L // C99 Standard for strn* funcs, POSIX only
#include "include/data_type.h"

data_type * init_data_type(char * literal, int dereference_level) {
  data_type * the_dt = calloc(1, sizeof(struct DATA_TYPE_T));
  the_dt->literal = literal;
  the_dt->is_standard = IS_STANDARD_TYPE(the_dt->literal);
  the_dt->dereference_level = dereference_level;
  return the_dt;
}

void debug_data_type(data_type * the_dt) {
  printf("[DATA_TYPE]\n");
  printf("literal: %s", the_dt->literal);
}

void free_data_type(data_type * the_dt) {
  if(the_dt) {
    if(the_dt->literal)
      free(the_dt->literal);
    free(the_dt);
  }
}
