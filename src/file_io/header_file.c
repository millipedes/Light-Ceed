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
    write_header_defines(the_st->all_uds[i], fp);
    // Want a space between the defines and the libs
    fprintf(fp, "\n");
    write_libs(the_st->all_uds[i], fp);
    // Want a space between the defines and the struct
    fprintf(fp, "\n");
    write_structure_definition(the_st->all_uds[i], fp);
    // Want a space between the struct and the function definitions
    fprintf(fp, "\n");
    write_function_definitions(the_st->all_uds[i], fp);
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
 * @param the_uds - The uds to write to.
 * @param      fp - The file pointer which contains the header file being
 * written to.
 * @return    N/a
 */
void write_header_defines(uds * the_uds, FILE * fp) {
  fprintf(fp, "#ifndef ");
  for(int i = 0; i < HEADER_DEF_LEN; i++)
    // '- 32' is to capitalize the letter
    fprintf(fp, "%c", the_uds->name[i] - 32);
  fprintf(fp, "_H\n");
  fprintf(fp, "#define ");
  for(int i = 0; i < HEADER_DEF_LEN; i++)
    // '- 32' is to capitalize the letter
    fprintf(fp, "%c", the_uds->name[i] - 32);
  fprintf(fp, "_H\n");
}

/**
 * This function writes the "#include ..." for a given header file (hence the
 * i, it specifies which header file to write).
 * @param the_uds - The uds to write to.
 * @param      fp - The file pointer which contains the header file being
 * written to.
 * @return   N/a
 */
void write_libs(uds * the_uds, FILE * fp) {
  for(int i = 0; i < the_uds->no_req_libs; i++) {
    fprintf(fp, "#include ");
    if(IS_STANDARD_LIB(the_uds->req_libs[i]))
      fprintf(fp, the_uds->req_libs[i]);
    else
      fprintf(fp, "\"%s\"", the_uds->req_libs[i]);
    fprintf(fp, "\n");
  }
}

/**
 * This function writes the "typedef ... }" for a given header file (hence the
 * i, it specifies which header file to write).
 * @param the_uds - The uds to write to.
 * @param      fp - The file pointer which contains the header file being written
 * to.
 * @return    N/a
 */
void write_structure_definition(uds * the_uds, FILE * fp) {
  fprintf(fp, "typedef struct ");
  for(int i = 0; i < (int)strnlen(the_uds->name, TYPE_MAX_SIZE); i++)
    fprintf(fp, "%c", isalpha(the_uds->name[i]) ? the_uds->name[i] - 32
        : the_uds->name[i]);
  fprintf(fp, "_T {\n");
  for(int i = 0; i < the_uds->no_members; i++) {
    write_tabstop(fp);
    fprintf(fp, "%s ", the_uds->members[i]->type->literal);
    write_deref_level(the_uds, fp, i);
    fprintf(fp, "the_%s;\n", the_uds->members[i]->name);
    if(has_qty_flag(the_uds->members[i])) {
      write_tabstop(fp);
      fprintf(fp, "int qty_%s;\n", the_uds->members[i]->name);
    }
  }
  fprintf(fp, "} %s;\n", the_uds->name);
}

/**
 * This function writes the function definitions for a given header file (hence
 * the i, it specifies which header file to write).
 * @param the_uds - The uds to write to.
 * @param      fp - The file pointer which contains the header file being written
 * to.
 * @return    N/a
 */
void write_function_definitions(uds * the_uds, FILE * fp) {
  for(int j = 0; j < the_uds->no_methods; j++)
    switch(the_uds->methods[j]) {
      case INIT:
        int void_flag = 0;
        fprintf(fp, "%s * init_%s(", the_uds->name, the_uds->name);
        for(int k = 0; k < the_uds->no_members; k++) {
          if(has_init_flag(the_uds->members[k]) && !void_flag) {
            void_flag = 1;
            fprintf(fp, "%s ", the_uds->members[k]->type->literal);
            write_deref_level(the_uds, fp, k);
            fprintf(fp, "the_%s", the_uds->members[k]->name);
          } else if(has_init_flag(the_uds->members[k]) && void_flag) {
            fprintf(fp, ", %s ", the_uds->members[k]->type->literal);
            write_deref_level(the_uds, fp, k);
            fprintf(fp, "the_%s", the_uds->members[k]->name);
          }
        }
        if(!void_flag)
          fprintf(fp, "void");
        fprintf(fp, ");\n");
        break;
      case DEEP_COPY:
        fprintf(fp, "%s * deep_copy_%s(%s * dest, %s * src);\n", the_uds->name,
            the_uds->name, the_uds->name, the_uds->name);
        break;
      case FREE:
        fprintf(fp, "void free_%s(%s * the_%s);\n", the_uds->name,
            the_uds->name, the_uds->name);
        break;
      case DEBUG:
        fprintf(fp, "void debug_%s(%s * the_%s);\n", the_uds->name,
            the_uds->name, the_uds->name);
        break;
      default:
        fprintf(stderr, "[WRITE_FUNCTION_DEF]: unsupported method: %s\n",
            method_type_to_string(the_uds->methods[j]));
        exit(1);
    }
}
