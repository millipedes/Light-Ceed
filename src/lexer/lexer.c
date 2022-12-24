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

uds_member * lex_member(lexer * lex, symbol_table * the_st) {
  lex = lex_skip_whitespace(lex);
  int starting_index = lex->current_index;
  int dereference_level = 0;
  int symbol_table_index = 0;
  data_type * the_dt = NULL;
  char * the_uds_member_name = NULL;
  /**
   * 1) Determine the type
   * 2) advance lexer past base type
   * 3) check for dereference_level (if so get it then advance)
   * 4) init data_type from dereference_level and type
   */
  if(IS_INT(lex->src + starting_index, INT_SIZE)) {
    lex_advance_n(lex, INT_SIZE); 
    dereference_level = lex_dereference_level(lex);
    the_dt = init_data_type("int", dereference_level);
  } else if(IS_DOUBLE(lex->src + starting_index, DOUBLE_SIZE)) {
    lex_advance_n(lex, DOUBLE_SIZE); 
    dereference_level = lex_dereference_level(lex);
    the_dt = init_data_type("double", dereference_level);
  } else if(IS_CHAR(lex->src + starting_index, CHAR_SIZE)) {
    lex_advance_n(lex, CHAR_SIZE); 
    dereference_level = lex_dereference_level(lex);
    the_dt = init_data_type("char", dereference_level);
  } else {
    for(int i = 0; i < the_st->no_uds; i++)
      if(IS_UDS(lex->src + starting_index, the_st->all_uds[i]->name,
            strnlen(the_st->all_uds[i]->name, TYPE_MAX_SIZE))) {
        lex_advance_n(lex, strnlen(the_st->all_uds[i]->name, TYPE_MAX_SIZE));
        dereference_level = lex_dereference_level(lex);
        the_dt = init_data_type(the_st->all_uds[symbol_table_index]->name,
            dereference_level);
        symbol_table_index = i;
        break;
      }
  }
  starting_index = lex->current_index;
  while(lex->src[lex->current_index] != '\n')
    lex_advance(lex);
  int ending_index = lex->current_index;
  size_t len = ending_index - starting_index + 1;
  the_uds_member_name = calloc(len, sizeof(char));
  // We do not want the '\n'
  strncpy(the_uds_member_name, lex->src + starting_index, len - 1);
  return init_uds_member(the_dt, the_uds_member_name);
}

int lex_dereference_level(lexer * lex) {
  int dereference_level = 0;
  if(lex->src[lex->current_index] == ' ')
    lex_advance(lex);
  if(lex->src[lex->current_index] == '*') {
    for( ; lex->src[lex->current_index] == '*'; dereference_level++)
      lex_advance(lex);
    // ' ' after the dereference_level if user put one
    if(lex->src[lex->current_index] == ' ')
      lex_advance(lex);
  }
  return dereference_level;
}

lexer * lex_advance_n(lexer * lex, int n) {
  for(int i = 0; i < n; i++)
    lex_advance(lex);
}

lexer * lex_advance(lexer * lex) {
  lex->current_index++;
  return lex;
}

lexer * lex_skip_whitespace(lexer * lex) {
  while(lex->src[lex->current_index] == ' ' ||
      lex->src[lex->current_index] == '\t')
    lex_advance(lex);
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
