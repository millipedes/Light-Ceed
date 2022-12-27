/**
 * @file   file_io.c
 * @brief  This file contains the functions which write the files from the
 * symbol_table.
 * @author Matthew C. Lindeman
 * @date   December 24, 2022
 * @bug    None known
 * @todo   Nothing
 */
#include "include/file_io.h"

/**
 * This function calls all required functions that write the files specified by
 * the symbol_table.
 * @param the_st - The symbol_table to be written.
 * @return   N/a
 */
void write_symbol_table(symbol_table * the_st) {
  write_header_file(the_st);
  write_c_file(the_st);
}
