#include "../lexer/include/lexer.h"

int main(void) {
  lexer * lex = init_lexer("double **** test\n");
  symbol_table * st = init_symbol_table();
  uds_member * uds_m = lex_member(lex, st);
  debug_uds_member(uds_m);
  free_uds_member(uds_m);
  // printf("%s\n", method_type_to_string(lex_method_type(lex)));
  // printf("%s\n", method_type_to_string(lex_method_type(lex)));
  // printf("%s\n", method_type_to_string(lex_method_type(lex)));
  free_lexer(lex);
  free_symbol_table(st);
  return 0;
}
