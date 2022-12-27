/**
 * @file   header_file.c
 * @brief  This file contains the functions required to write the header file(s)
 * specified by the symbol_table.
 * @author Matthew C. Lindeman
 * @date   December 24, 2022
 * @bug    None known
 * @todo   Nothing
 */
#define _POSIX_C_SOURCE 200809L // C99 Standard for strn* funcs, POSIX only
#include "include/header_file.h"

/**
 * This function writes the header files specified by the symbol_table. It does
 * this via calling other functions that are responsible for writing
 * sub-portions of a header file for each header file that needs to be written.
 * @param the_st - The symbol_table that has the specified header files.
 * @return   N/a
 */
void write_header_file(symbol_table * the_st) {
  write_directories(the_st);
  FILE * fp = NULL;
  size_t path_len = 0;
  size_t h_len = 0;
  size_t total_len = 0;
  char * path = NULL;
  for(int i = 0; i < the_st->no_uds; i++) {
    path_len
      = strnlen(the_st->all_uds[i]->uds_fs_info->each_sub_dir
          [the_st->all_uds[i]->uds_fs_info->qty_sub_dirs - 1], TYPE_MAX_SIZE);
    h_len = strnlen(the_st->all_uds[i]->uds_fs_info->h_file_name, TYPE_MAX_SIZE);
    total_len = path_len + h_len + 2;
    path = calloc(total_len, sizeof(char));
    strncpy(path, the_st->all_uds[i]->uds_fs_info->each_sub_dir
          [the_st->all_uds[i]->uds_fs_info->qty_sub_dirs - 1], path_len);
    strncat(path, "/", h_len);
    strncat(path, the_st->all_uds[i]->uds_fs_info->h_file_name, h_len);
    fp = fopen(path, "w");
    write_header_defines(the_st, fp, i);
    // Want a space between the defines and the libs
    fprintf(fp, "\n");
    write_libs(the_st, fp, i);
    // Want a space between the defines and the struct
    fprintf(fp, "\n");
    write_structure_definition(the_st, fp, i);
    // Want a space between the struct and the function definitions
    fprintf(fp, "\n");
    write_function_definitions(the_st, fp, i);
    fprintf(fp, "\n");
    fprintf(fp, "#endif\n");
    free(path);
    path = NULL;
    fclose(fp);
    fp = NULL;
  }
}

/**
 * This function writes the directories required to the HD.
 * @param the_st - The symbol_table which contains the uds which have the
 * directories that need to be written.
 * @return   N/a
 */
void write_directories(symbol_table * the_st) {
  struct stat dir_status = {0};
  for(int i = 0; i < the_st->no_uds; i++)
    for(int j = 0; j < the_st->all_uds[i]->uds_fs_info->qty_sub_dirs; j++)
      if(stat(the_st->all_uds[i]->uds_fs_info->each_sub_dir[j], &dir_status)
          == -1)
        if(mkdir(the_st->all_uds[i]->uds_fs_info->each_sub_dir[j], 0755) != 0)
          fprintf(stderr, "[WRITE_DIRECTORIES]: %s\n", strerror(errno));
}

/**
 * This function writes the "#ifndef ..." for a given header file (hence the
 * i, it specifies which header file to write).
 * @param the_st - The symbol_table to write to.
 * @param     fp - The file pointer which contains the header file being written
 * to.
 * @param      i - The index of the uds spec whose header file is being written.
 * @return   N/a
 */
void write_header_defines(symbol_table * the_st, FILE * fp, int i) {
  fprintf(fp, "#ifndef ");
  for(int j = 0; j < HEADER_DEF_LEN; j++)
    fprintf(fp, "%c", the_st->all_uds[i]->name[j] - 32);
  fprintf(fp, "_H\n");
  fprintf(fp, "#define ");
  for(int j = 0; j < HEADER_DEF_LEN; j++)
    fprintf(fp, "%c", the_st->all_uds[i]->name[j] - 32);
  fprintf(fp, "_H\n");
}

/**
 * This function writes the "#include ..." for a given header file (hence the
 * i, it specifies which header file to write).
 * @param the_st - The symbol_table to write to.
 * @param     fp - The file pointer which contains the header file being written
 * to.
 * @param      i - The index of the uds spec whose header file is being written.
 * @return   N/a
 */
void write_libs(symbol_table * the_st, FILE * fp, int i) {
  for(int j = 0; j < the_st->all_uds[i]->no_req_libs; j++) {
    fprintf(fp, "#include ");
    if(IS_STANDARD_LIB(the_st->all_uds[i]->req_libs[j]))
      fprintf(fp, the_st->all_uds[i]->req_libs[j]);
    else
      fprintf(fp, "\"%s\"", the_st->all_uds[i]->req_libs[j]);
    fprintf(fp, "\n");
  }
}

/**
 * This function writes the "typedef ... }" for a given header file (hence the
 * i, it specifies which header file to write).
 * @param the_st - The symbol_table to write to.
 * @param     fp - The file pointer which contains the header file being written
 * to.
 * @param      i - The index of the uds spec whose header file is being written.
 * @return   N/a
 */
void write_structure_definition(symbol_table * the_st, FILE * fp, int i) {
  fprintf(fp, "typedef struct ");
  for(int j = 0; j < (int)strnlen(the_st->all_uds[i]->name, TYPE_MAX_SIZE); j++)
    fprintf(fp, "%c", the_st->all_uds[i]->name[j] - 32);
  fprintf(fp, "_T {\n");
  for(int j = 0; j < the_st->all_uds[i]->no_members; j++) {
    write_tabstop(fp);
    fprintf(fp, "%s ", the_st->all_uds[i]->members[j]->type->literal);
    write_deref_level(the_st, fp, i, j);
    fprintf(fp, "the_%s;\n", the_st->all_uds[i]->members[j]->name);
    if(has_qty_flag(the_st->all_uds[i]->members[j])) {
      write_tabstop(fp);
      fprintf(fp, "int qty_%s;\n", the_st->all_uds[i]->members[j]->name);
    }
  }
  fprintf(fp, "} %s;\n", the_st->all_uds[i]->name);
}

/**
 * This function writes the function definitions for a given header file (hence
 * the i, it specifies which header file to write).
 * @param the_st - The symbol_table to write to.
 * @param     fp - The file pointer which contains the header file being written
 * to.
 * @param      i - The index of the uds spec whose header file is being written.
 * @return   N/a
 */
void write_function_definitions(symbol_table * the_st, FILE * fp, int i) {
  for(int j = 0; j < the_st->all_uds[i]->no_methods; j++) {
    switch(the_st->all_uds[i]->methods[j]) {
      case INIT:
        int void_flag = 0;
        fprintf(fp, "%s * init_%s(", the_st->all_uds[i]->name,
            the_st->all_uds[i]->name);
        for(int k = 0; k < the_st->all_uds[i]->no_members; k++) {
          if(has_init_flag(the_st->all_uds[i]->members[k])
              && !void_flag) {
            void_flag = 1;
            fprintf(fp, "%s ", the_st->all_uds[i]->members[k]->type->literal);
            write_deref_level(the_st, fp, i, k);
            fprintf(fp, "the_%s", the_st->all_uds[i]->members[k]->name);
          } else if(has_init_flag(the_st->all_uds[i]->members[k])
              && void_flag) {
            fprintf(fp, ", %s ", the_st->all_uds[i]->members[k]->type->literal);
            write_deref_level(the_st, fp, i, k);
            fprintf(fp, "the_%s", the_st->all_uds[i]->members[k]->name);
          }
        }
        if(!void_flag)
          fprintf(fp, "void");
        fprintf(fp, ");\n");
        break;
      case DEEP_COPY:
        fprintf(fp, "%s * deep_copy_%s(%s * dest, %s * src);\n",
            the_st->all_uds[i]->name, the_st->all_uds[i]->name,
            the_st->all_uds[i]->name, the_st->all_uds[i]->name);
        break;
      case FREE:
        fprintf(fp, "void free_%s(%s * the_%s);\n", the_st->all_uds[i]->name,
            the_st->all_uds[i]->name, the_st->all_uds[i]->name);
        break;
      case DEBUG:
        fprintf(fp, "void debug_%s(%s * the_%s);\n", the_st->all_uds[i]->name,
            the_st->all_uds[i]->name, the_st->all_uds[i]->name);
        break;
      default:
        fprintf(stderr, "[WRITE_FUNCTION_DEF]: unsupported method: %s\n",
            method_type_to_string(the_st->all_uds[i]->methods[j]));
        exit(1);
    }
  }
}

/**
 * This function writes the dereference_level for uds of index i's member of
 * index j. This function is repeated several times, makes sense to be relegated
 * to its own function
 * @param the_st - The symbol_table to write to.
 * @param     fp - The file pointer which contains the header file being written
 * to.
 * @param      i - The index of the uds spec whose header file is being written.
 * @param      j - The index of the uds's member who is being written.
 * @return   N/a
 */
void write_deref_level(symbol_table * the_st, FILE * fp, int i, int j) {
  for(int k = 0; k < the_st->all_uds[i]->members[j]->type->dereference_level;
      k++) {
    fprintf(fp, "*");
    if(k == the_st->all_uds[i]->members[j]->type->dereference_level - 1)
      fprintf(fp, " ");
  }
}

/**
 * This function writes a tabstop to the given fp. This is controled via TABSTOP
 * in the header file.
 * @param   fp - The file pointed to write the tabstop to.
 * @return N/a
 */
void write_tabstop(FILE * fp) {
  for(int i = 0; i < TABSTOP; i++)
    fprintf(fp, " ");
}