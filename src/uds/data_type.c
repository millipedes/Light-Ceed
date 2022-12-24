#define _POSIX_C_SOURCE 200809L // C99 Standard for strn* funcs, POSIX only
#include "include/data_type.h"

data_type * init_data_type(char * literal, int dereference_level) {
  data_type * the_dt = calloc(1, sizeof(struct DATA_TYPE_T));
  // '+ 1' for Nul terminator
  size_t len = strnlen(literal, TYPE_MAX_SIZE) + 1;
  the_dt->literal = calloc(len, sizeof(char));
  strncpy(the_dt->literal, literal, len);
  the_dt->is_standard = IS_STANDARD_TYPE(the_dt->literal);
  the_dt->dereference_level = dereference_level;
  return the_dt;
}

void debug_data_type(data_type * the_dt) {
  printf("[DATA_TYPE]\n");
  printf("literal: `%s`\n", the_dt->literal);
  printf("%s\n", the_dt->is_standard ? "Standard" : "Not Standard");
  printf("Dereference Level: %d\n", the_dt->dereference_level);
}

void free_data_type(data_type * the_dt) {
  if(the_dt) {
    if(the_dt->literal)
      free(the_dt->literal);
    free(the_dt);
  }
}
