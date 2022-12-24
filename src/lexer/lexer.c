#define _POSIX_C_SOURCE 200809L // C99 Standard for strn* funcs, POSIX only
#include "include/lexer.h"

lexer * init_lexer(char * src) {
  lexer * lex = calloc(1, sizeof(struct LEXER_T));
  lex->src = src;
  lex->current_index = 0;
  return lex;
}

symbol_table * lex_symbol_table(char * path) {
}

uds * lex_uds(lexer * lex) {
}

method_type lex_method_type(lexer * lex) {
  int starting_index = lex->current_index;
  int ending_index = 0;

  if(lex->src[lex->current_index] == '\n')
    return MT_NULL;

  while(lex->src[lex->current_index] != ',' &&
      lex->src[lex->current_index] != '\n') {
    lex_advance(lex);
    ending_index = lex->current_index - 1;
    /**
     * If it is an "add" default method, additional processing is required and
     * we do not just skip the ', '.
     *
     * @TODO add this functionality across program
     */
    if(IS_ADD(lex->src + starting_index, ending_index - starting_index))
      return ADD;
  }

  // Get it past the ', '
  if(lex->src[lex->current_index] == ',')
    lex_advance(lex);
  if(lex->src[lex->current_index] == ' ')
    lex_advance(lex);

  if(IS_INIT(lex->src + starting_index, ending_index - starting_index))
    return INIT;
  if(IS_DEBUG(lex->src + starting_index, ending_index - starting_index))
    return DEBUG;
  if(IS_DEEP_COPY(lex->src + starting_index, ending_index - starting_index))
    return DEEP_COPY;
  if(IS_FREE(lex->src + starting_index, ending_index - starting_index))
    return FREE;
  fprintf(stderr, "Unrecognized Standard Method: %s\n",
      lex->src + starting_index);
  exit(1);
}

uds_member * lex_member(lexer * lex) {
}

lexer * lex_advance(lexer * lex) {
  lex->current_index++;
  return lex;
}

void debug_lexer(lexer * lex) {
  printf("[LEXER]\n");
  printf("Current Index: %d\n", lex->current_index);
  printf("`%s`\n", lex->src);
}

void free_lexer(lexer * lex) {
  if(lex)
    free(lex);
  lex = NULL;
}
