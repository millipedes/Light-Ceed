#include "../lexer/include/lexer.h"

int main(void) {
  symbol_table * st = init_symbol_table();
  uds * the_uds = lex_uds("idata/test.lc", st);
  debug_uds(the_uds);
  free_uds(the_uds);
  free_symbol_table(st);

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
