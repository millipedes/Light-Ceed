#include "include/symbol_table.h"

symbol_table * init_symbol_table(void) {
  symbol_table * the_st = calloc(1, sizeof(struct SYMBOL_TABLE_T));
  the_st->all_uds = NULL;
  the_st->no_uds = 0;
  return the_st;
}

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

void debug_symbol_table(symbol_table * the_st) {
  printf("[SYMBOL_TABLE]\n");
  printf("No uds: %d\n", the_st->no_uds);
  for(int i = 0; i < the_st->no_uds; i++)
    debug_uds(the_st->all_uds[i]);
}

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
