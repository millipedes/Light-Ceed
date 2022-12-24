#define _POSIX_C_SOURCE 200809L // C99 Standard for strn* funcs, POSIX only
#include "include/lexer.h"

lexer * init_lexer(char * src) {
  lexer * lex = calloc(1, sizeof(struct LEXER_T));
  lex->src = src;
  lex->current_index = 0;
  return lex;
}

symbol_table * lex_symbol_table(char * file_name) {
  FILE * fp = fopen(file_name, "r");
  symbol_table * the_st = init_symbol_table();
  uds * tmp_uds = NULL;
  do {
    tmp_uds = lex_uds(fp, the_st);
    if(tmp_uds)
      the_st = add_symbol_table_member(the_st, tmp_uds);
  } while(tmp_uds);
  fclose(fp);
  return the_st;
}

uds * lex_uds(FILE * fp, symbol_table * the_st) {
  method_type tmp_mt = INIT;
  method_type * method_types = NULL;
  uds * the_uds = NULL;
  int no_method_types = 0;
  char buffer[TYPE_MAX_SIZE];
  if(!fgets(buffer, TYPE_MAX_SIZE, fp))
    return NULL;
  while(buffer[0] == '\n')
    fgets(buffer, TYPE_MAX_SIZE, fp);
  lexer * lex = init_lexer(buffer);
  char ** npfn = lex_name_path_file_name(lex);
  free_lexer(lex);
  fgets(buffer, TYPE_MAX_SIZE, fp);
  lex = init_lexer(buffer);
  while(tmp_mt != MT_NULL) {
    tmp_mt = lex_method_type(lex);
    if(tmp_mt != MT_NULL) {
      no_method_types++;
      if(!method_types)
        method_types = calloc(1, sizeof(method_type));
      else
        method_types = realloc(method_types, no_method_types *
            sizeof(method_type));
      method_types[no_method_types - 1] = tmp_mt;
    }
  }
  free_lexer(lex);
  the_uds = init_uds(npfn[0], npfn[1], npfn[2], method_types, no_method_types);
  while(fgets(buffer, TYPE_MAX_SIZE, fp)) {
    lex = init_lexer(buffer);
    if(lex->src[0] != '}')
      add_uds_member(the_uds, lex_member(lex, the_st));
    else {
      fgets(buffer, TYPE_MAX_SIZE, fp);
      free_lexer(lex);
      break;
    }
    free_lexer(lex);
  }
  free(npfn);
  return the_uds;
}

char ** lex_name_path_file_name(lexer * lex) {
  int last_fs = 0;
  int name_end = 0;
  char ** name_path_file_name = calloc(3, sizeof(char **));
  /**
   *     _       _
   * nam|e| path|/|file_name
   *               _
   * First set of | |'s is name_end, and second is end of path. This is all we
   * need to get all three required fields from the input file.
   */
  while(lex->src[lex->current_index] != '{') {
    if(lex->src[lex->current_index] == ' ' && !name_end)
      name_end = lex->current_index - 1;
    if(lex->src[lex->current_index] == '/')
      last_fs = lex->current_index;
    lex_advance(lex);
  }
  size_t len = (size_t)name_end + 2;
  name_path_file_name[0] = calloc(len, sizeof(char));
  strncpy(name_path_file_name[0], lex->src, len - 1);
  len = (size_t)(last_fs - (name_end + 2) + 1);
  name_path_file_name[1] = calloc(len, sizeof(char));
  strncpy(name_path_file_name[1], lex->src + name_end + 2, len - 1);
  len = (size_t)(lex->current_index - (last_fs + 1));
  name_path_file_name[2] = calloc(len, sizeof(char));
  strncpy(name_path_file_name[2], lex->src + last_fs + 1, len - 1);
  return name_path_file_name;
}

method_type lex_method_type(lexer * lex) {
  lex = lex_skip_whitespace_and_commas(lex);
  int starting_index = lex->current_index;

  if(lex->src[lex->current_index] == '\n')
    return MT_NULL;

  /**
   * If it is an "add" default method, additional processing is required and
   * we do not just skip the ', '.
   *
   * @TODO add this functionality across program
   */
  if(IS_ADD(lex->src + starting_index, ADD_SIZE)) {
    lex = lex_advance_n(lex, ADD_SIZE);
    return ADD;
  } else if(IS_INIT(lex->src + starting_index, INIT_SIZE)) {
    lex = lex_advance_n(lex, INIT_SIZE);
    return INIT;
  } else if(IS_DEBUG(lex->src + starting_index, DEBUG_SIZE)) {
    lex = lex_advance_n(lex, DEBUG_SIZE);
    return DEBUG;
  } else if(IS_DEEP_COPY(lex->src + starting_index, DEEP_COPY_SIZE)) {
    lex = lex_advance_n(lex, DEEP_COPY_SIZE);
    return DEEP_COPY;
  } else if(IS_FREE(lex->src + starting_index, FREE_SIZE)) {
    lex = lex_advance_n(lex, FREE_SIZE);
    return FREE;
  }
  fprintf(stderr, "Unrecognized Standard Method: `%s`\n",
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
    lex = lex_advance_n(lex, INT_SIZE); 
    dereference_level = lex_dereference_level(lex);
    the_dt = init_data_type("int", dereference_level);
  } else if(IS_DOUBLE(lex->src + starting_index, DOUBLE_SIZE)) {
    lex = lex_advance_n(lex, DOUBLE_SIZE); 
    dereference_level = lex_dereference_level(lex);
    the_dt = init_data_type("double", dereference_level);
  } else if(IS_CHAR(lex->src + starting_index, CHAR_SIZE)) {
    lex = lex_advance_n(lex, CHAR_SIZE); 
    dereference_level = lex_dereference_level(lex);
    the_dt = init_data_type("char", dereference_level);
  } else {
    for(int i = 0; i < the_st->no_uds; i++)
      if(IS_UDS(lex->src + starting_index, the_st->all_uds[i]->name,
            strnlen(the_st->all_uds[i]->name, TYPE_MAX_SIZE))) {
        lex = lex_advance_n(lex, strnlen(the_st->all_uds[i]->name,
              TYPE_MAX_SIZE));
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
  return lex;
}

lexer * lex_advance(lexer * lex) {
  lex->current_index++;
  return lex;
}

lexer * lex_skip_whitespace(lexer * lex) {
  while(lex->src[lex->current_index] == ' ' ||
      lex->src[lex->current_index] == '\t')
    lex = lex_advance(lex);
  return lex;
}

lexer * lex_skip_whitespace_and_commas(lexer * lex) {
  while(lex->src[lex->current_index] == ' ' ||
      lex->src[lex->current_index] == '\t' ||
      lex->src[lex->current_index] == ',')
    lex = lex_advance(lex);
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
