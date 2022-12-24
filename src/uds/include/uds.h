#ifndef UDS_H
#define UDS_H

#include "uds_member.h"
#include "method_type.h"

typedef struct UDS_T {
  uds_member ** members;
  method_type * methods;
  char * name;
  char * path;
  int no_members;
  int no_methods;
} uds;

uds * init_uds(char * name, char * path, method_type * methods, int no_methods);
uds * add_uds_member(uds * the_uds, uds_member * new_uds_member);
void debug_uds(uds * the_uds);
void free_uds(uds * the_uds);

#endif
