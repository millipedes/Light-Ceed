/**
 * @file   symbol_table.c
 * @brief  This file contains the functions relating to the maintainence of the
 * symbol_table data structure (the structure that stores all data about the
 * user defined data structures).
 * @author Matthew C. Lindeman
 * @date   December 24, 2022
 * @bug    None known
 * @todo   Nothing
 */
#define _POSIX_C_SOURCE 200809L // C99 Standard for strn* funcs, POSIX only
#include "include/symbol_table.h"

/**
 * This function initializes a symbol_table to have no uds.
 * @param     N/a
 * @return the_st - The newly inited symbol_table.
 */
symbol_table * init_symbol_table(void) {
  symbol_table * the_st = calloc(1, sizeof(struct SYMBOL_TABLE_T));
  the_st->all_uds = NULL;
  the_st->no_uds = 0;
  return the_st;
}

/**
 * This function adds a uds to the symbol_table.
 * @param  the_st - The symbol_table to be added to.
 * @param new_uds - The uds that will be added to the symbol_table.
 * @return the_st - The symbol_table with the uds added.
 */
symbol_table * add_symbol_table_member(symbol_table * the_st, uds * new_uds) {
  the_st->no_uds++;
  if(!the_st->all_uds)
    the_st->all_uds = calloc(1, sizeof(struct UDS_T *));
  else
    the_st->all_uds = realloc(the_st->all_uds, the_st->no_uds
        * sizeof(struct UDS_T *));
  the_st->all_uds[the_st->no_uds - 1] = new_uds;
  the_st = update_lib_requirements(the_st);
  return the_st;
}

symbol_table * update_lib_requirements(symbol_table * the_st) {
  for(int i = 0; i < the_st->no_uds; i++) {
    for(int j = 0; j < the_st->all_uds[i]->no_members; j++) {
      int found_type = 0;
      // If data type of member if not standard
      if(!the_st->all_uds[i]->members[j]->type->is_standard) {
        found_type = search_type(the_st,
            the_st->all_uds[i]->members[j]->type->literal);
        if(found_type != -1)
          the_st->all_uds[i] = add_lib(the_st->all_uds[i], relative_path(
                the_st->all_uds[found_type]->uds_fs_info,
                the_st->all_uds[i]->uds_fs_info));
      } else {
        the_st->all_uds[i] = make_req_libs(the_st->all_uds[i]);
      }
    }
  }
  return the_st;
}

int search_type(symbol_table * the_st, char * type) {
  for(int i = 0; i < the_st->no_uds; i++) {
    if(!strncmp(the_st->all_uds[i]->name, type, TYPE_MAX_SIZE))
      return i;
  }
  return -1;
}

/**
 * This function is used to debug the symbol_table.
 * @param the_st - The symbol_table to be debugged.
 * @return   N/a
 */
void debug_symbol_table(symbol_table * the_st) {
  printf("[SYMBOL_TABLE]\n");
  printf("No uds: %d\n", the_st->no_uds);
  for(int i = 0; i < the_st->no_uds; i++)
    debug_uds(the_st->all_uds[i]);
}

/**
 * This function frees a symbol_table.
 * @param the_st - The symbol_table to be freed.
 * @return   N/a
 */
void free_symbol_table(symbol_table * the_st) {
  if(the_st) {
    if(the_st->all_uds) {
      for(int i = 0; i < the_st->no_uds; i++)
        if(the_st->all_uds[i])
          free_uds(the_st->all_uds[i]);
      free(the_st->all_uds);
    }
    free(the_st);
  }
}
