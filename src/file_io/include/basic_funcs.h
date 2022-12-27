/**
 * @file   basic_funcs.c
 * @brief  This file contains the function definitions of basic_funcs.c
 * @author Matthew C. Lindeman
 * @date   December 27, 2022
 * @bug    None known
 * @todo   Nothing
 */
#ifndef BFNC_H
#define BFNC_H

#include <stdlib.h>
#include "../../uds/include/uds.h"

#define TABSTOP 2

void write_deref_level(uds * the_uds, FILE * fp, int i);
void write_n_tabstop(FILE * fp, int n);
void write_tabstop(FILE * fp);

#endif
