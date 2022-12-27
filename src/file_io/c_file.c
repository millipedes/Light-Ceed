/**
 * @file   c_file.c
 * @brief  This file writes the c files for the give symbol_table.
 * @author Matthew C. Lindeman
 * @date   December 27, 2022
 * @bug    None known
 * @todo   Nothing
 */
#define _POSIX_C_SOURCE 200809L // C99 Standard for strn* funcs, POSIX only
#include "include/c_file.h"

/**
 * This function writes a c file for the given symbol_table.
 * @param the_st - The symbol_table from which the c files are written.
 * @return   N/a
 */
void write_c_file(symbol_table * the_st) {
  FILE * fp = NULL;
  size_t path_len = 0;
  size_t c_len = 0;
  size_t total_len = 0;
  char * path = NULL;
  for(int i = 0; i < the_st->no_uds; i++) {
    path_len
      = strnlen(the_st->all_uds[i]->uds_fs_info->each_sub_dir
          [the_st->all_uds[i]->uds_fs_info->qty_sub_dirs - 2], TYPE_MAX_SIZE);
    c_len = strnlen(the_st->all_uds[i]->uds_fs_info->c_file_name, TYPE_MAX_SIZE);
    total_len = path_len + c_len + 2;
    path = calloc(total_len, sizeof(char));
    strncpy(path, the_st->all_uds[i]->uds_fs_info->each_sub_dir
          [the_st->all_uds[i]->uds_fs_info->qty_sub_dirs - 2], path_len);
    strncat(path, "/", c_len);
    strncat(path, the_st->all_uds[i]->uds_fs_info->c_file_name, c_len);
    fp = fopen(path, "w");
    write_include_statement(the_st->all_uds[i], fp);
    // Want a space between the defines and the libs
    fprintf(fp, "\n");
    write_functions(the_st->all_uds[i], fp);
    free(path);
    path = NULL;
    fclose(fp);
    fp = NULL;
  }
}

/**
 * This function is responsible for writing the include statement from the
 * given C file.
 * @param the_uds - The uds corresponding to the file to being written to.
 * @param      fp - The file pointer corresponding to the uds.
 * @return    N/a
 */
void write_include_statement(uds * the_uds, FILE * fp) {
  fprintf(fp, "#include \".%s/%s\"\n", INCLUDE_STRING,
      the_uds->uds_fs_info->h_file_name);
}

/**
 * This function is responsible for writing the functions from the given C file.
 * @param the_uds - The uds corresponding to the file to being written to.
 * @param      fp - The file pointer corresponding to the uds.
 * @return    N/a
 */
void write_functions(uds * the_uds, FILE * fp) {
  for(int i = 0; i < the_uds->no_methods; i++) {
    switch(the_uds->methods[i]) {
      case INIT:
        write_init_function(the_uds, fp);
        break;
      case DEBUG:
        write_debug_function(the_uds, fp);
        break;
      case DEEP_COPY:
        fprintf(stderr, "[WRITE_FUNCTIONS]: Deep Copy Not Yet Implemented\n");
        break;
      case FREE:
        write_free_function(the_uds, fp);
        break;
      case MT_NULL:
        fprintf(stderr, "[WRITE_FUNCTIONS]: MT_NULL passed\nExiting\n");
        exit(1);
    }
    fprintf(fp, "\n");
  }
}

void write_init_function(uds * the_uds, FILE * fp) {
  int void_flag = 0;
  fprintf(fp, "%s * init_%s(", the_uds->name, the_uds->name);
  for(int i = 0; i < the_uds->no_members; i++) {
    if(has_init_flag(the_uds->members[i]) && !void_flag) {
      void_flag = 1;
      fprintf(fp, "%s ", the_uds->members[i]->type->literal);
      write_deref_level(the_uds, fp, i);
      fprintf(fp, "the_%s", the_uds->members[i]->name);
    } else if(has_init_flag(the_uds->members[i]) && void_flag) {
      fprintf(fp, ", %s ", the_uds->members[i]->type->literal);
      write_deref_level(the_uds, fp, i);
      fprintf(fp, "the_%s", the_uds->members[i]->name);
    }
  }
  if(!void_flag)
    fprintf(fp, "void");
  fprintf(fp, ") {\n");
  write_tabstop(fp);
  fprintf(fp, "%s * the_%s = calloc(1, sizeof(struct ", the_uds->name,
      the_uds->name);
  for(int i = 0; i < (int)strnlen(the_uds->name, TYPE_MAX_SIZE); i++)
    // '- 32' is to capitalize the letter
    fprintf(fp, "%c", isalpha(the_uds->name[i]) ? the_uds->name[i] - 32
        : the_uds->name[i]);
  fprintf(fp, "_T));\n");
  for(int i = 0; i < the_uds->no_members; i++)
    write_member_initializer(the_uds->members[i], the_uds->name, fp);
  write_tabstop(fp);
  fprintf(fp, "return the_%s;\n", the_uds->name);
  fprintf(fp, "}\n");
}

/**
 * This function writes the initializing statement of a member of a uds to the
 * given file.
 * @param the_uds_member - This is the member of the uds that is being
 * initialized
 * @param    struct_name - The name of the structure whose member is being
 * initialized.
 * @param             fp - The file being printed to.
 * @return           N/a
 */
void write_member_initializer(uds_member * the_uds_member, char * struct_name,
    FILE * fp) {
  write_tabstop(fp);
  fprintf(fp, "the_%s->", struct_name);
  if(!has_init_flag(the_uds_member)) {
    if(the_uds_member->type->dereference_level > 0)
      fprintf(fp, "the_%s = NULL;\n", the_uds_member->name);
    else if(IS_INT(the_uds_member->type->literal, TYPE_MAX_SIZE))
      fprintf(fp, "the_%s = 0;\n", the_uds_member->name);
    else if(IS_DOUBLE(the_uds_member->type->literal, TYPE_MAX_SIZE))
      fprintf(fp, "the_%s = 0.0;\n", the_uds_member->name);
    else if(IS_CHAR(the_uds_member->type->literal, TYPE_MAX_SIZE))
      fprintf(fp, "the_%s = \'\\0\';\n", the_uds_member->name);
  } else {
    fprintf(fp, "the_%s = the_%s;\n", the_uds_member->name,
        the_uds_member->name);
  }
  if(has_qty_flag(the_uds_member)) {
    write_tabstop(fp);
    fprintf(fp, "the_%s->", struct_name);
    fprintf(fp, "qty_%s = 0;\n", the_uds_member->name);
  }
}

void write_debug_function(uds * the_uds, FILE * fp) {
  fprintf(fp, "void debug_%s(%s * the_%s) {\n", the_uds->name, the_uds->name,
      the_uds->name);
  write_tabstop(fp);
  fprintf(fp, "printf(\"[");
  for(int i = 0; i < (int)strnlen(the_uds->name, TYPE_MAX_SIZE); i++)
    fprintf(fp, "%c", isalpha(the_uds->name[i]) ? the_uds->name[i] - 32
        : the_uds->name[i]);
  fprintf(fp, "]\");\n");
  for(int i = 0; i < the_uds->no_members; i++)
    if(the_uds->members[i]->type->dereference_level == 0) {
      write_tabstop(fp);
      write_standard_type_printf(the_uds, the_uds->members[i], fp);
    } else {
      if(the_uds->members[i]->type->dereference_level == 1
          && IS_CHAR(the_uds->members[i]->type->literal, TYPE_MAX_SIZE)) {
        write_tabstop(fp);
        fprintf(fp, "printf(\"%s: `%%s`\\n\", the_%s->the_%s);\n",
            the_uds->name, the_uds->name, the_uds->members[i]->name);
      } else if(the_uds->members[i]->type->dereference_level == 1) {
        write_tabstop(fp);
        write_standard_type_printf_pointer(the_uds, the_uds->members[i], fp);
      }else if(has_qty_flag(the_uds->members[i])) {
        write_tabstop(fp);
        fprintf(fp, "for(int i = 0; i < the_%s->qty_%s; i++)\n",
            the_uds->name, the_uds->members[i]->name);
        write_n_tabstop(fp, 2);
        if(the_uds->members[i]->type->is_standard)
          write_standard_type_printf_windex(the_uds, the_uds->members[i], fp);
        else
          fprintf(fp, "debug_%s(the_%s->the_%s[i]);\n",
              the_uds->members[i]->type->literal, the_uds->name,
              the_uds->members[i]->name);
      } else if(!the_uds->members[i]->type->is_standard) {
        write_tabstop(fp);
        fprintf(fp, "debug_%s(the_%s->the_%s);\n",
            the_uds->members[i]->type->literal, the_uds->name,
            the_uds->members[i]->name);
      }
    }
  fprintf(fp, "}\n");
}

void write_standard_type_printf(uds * the_uds, uds_member * member,
    FILE * fp) {
  fprintf(fp, "printf(\"%s: ", member->name);
  if(IS_INT(member->type->literal, TYPE_MAX_SIZE))
    fprintf(fp, "%%d\\n\", the_%s->the_%s);\n", the_uds->name, member->name);
  if(IS_DOUBLE(member->type->literal, TYPE_MAX_SIZE))
    fprintf(fp, "%%f\\n\", the_%s->the_%s);\n", the_uds->name, member->name);
  if(IS_CHAR(member->type->literal, TYPE_MAX_SIZE))
    fprintf(fp, "%%c\\n\", the_%s->the_%s);\n", the_uds->name, member->name);
}

void write_standard_type_printf_windex(uds * the_uds, uds_member * member,
    FILE * fp) {
  fprintf(fp, "printf(\"%s: ", member->name);
  if(IS_INT(member->type->literal, TYPE_MAX_SIZE))
    fprintf(fp, "%%d\\n\", the_%s->the_%s[i]);\n", the_uds->name, member->name);
  if(IS_DOUBLE(member->type->literal, TYPE_MAX_SIZE))
    fprintf(fp, "%%f\\n\", the_%s->the_%s[i]);\n", the_uds->name, member->name);
  if(IS_CHAR(member->type->literal, TYPE_MAX_SIZE))
    fprintf(fp, "%%s\\n\", the_%s->the_%s[i]);\n", the_uds->name, member->name);
}

void write_standard_type_printf_pointer(uds * the_uds, uds_member * member,
    FILE * fp) {
  fprintf(fp, "if(the_%s->the_%s)\n", the_uds->name, member->name);
  write_n_tabstop(fp, 2);
  fprintf(fp, "printf(\"%s: ", member->name);
  if(IS_INT(member->type->literal, TYPE_MAX_SIZE))
    fprintf(fp, "%%d\\n\", *(the_%s->the_%s));\n", the_uds->name, member->name);
  if(IS_DOUBLE(member->type->literal, TYPE_MAX_SIZE))
    fprintf(fp, "%%f\\n\", *(the_%s->the_%s));\n", the_uds->name, member->name);
}

void write_free_function(uds * the_uds, FILE * fp) {
  fprintf(fp, "void free_%s(%s * the_%s) {\n", the_uds->name, the_uds->name,
      the_uds->name);
  write_tabstop(fp);
  fprintf(fp, "if(the_%s) {\n", the_uds->name);
  for(int i = 0; i < the_uds->no_members; i++)
    if(the_uds->members[i]->type->dereference_level > 0) {
      if(has_qty_flag(the_uds->members[i])) {
        write_n_tabstop(fp, 2);
        fprintf(fp, "if(the_%s->the_%s) {\n", the_uds->name,
            the_uds->members[i]->name);
        write_n_tabstop(fp, 3);
        fprintf(fp, "for(int i = 0; i < the_%s->qty_%s; i++)\n",
            the_uds->name, the_uds->members[i]->name);
        write_n_tabstop(fp, 4);
        fprintf(fp, "if(the_%s->the_%s[i])\n",
            the_uds->name, the_uds->members[i]->name);
        write_n_tabstop(fp, 5);
        if(the_uds->members[i]->type->is_standard)
          fprintf(fp, "free(the_%s->the_%s[i]);\n",
              the_uds->name, the_uds->members[i]->name);
        else
          fprintf(fp, "free_%s(the_%s->the_%s[i]);\n",
              the_uds->members[i]->type->literal, the_uds->name,
              the_uds->members[i]->name);
        write_n_tabstop(fp, 3);
        fprintf(fp, "free(the_%s->the_%s);\n",
            the_uds->name, the_uds->members[i]->name);
        write_n_tabstop(fp, 2);
        fprintf(fp, "}\n");
      } else {
        write_n_tabstop(fp, 2);
        fprintf(fp, "if(the_%s->the_%s)\n", the_uds->name,
            the_uds->members[i]->name);
        write_n_tabstop(fp, 3);
        fprintf(fp, "free(the_%s->the_%s);\n", the_uds->name,
            the_uds->members[i]->name);
      }
    }
  write_n_tabstop(fp, 2);
  fprintf(fp, "free(the_%s);\n", the_uds->name);
  write_tabstop(fp);
  fprintf(fp, "}\n");

  fprintf(fp, "}\n");
}
