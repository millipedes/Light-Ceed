#include "../lexer/include/lexer.h"

int main(void) {
  symbol_table * the_st = lex_symbol_table("idata/test.lc");
  debug_symbol_table(the_st);
  free_symbol_table(the_st);

  // lexer * lex = init_lexer("init, debug, deep_copy, free\n");
  // uds_member * uds_m = lex_member(lex, st);
  // debug_uds_member(uds_m);
  // free_uds_member(uds_m);
  // printf("%s\n", method_type_to_string(lex_method_type(lex)));
  // printf("%s\n", method_type_to_string(lex_method_type(lex)));
  // printf("%s\n", method_type_to_string(lex_method_type(lex)));
  // printf("%s\n", method_type_to_string(lex_method_type(lex)));
  // printf("%s\n", method_type_to_string(lex_method_type(lex)));
  // free_lexer(lex);
  return 0;
}
