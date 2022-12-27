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
#include "../../fs/include/fs_info.h"

#define STDIO  "<stdio.h>"
#define STDLIB "<stdlib.h>"
#define STRING "<string.h>"
#define IS_STANDARD_LIB(x) (!strncmp(x, STDIO,  TYPE_MAX_SIZE) ||     \
                            !strncmp(x, STDLIB, TYPE_MAX_SIZE) ||     \
                            !strncmp(x, STRING, TYPE_MAX_SIZE)) ? 1 : 0

typedef struct UDS_T {
  uds_member ** members;
  method_type * methods;
  char ** req_libs;
  fs_info * uds_fs_info;
  char * name;
  int no_members;
  int no_methods;
  int no_req_libs;
} uds;

uds * init_uds(char * name, char * path, char * file_name,
    method_type * methods, int no_methods);
uds * make_req_libs(uds * the_uds);
uds * add_lib(uds * the_uds, char * the_lib);
uds * add_uds_member(uds * the_uds, uds_member * new_uds_member);
void debug_uds(uds * the_uds);
void free_uds(uds * the_uds);

#endif
