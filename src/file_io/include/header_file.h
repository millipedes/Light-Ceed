/**
 * @file   header_file.h
 * @brief  This file contains the function definitions for header_file.c
 * @author Matthew C. Lindeman
 * @date   December 24, 2022
 * @bug    None known
 * @todo   Nothing
 */
#ifndef HFI_H
#define HFI_H

#include <sys/stat.h>
#include <errno.h>

#include "../../uds/include/symbol_table.h"

#define INCLUDE_STRING "/include"
#define HEADER_DEF_LEN 3
#define TABSTOP 2

void write_header_file(symbol_table * the_st);
void write_directories(symbol_table * the_st);
int partition_sub_dirs(char * path, char *** each_sub_dir);
void write_header_defines(symbol_table * the_st, FILE * fp, int i);
void write_libs(symbol_table * the_st, FILE * fp, int i);
void write_structure_definition(symbol_table * the_st, FILE * fp, int i);
void write_function_definitions(symbol_table * the_st, FILE * fp, int i);
void write_deref_level(symbol_table * the_st, FILE * fp, int i, int j);
void write_tabstop(FILE * fp);

#endif
