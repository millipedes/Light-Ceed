/**
 * @file   lexer.c
 * @brief  This file contains the functions the lex the .lc input file provided
 * by the user.
 * @author Matthew C. Lindeman
 * @date   December 24, 2022
 * @bug    None known
 * @todo   Nothing
 */
#define _POSIX_C_SOURCE 200809L // C99 Standard for strn* funcs, POSIX only
#include "include/lexer.h"

/**
 * This function initializes a lexer from a source buffer. It is expected that
 * the src buffer is not dynamically allocated (as it is not freed in free
 * function).
 * @param  src - The source buffer
 * @return lex - The newly initialized lexer.
 */
lexer * init_lexer(char * src) {
  lexer * lex = calloc(1, sizeof(struct LEXER_T));
  lex->src = src;
  lex->current_index = 0;
  return lex;
}

/**
 * This function takes in the input .lc file and returns a symbol_table
 * structure.
 * @param file_name - The name (full path relative to where you are exeecuting)
 * of the input file.
 * @return   the_st - The newly filled symbol_table.
 */
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

/**
 * This function is responsible for lexing a particular user defined data
 * structure. It does this by:
 *   1) lexing the name/path of the uds,
 *   2) lexing default methods,
 *   3) lexing members.
 * @param       fp - The file pointer of the input file.
 * @param   the_st - The symbol_table that is in the process of being made.
 * @return the_uds - The new uds data structure that was lexed.
 */
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

/**
 * This function lexes the name of the uds, the path of the uds (minus last
 * '/'), and the file name of the uds.
 * @param lex - The lexer containing the buffer of the name, path, and file_name
 * of the uds.
 * @return name_path_file_name -
 *   Index 0) The name
 *   Index 1) The path
 *   Index 2) The file name
 */
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

/**
 * This function lexes a method type. I.e. it determines the next procedurally
 * generated method to be generated, then advances the lexer just beyond the
 * method.
 * @param      lex - The lexer containing the methods to be generated.
 * @return MT_NULL - No more methods need to be lexed.
 *              .\ - Additional methods may exist.
 */
method_type lex_method_type(lexer * lex) {
  lex = lex_skip_whitespace_and_commas(lex);
  int starting_index = lex->current_index;

  if(lex->src[lex->current_index] == '\n')
    return MT_NULL;

  if(IS_INIT(lex->src + starting_index, INIT_SIZE)) {
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

/**
 * This function lexes a member of a user defined data structure.
 * @param    lex - The lexer which contains a member of a user defined data
 * structure.
 * @param the_st - The symbol_table which contains the other uds's.
 * @return    .\ - The uds_member that was lexed from lex.
 */
uds_member * lex_member(lexer * lex, symbol_table * the_st) {
  lex = lex_skip_whitespace(lex);
  int starting_index = lex->current_index;
  int dereference_level = 0;
  int symbol_table_index = 0;
  data_type * the_dt = NULL;
  char * the_uds_member_name = NULL;
  char * flags = NULL;
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
  while(lex->src[lex->current_index] != '\n'
      && lex->src[lex->current_index] != ':')
    lex = lex_advance(lex);
  int ending_index = lex->current_index;
  size_t len = ending_index - starting_index + 1;
  the_uds_member_name = calloc(len, sizeof(char));
  // We do not want the '\n'
  strncpy(the_uds_member_name, lex->src + starting_index, len - 1);
  if(lex->src[lex->current_index] != ':')
    return init_uds_member(the_dt, the_uds_member_name, flags);
  else {
    lex = lex_advance(lex);
    len = strnlen(lex->src, TYPE_MAX_SIZE) - lex->current_index;
    flags = calloc(len, sizeof(char));
    // '\n' hence the '- 1'
    strncpy(flags, lex->src + lex->current_index, len - 1);
    return init_uds_member(the_dt, the_uds_member_name, flags);
  }
}

/**
 * This function is used by lex_member to lex the dereference_level of a member.
 * This segment of code is repeated a couple of times and it makes sense to
 * relagate it to its own function.
 * @param               lex - The lexer whose current_index is at (or a space
 * away from) the start of the '*'s.
 * @return dereference_level - The number of '*'s.
 */
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

/**
 * This function advances the lexer's current_index by n.
 * @param  lex - The lexer to advance.
 * @param    n - The number to advance current_index by.
 * @return lex - The lexer advanced.
 */
lexer * lex_advance_n(lexer * lex, int n) {
  for(int i = 0; i < n; i++)
    lex_advance(lex);
  return lex;
}

/**
 * This function advances a lexer's current_index by 1.
 * @param  lex - The lexer to advance.
 * @return lex - The advanced lexer.
 */
lexer * lex_advance(lexer * lex) {
  lex->current_index++;
  return lex;
}

/**
 * This function advances the lexer's current_index past whitespace that is not
 * '\n'.
 * @param  lex - The lexer to be advanced.
 * @return lex - The advanced lexer.
 */
lexer * lex_skip_whitespace(lexer * lex) {
  while(lex->src[lex->current_index] == ' ' ||
      lex->src[lex->current_index] == '\t')
    lex = lex_advance(lex);
  return lex;
}

/**
 * This function advances the lexer's current_index past whitespace that is not
 * '\n' and commas.
 * @param  lex - The lexer to be advanced.
 * @return lex - The advanced lexer.
 */
lexer * lex_skip_whitespace_and_commas(lexer * lex) {
  while(lex->src[lex->current_index] == ' ' ||
      lex->src[lex->current_index] == '\t' ||
      lex->src[lex->current_index] == ',')
    lex = lex_advance(lex);
  return lex;
}

/**
 * This function is used to debug the lexer.
 * @param  lex - The lexer to be debugged.
 * @return N/a
 */
void debug_lexer(lexer * lex) {
  printf("[LEXER]\n");
  printf("Current Index: %d\n", lex->current_index);
  printf("`%s`\n", lex->src);
}

/**
 * This function frees a given lexer (see init_lexer for why src not freed).
 * @param  lex - The lexer to be freed.
 * @return N/a
 */
void free_lexer(lexer * lex) {
  if(lex)
    free(lex);
  lex = NULL;
}
