/**
 * @file   uds.h
 * @brief  This function contains the function definitions for uds.c
 * @author Matthew C. Lindeman
 * @date   December 24, 2022
 * @bug    None known
 * @todo   Nothing
 */
#ifndef UDS_H
#define UDS_H

#include "uds_member.h"
#include "method_type.h"
#include "libs.h"

#define H_EXTENSION ".h"

typedef struct UDS_T {
  uds_member ** members;
  method_type * methods;
  libs * req_libs;
  char * name;
  char * path;
  char * file_name;
  char * h_file_name;
  int no_members;
  int no_methods;
  int no_req_libs;
} uds;

uds * init_uds(char * name, char * path, char * file_name,
    method_type * methods, int no_methods);
uds * make_header_file(uds * the_uds);
uds * make_req_libs(uds * the_uds);
uds * add_lib(uds * the_uds, libs the_lib);
uds * add_uds_member(uds * the_uds, uds_member * new_uds_member);
void debug_uds(uds * the_uds);
void free_uds(uds * the_uds);

#endif
