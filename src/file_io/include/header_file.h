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

#include <ctype.h>
#include <sys/stat.h>
#include <errno.h>

#include "../../uds/include/symbol_table.h"
#include "basic_funcs.h"

#define HEADER_DEF_LEN 3

void write_header_file(symbol_table * the_st);
void write_directories(symbol_table * the_st);
void write_header_defines(uds * the_uds, FILE * fp);
void write_libs(uds * the_uds, FILE * fp);
void write_structure_definition(uds * the_uds, FILE * fp);
void write_function_definitions(uds * the_uds, FILE * fp);

#endif
