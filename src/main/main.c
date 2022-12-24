#include "../lexer/include/lexer.h"
#include "../file_io/include/file_io.h"

int main(void) {
  symbol_table * the_st = lex_symbol_table("idata/test.lc");
  // debug_symbol_table(the_st);
  write_symbol_table(the_st);
  free_symbol_table(the_st);
  return 0;
}
