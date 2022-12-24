#ifndef LEX_H
#define LEX_H

#include "../../uds/include/symbol_table.h"

typedef struct LEXER_T {
  char * src;
  int current_index;
} lexer;

lexer * init_lexer(char * src);
symbol_table * lex_symbol_table(char * path);
uds * lex_uds(lexer * lex);
method_type lex_method_type(lexer * lex);
uds_member * lex_member(lexer * lex, symbol_table * the_st);
int lex_dereference_level(lexer * lex);
lexer * lex_advance(lexer * lex);
lexer * lex_advance_n(lexer * lex, int n);
lexer * lex_skip_whitespace(lexer * lex);
void debug_lexer(lexer * lex);
void free_lexer(lexer * lex);

#endif
