/**
 * @file   symbol_table.h
 * @brief  This file contains the function definitions for symbol_table.c
 * @author Matthew C. Lindeman
 * @date   December 24, 2022
 * @bug    None known
 * @todo   Nothing
 */
#ifndef SYT_H
#define SYT_H

#include "uds.h"

typedef struct SYMBOL_TABLE_T {
  uds ** all_uds;
  int no_uds;
} symbol_table;

symbol_table * init_symbol_table(void);
symbol_table * add_symbol_table_member(symbol_table * the_st, uds * new_uds);
void debug_symbol_table(symbol_table * the_st);
void free_symbol_table(symbol_table * the_st);

#endif
