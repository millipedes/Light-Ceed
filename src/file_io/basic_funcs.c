/**
 * @file   basic_funcs.c
 * @brief  This file contains the functions that all of the writing files
 * require. This is to avoid repeated code and circular include statements.
 * @author Matthew C. Lindeman
 * @date   December 27, 2022
 * @bug    None known
 * @todo   Nothing
 */
#include "include/basic_funcs.h"

/**
 * This function writes the dereference_level for uds member of index i. This
 * function is repeated several times, makes sense to be relegated to its own
 * function.
 * @param the_uds - The uds to write to.
 * @param     fp - The file pointer which contains the header file being written
 * to.
 * @param      i - The index of the uds member whose dr level is being written.
 * @return   N/a
 */
void write_deref_level(uds * the_uds, FILE * fp, int i) {
  for(int j = 0; j < the_uds->members[i]->type->dereference_level; j++) {
    fprintf(fp, "*");
    if(j == the_uds->members[i]->type->dereference_level - 1)
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
