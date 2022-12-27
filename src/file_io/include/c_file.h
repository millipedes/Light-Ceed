#ifndef CFI_H
#define CFI_H

#include "../../uds/include/symbol_table.h"
#include "basic_funcs.h"

void write_c_file(symbol_table * the_st);
void write_include_statement(uds * the_uds, FILE * fp);
void write_functions(uds * the_uds, FILE * fp);
void write_init_function(uds * the_uds, FILE * fp);
void write_member_initializer(uds_member * the_uds_member, char * struct_name,
    FILE * fp);

#endif
