#include "../lexer/include/lexer.h"

int main(void) {
  lexer * lex = init_lexer("free, init");
  printf("%s\n", method_type_to_string(lex_method_type(lex)));
  printf("%s\n", method_type_to_string(lex_method_type(lex)));
  printf("%s\n", method_type_to_string(lex_method_type(lex)));
  free_lexer(lex);
  return 0;
}
