/**
 * @file   lexer.h
 * @brief  This file contains the function definitions for lexer.c
 * @author Matthew C. Lindeman
 * @date   December 24, 2022
 * @bug    None known
 * @todo   Nothing
 */
#ifndef LEX_H
#define LEX_H

#include "../../uds/include/symbol_table.h"

typedef struct LEXER_T {
  char * src;
  int current_index;
} lexer;

lexer * init_lexer(char * src);
symbol_table * lex_symbol_table(char * file_name);
uds * lex_uds(FILE * fp, symbol_table * the_st);
char ** lex_name_path_file_name(lexer * lex);
method_type lex_method_type(lexer * lex);
uds_member * lex_member(lexer * lex, symbol_table * the_st);
int lex_dereference_level(lexer * lex);
lexer * lex_advance(lexer * lex);
lexer * lex_advance_n(lexer * lex, int n);
lexer * lex_skip_whitespace(lexer * lex);
lexer * lex_skip_whitespace_and_commas(lexer * lex);
void debug_lexer(lexer * lex);
void free_lexer(lexer * lex);

#endif
