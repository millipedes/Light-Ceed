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
  return the_st;
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
