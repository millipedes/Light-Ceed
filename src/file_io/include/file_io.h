#ifndef FIO_H
#define FIO_H

#include <sys/stat.h>
#include <errno.h>

#include "../../uds/include/symbol_table.h"

#define INCLUDE_STRING "/include"
#define HEADER_DEF_LEN 3
#define TABSTOP 2

void write_symbol_table(symbol_table * the_st);
void write_directories(symbol_table * the_st);
int partition_sub_dirs(char * path, char *** each_sub_dir);
void write_header_file(symbol_table * the_st);
void write_header_defines(symbol_table * the_st, FILE * fp, int i);
void write_libs(symbol_table * the_st, FILE * fp, int i);
void write_structure_definition(symbol_table * the_st, FILE * fp, int i);
void write_function_definitions(symbol_table * the_st, FILE * fp, int i);
void write_deref_level(symbol_table * the_st, FILE * fp, int i, int j);
void write_tabstop(FILE * fp);
// void write_struct_definition(symbol_table * the_st);
// int write_init_function(symbol_table * the_st);
// int write_debug_function(symbol_table * the_st);
// int write_deep_copy_function(symbol_table * the_st);
// int write_free_function(symbol_table * the_st);

#endif
