#ifndef FIO_H
#define FIO_H

#include <sys/stat.h>
#include <errno.h>

#include "../../uds/include/symbol_table.h"

#define INCLUDE_STRING "/include"

void write_symbol_table(symbol_table * the_st);
void write_directories(symbol_table * the_st);
int partition_sub_dirs(char * path, char *** each_sub_dir);
void write_struct_definition(symbol_table * the_st);
int write_init_function(symbol_table * the_st);
int write_debug_function(symbol_table * the_st);
int write_deep_copy_function(symbol_table * the_st);
int write_free_function(symbol_table * the_st);

#endif
