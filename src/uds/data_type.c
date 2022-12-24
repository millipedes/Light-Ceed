/**
 * @file   data_type.c
 * @brief  This function contains the functions relating to a data type for a
 * given member of a uds.
 * @author Matthew C. Lindeman
 * @date   December 24, 2022
 * @bug    None known
 * @todo   Nothing
 */
#define _POSIX_C_SOURCE 200809L // C99 Standard for strn* funcs, POSIX only
#include "include/data_type.h"

/**
 * This function initializes a data type with the given literal and
 * dereference_level.
 * @param           literal - The literal type of the data type (this value is
 * deep copied, user responsible for freeing literal if it needs to be freed).
 * @param dereference_level - The dereference_level of the data type.
 * @return
 */
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

/**
 * This function debugs a given data type.
 * @param the_dt - The data type to be debugged.
 * @return   N/a
 */
void debug_data_type(data_type * the_dt) {
  printf("[DATA_TYPE]\n");
  printf("literal: `%s`\n", the_dt->literal);
  printf("%s\n", the_dt->is_standard ? "Standard" : "Not Standard");
  printf("Dereference Level: %d\n", the_dt->dereference_level);
}

/**
 * This function frees a given data type.
 * @param the_dt - The data type to be freed.
 * @return   N/a
 */
void free_data_type(data_type * the_dt) {
  if(the_dt) {
    if(the_dt->literal)
      free(the_dt->literal);
    free(the_dt);
  }
}
